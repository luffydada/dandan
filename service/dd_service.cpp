/***************************************************************************
*    File name:        dd_service.cpp
*    Description:
*    Create date:      2016-11-23 21:46
*    Modified date:    2016-11-23 21:46
*    Version:          V1.0
*    Author:           wangying
***************************************************************************/
#include <glib-2.0/glib.h>

#include "dd-daemon.h"
#include "../dandan.h"
class ddServicePrivate : public ddPrivateBase {
	DD_PUBLIC_DECLARE(ddService)
public:
/*	static ddVoid cb_OwnerNameChangedNotify(GObject *object, GParamSpec *pspec, gpointer userdata) {
		g_print("ddService,cb_OwnerNameChangedNotify\n");

		gchar *pname_owner = g_dbus_proxy_get_name_owner((GDBusProxy*)object);
		if ( pname_owner ){
			g_print("ddService,DBus service is ready!\n");
			g_free(pname_owner);
		}
		else{
			g_print("ddService,DBus service is NOT ready!\n");
			g_free(pname_owner);
		}
	}*/

	static ddVoid onGBusNameAppearedCallback(GDBusConnection *connection, const gchar *name, const gchar *name_owner, gpointer user_data) {
		ddServicePrivate * pthis = (ddServicePrivate *)user_data;
		g_print("ddService,onGBusNameAppearedCallback,name:%s,name_owner:%s\n", name, name_owner);
		if ( !pthis->m_isServer ) {
			pthis->startupSelf();
		}
	}

	static ddVoid onGBusNameVanishedCallback(GDBusConnection *connection, const gchar *name, gpointer user_data) {
		ddServicePrivate *pthis = (ddServicePrivate *)user_data;
		g_print("ddService,onGBusNameVanishedCallback,name:%s,isServer:%d\n", name, pthis->m_isServer);
		if ( !pthis->m_isServer ) {
			ddService::startupServer();
		} else {
			pthis->startupSelf();
		}
	}

	static ddVoid onGBusAcquiredCallback(GDBusConnection *connection, const gchar *name, gpointer user_data) {
		ddServicePrivate * pthis = (ddServicePrivate *)user_data;
		g_print("ddService,onGBusAcquiredCallback,name:%s,isServer:%d\n", name, pthis->m_isServer);
		if ( pthis->m_isServer ) {
			/** Second step: Try to get a connection to the given bus. */
			pthis->m_pService = com_dd_service_skeleton_new();
			if ( !pthis->m_pService ) {
				g_print("ddService,onGBusAcquiredCallback,new skeleton failed\n");
				return;
			}

			/** Third step: Attach to dbus signals. */
			g_signal_connect(pthis->m_pService, "handle-ioctl", G_CALLBACK(on_handle_ioctl), nil);
			g_signal_connect(pthis->m_pService, "handle-test", G_CALLBACK(on_handle_test), nil);

			/** Fourth step: Export interface skeleton. */
			GError *error = NULL;
			g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(pthis->m_pService), connection, pthis->objectPath(), &error);
			if ( error ) {
				g_print("ddService,onGBusAcquiredCallback,g_dbus_interface_skeleton_export,error:%s\n", error->message);
				g_error_free(error);
			}
		} else {
			/** Second step: try to get a connection to the given bus.*/
			g_print("Second step\n");
			GError *pProxyError = nil;
			pthis->m_pService = com_dd_service_proxy_new_sync(connection, G_DBUS_PROXY_FLAGS_NONE,
					pthis->busName(), pthis->objectPath(), nil, &pProxyError);
			if ( !pProxyError ){
				g_print("ddService,onGBusAcquiredCallback,failed to create proxy,error:%s\n", pProxyError->message);
				g_error_free(pProxyError);
			}
			if ( !pthis->m_pService ){
				return;
			}

			g_print("Third step\n");
			/** Third step: Attach to dbus signals */
//			g_signal_connect(pthis->m_pService, "notify::g-name-owner", G_CALLBACK(cb_OwnerNameChangedNotify), NULL);
			g_signal_connect(pthis->m_pService, "notification", G_CALLBACK(on_notification), NULL);
			g_signal_connect(pthis->m_pService, "test", G_CALLBACK(on_test), NULL);
		}
	}

	static ddVoid onGBusNameAcquiredCallback(GDBusConnection *connection, const gchar *name, gpointer user_data) {
		g_print("ddService,onGBusNameAcquiredCallback,name:%s\n", name);
	}

	static ddVoid onGBusNameLostCallback(GDBusConnection *connection, const gchar *name, gpointer user_data) {
		g_print("ddService,onGBusNameLostCallback,name:%s\n", name);
	}

	static ddVoid onCallIoctlCallback(GObject *source_object, GAsyncResult *res, gpointer user_data) {
		ddServicePrivate * pthis = (ddServicePrivate *)user_data;
		GVariant *out_pout = nil;
		GError *pError = nil;
		com_dd_service_call_ioctl_finish(pthis->m_pService, &out_pout, res, &pError);
		if ( pError ) {
			g_print("ddService,onCallIoctlCallback,com_dd_service_call_ioctl_finish,error:%s\n", pError->message);
			g_error_free(pError);
		}
	}

	static ddVoid onCallTestCallback(GObject *source_object, GAsyncResult *res, gpointer user_data) {
		ddServicePrivate* pthis = (ddServicePrivate *)user_data;
		gint* pOut = nil;
		GError *pError = nil;
		com_dd_service_call_test_finish(pthis->m_pService, pOut, res, &pError);
		if ( pError ) {
			g_print("ddService,onCallTestCallback,com_dd_service_call_test_finish,error:%s\n", pError->message);
			g_error_free(pError);
		}
	}

	static gboolean on_handle_ioctl(ComDdService *object, GDBusMethodInvocation *invocation, 
			GVariant *arg_pin, guint arg_uin, guint arg_uout) {
		g_print("ddService,on_handle_ioctl,arg_uin:%d,arg_uout:%d\n", arg_uin, arg_uout);
		GVariant *out_pout = nil;
		com_dd_service_complete_ioctl(object, invocation, out_pout);
		return yes;
	} 

	static gboolean on_handle_test(ComDdService *object, GDBusMethodInvocation *invocation, gint arg_pin) {
		g_print("ddService,on_handle_test,arg_pin:%d\n", arg_pin);
		gint pout = arg_pin + 1;
		com_dd_service_complete_test(object, invocation, pout);
		return yes;
	}

	static ddVoid on_notification(ComDdService *object, GVariant *arg_data) {
		g_print("ddService,on_notification\n");
	}

	static ddVoid on_test(ComDdService *object, gint arg_data) {
		g_print("ddService,on_test,arg_data:%d\n", arg_data);
	}

	ddServicePrivate() :m_isServer(no), m_busWatchId(0), m_busOwnId(0) {
	}

	~ddServicePrivate() {
		if ( m_busWatchId ) {
			g_bus_unwatch_name(m_busWatchId);
		}
		if ( m_busOwnId ) {
			g_bus_unown_name(m_busOwnId);
		}
		if ( m_pService ) {
			g_object_unref(m_pService);
		}
	}

	ddVoid startup(ddpCChar pName, ddBool isServer) {
		m_isServer = isServer;
		m_strName = pName;
		m_busWatchId = g_bus_watch_name (G_BUS_TYPE_SESSION, isServer ? busName() : uniqueName(pName),
				  G_BUS_NAME_WATCHER_FLAGS_NONE,
                  onGBusNameAppearedCallback,
                  onGBusNameVanishedCallback,
                  this, nil);

	}

	ddVoid startupSelf() {
		/** first step: connect to dbus */
		m_busOwnId = g_bus_own_name(G_BUS_TYPE_SESSION,
		   	m_isServer ? busName() : uniqueName(m_strName.data()),
		   	G_BUS_NAME_OWNER_FLAGS_NONE,
			onGBusAcquiredCallback,
			onGBusNameAcquiredCallback,
			onGBusNameLostCallback,
			this, nil);
	}

	ddVoid ioctl(ddpCByte pIn, ddUInt16 uIn, ddpByte pOut, ddUInt16 uOut) {
		GVariant *arg_pin = g_variant_new_fixed_array (G_VARIANT_TYPE_BYTE, pIn, uIn, sizeof(ddByte));
		if ( arg_pin ) {
			if ( pOut && uOut ) {
				GError *pError = nil;
				GVariant *out_pout = nil;
				com_dd_service_call_ioctl_sync(m_pService, arg_pin, uIn, uOut, &out_pout, nil, &pError);
				if ( pError ) {
					g_print("ddService,ioctl,com_dd_service_call_ioctl_sync,error:%s\n", pError->message);
					g_error_free(pError);
				}
				if ( out_pout ) {
					gsize n_elements = 0;
					const guchar *pout = (const guchar *)g_variant_get_fixed_array(out_pout, &n_elements, sizeof(guchar));
					if ( pout ) {
						memcpy(pOut, pout, n_elements > uOut ? uOut : n_elements);
					}
				}
			} else {
				com_dd_service_call_ioctl(m_pService, arg_pin, uIn, 0, nil, onCallIoctlCallback, this);
			}
		}
	}

	ddpCChar busName() { return "com.dd.service"; }
	ddpCChar objectPath() { return "/com/dd/service"; }
	ddpCChar uniqueName(ddpCChar pName) {
		static ddChar name[64] = {0};
		sprintf(name, "dd.eddy.%s", pName);
	   	return name;
	}
private:
	ddBool m_isServer;
	guint m_busWatchId;
	guint m_busOwnId;
	ComDdService *m_pService;
	std::string m_strName;
};

ddService::ddService()
{
	DD_D_NEW(ddServicePrivate);
}

ddService::~ddService()
{
	DD_D_DELETE();
}

ddInt ddService::execsh(ddpCChar cmd, std::vector<std::string> &resvec)
{
	FILE *pp = popen(cmd, "r");

	if ( !pp ) {
		return -1;
	}

	int  length = 0;
    gchar tmp[1024] = { 0 }; //设置一个合适的长度，以存储每一行输出
    resvec.clear();

    while ( fgets(tmp, sizeof(tmp), pp ) ) {
		length = strlen(tmp);
        if ( tmp[length - 1] == '\n' )
            tmp[length - 1] = '\0'; //去除换行符
        resvec.push_back(tmp);
    }

    pclose(pp);
    return resvec.size();
}

ddVoid ddService::startupServer()
{
	pid_t pid = fork();
	if ( pid < 0 ) {
		g_print("ddService,fork child process error:%d\n", errno);
	} else if ( 0 == pid ) {
		execv("./ddServer", NULL);
		g_print("ddService,execv error:%d\n", errno);
	}
}

ddVoid ddService::startup(ddpCChar pName, ddBool isServer/* = no*/)
{
	GError *pConnError = NULL;
	GDBusConnection *pConnection = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, &pConnError);
	if ( pConnError ) {
		g_print("ddService,startup,failed to connect to dbus,error:%s\n", pConnError->message);
		g_error_free(pConnError);
	}
	if ( !pConnection ) {
		gchar name[50] = "DBUS_SESSION_BUS_ADDRESS";
		const gchar *value = NULL;
		std::vector<std::string> vec;
		execsh("dbus-launch", vec);
		value = vec[0].c_str() + strlen(name) + 1;

		char buffer[1024] = {0}, data[256] = {0};
		strcpy(buffer, vec[0].data()+ strlen(name) + 1);
		char *pos = strchr(buffer, ',');
		strncpy(data, buffer, strlen(buffer) - strlen(pos));
		g_print("server,dbus-launch name:%s, data:%s\n", name, data);
		setenv(name, /*data*/value, 0);
		// ----- //
		usleep(20* 1000);
		pConnection = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, &pConnError);
		if ( pConnError ) {
			g_print("ddService,startup,failed to connect to dbus again,error:%s\n", pConnError->message);
			g_error_free(pConnError);
		}
	}
	if ( pConnection ) {
		g_object_unref(pConnection);
	   	pConnection = nil;
	}
	if ( pName ) {
		DD_GLOBAL_INSTANCE_DO(ddService, dPtr()->startup(pName, isServer));
	}
}

ddVoid ddService::ioctl(ddpCByte pIn, ddUInt16 uIn, ddpByte pOut/* = nil*/, ddUInt16 uOut/* = 0*/)
{
	if ( pIn && uIn ) {
		DD_GLOBAL_INSTANCE_DO(ddService, dPtr()->ioctl(pIn, uIn, pOut, uOut));
	}
}

