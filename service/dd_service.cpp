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
	ddServicePrivate() :m_isServer(no), m_busWatchId(0), m_busOwnId(0), m_pService(nil) {
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
		dd_log_d("ddService,onGBusNameAppearedCallback,name:%s,name_owner:%s\n", name, name_owner);
		if ( !pthis->m_isServer ) {
			pthis->startupSelf();
		}
	}

	static ddVoid onGBusNameVanishedCallback(GDBusConnection *connection, const gchar *name, gpointer user_data) {
		ddServicePrivate *pthis = (ddServicePrivate *)user_data;
		dd_log_d("ddService,onGBusNameVanishedCallback,name:%s,isServer:%d\n", name, pthis->m_isServer);
		if ( !pthis->m_isServer ) {
			ddService::startupServer();
		} else {
			pthis->startupSelf();
		}
	}

	static ddVoid onGBusAcquiredCallback(GDBusConnection *connection, const gchar *name, gpointer user_data) {
		ddServicePrivate * pthis = (ddServicePrivate *)user_data;
		dd_log_d("ddService,onGBusAcquiredCallback entry,name:%s,isServer:%d\n", name, pthis->m_isServer);
		if ( pthis->m_isServer ) {
			/** Second step: Try to get a connection to the given bus. */
			pthis->m_pService = com_dd_service_skeleton_new();
			if ( !pthis->m_pService ) {
				dd_log_e2("ddService,onGBusAcquiredCallback,new skeleton failed\n");
				return;
			}

			/** Third step: Attach to dbus signals. */
			g_signal_connect(pthis->m_pService, "handle-ioctl", G_CALLBACK(on_handle_ioctl), nil);
			g_signal_connect(pthis->m_pService, "handle-test", G_CALLBACK(on_handle_test), nil);

			/** Fourth step: Export interface skeleton. */
			GError *error = NULL;
			g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(pthis->m_pService), connection, pthis->objectPath(), &error);
			if ( error ) {
				dd_log_e("ddService,onGBusAcquiredCallback,g_dbus_interface_skeleton_export,error:%s\n", error->message);
				g_error_free(error);
				error = nil;
			}
		} else {
			/** Second step: try to get a connection to the given bus.*/
			GError *pProxyError = nil;
			pthis->m_pService = com_dd_service_proxy_new_sync(connection, G_DBUS_PROXY_FLAGS_NONE,
					pthis->busName(), pthis->objectPath(), nil, &pProxyError);
			if ( pProxyError ) {
				dd_log_w("ddService,onGBusAcquiredCallback,failed to create proxy,error:%s\n", pProxyError->message);
				g_error_free(pProxyError);
				pProxyError = nil;
			}
			if ( !pthis->m_pService ){
				dd_log_e("new proxy failed,error:%s\n", strerror(errno));
				return;
			}

			/** Third step: Attach to dbus signals */
//			g_signal_connect(pthis->m_pService, "notify::g-name-owner", G_CALLBACK(cb_OwnerNameChangedNotify), NULL);
			g_signal_connect(pthis->m_pService, "notification", G_CALLBACK(on_notification), NULL);
			g_signal_connect(pthis->m_pService, "test", G_CALLBACK(on_test), NULL);
		}
	}

	static ddVoid onGBusNameAcquiredCallback(GDBusConnection *connection, const gchar *name, gpointer user_data) {
		dd_log_d("ddService,onGBusNameAcquiredCallback,name:%s\n", name);
	}

	static ddVoid onGBusNameLostCallback(GDBusConnection *connection, const gchar *name, gpointer user_data) {
		dd_log_d("ddService,onGBusNameLostCallback,name:%s\n", name);
	}

	static ddVoid onCallIoctlCallback(GObject *source_object, GAsyncResult *res, gpointer user_data) {
		ddServicePrivate * pthis = (ddServicePrivate *)user_data;
		GVariant *out_pout = nil;
		GError *pError = nil;
		com_dd_service_call_ioctl_finish(pthis->m_pService, &out_pout, res, &pError);
		if ( pError ) {
			dd_log_w("ddService,onCallIoctlCallback,com_dd_service_call_ioctl_finish,error:%s\n", pError->message);
			g_error_free(pError);
			pError = nil;
		}
	}

	static ddVoid onCallTestCallback(GObject *source_object, GAsyncResult *res, gpointer user_data) {
		ddServicePrivate* pthis = (ddServicePrivate *)user_data;
		gint* pOut = nil;
		GError *pError = nil;
		com_dd_service_call_test_finish(pthis->m_pService, pOut, res, &pError);
		if ( pError ) {
			dd_log_w("ddService,onCallTestCallback,com_dd_service_call_test_finish,error:%s\n", pError->message);
			g_error_free(pError);
			pError = nil;
		}
	}

	static gboolean on_handle_ioctl(ComDdService *object, GDBusMethodInvocation *invocation, 
			guint arg_iocmd, GVariant *arg_pin, guint arg_uin, guint arg_uout) {
		dd_log_d("on_handle_ioctl,iocmd:%d,uin:%d,uout:%d\n", arg_iocmd, arg_uin, arg_uout);
		gsize uin = 0;
		const guchar *pin = (const guchar *)g_variant_get_fixed_array(arg_pin, &uin, sizeof(guchar));
		ddByte pout[DD_MAXPATH] = {0};
		memset(pout, 0, DD_MAXPATH);
		DD_GLOBAL_INSTANCE_DO(ddDevManager, ioctl(arg_iocmd, pin, uin, pout, arg_uout));
//		arg_uout = !arg_uout ? 4 : arg_uout;
		GVariant *out_pout = g_variant_new_fixed_array(G_VARIANT_TYPE_BYTE, pout, arg_uout, sizeof(ddByte));
		com_dd_service_complete_ioctl(object, invocation, out_pout);
		return yes;
	} 

	static gboolean on_handle_test(ComDdService *object, GDBusMethodInvocation *invocation, gint arg_pin) {
		dd_log_d("on_handle_test,arg_pin:%d\n", arg_pin);
		gint pout = arg_pin + 1;
		com_dd_service_complete_test(object, invocation, pout);
		return yes;
	}

	static ddVoid on_notification(ComDdService *object, GVariant *arg_data) {
		dd_log_d2("on_notification\n");
		gsize uin = 0;
		const guchar *pin = (const guchar *)g_variant_get_fixed_array(arg_data, &uin, sizeof(guchar));
		DD_GLOBAL_INSTANCE_DO(ddSrvManager, notify(pin, uin));
	}

	static ddVoid on_test(ComDdService *object, gint arg_data) {
		dd_log_d("on_test,arg_data:%d\n", arg_data);
	}

	ddBool startup(ddpCChar pName, ddBool isServer) {
		m_isServer = isServer;
		m_strName = pName;
		m_busWatchId = g_bus_watch_name (G_BUS_TYPE_SESSION, busName(),
				  G_BUS_NAME_WATCHER_FLAGS_NONE,
                  onGBusNameAppearedCallback,
                  onGBusNameVanishedCallback,
                  this, nil);
		return m_busWatchId > 0;
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

	ddVoid ioctl(ddUInt16 iocmd, ddCPointer pIn, ddUInt16 uIn, ddPointer pOut, ddUInt16 uOut) {
		uIn = uIn > DD_MAXPATH ? DD_MAXPATH : uIn;
		GVariant *arg_pin = g_variant_new_fixed_array (G_VARIANT_TYPE_BYTE, pIn, uIn, sizeof(ddByte));
		if ( arg_pin ) {
			if ( pOut && uOut ) {
				uOut = uOut > DD_MAXPATH ? DD_MAXPATH : uOut;
				GError *pError = nil;
				GVariant *out_pout = nil;
				com_dd_service_call_ioctl_sync(m_pService, iocmd, arg_pin, uIn, uOut, &out_pout, nil, &pError);
				if ( pError ) {
					dd_log_i("ioctl,com_dd_service_call_ioctl_sync,error:%s\n", pError->message);
					g_error_free(pError);
					pError = nil;
				}
				if ( out_pout ) {
					gsize n_elements = 0;
					const guchar *pout = (const guchar *)g_variant_get_fixed_array(out_pout, &n_elements, sizeof(guchar));
					if ( pout ) {
						memcpy(pOut, pout, n_elements > uOut ? uOut : n_elements);
					}
				}
			} else {
				com_dd_service_call_ioctl(m_pService, iocmd, arg_pin, uIn, 0, nil, onCallIoctlCallback, this);
			}
		}
	}

	ddVoid notify(ddCPointer pIn, ddUInt16 uIn) {
		uIn = uIn > DD_MAXPATH ? DD_MAXPATH : uIn;
		GVariant *arg_pin = g_variant_new_fixed_array (G_VARIANT_TYPE_BYTE, pIn, uIn, sizeof(ddByte));
		if ( arg_pin ) {
			com_dd_service_emit_notification(m_pService, arg_pin);
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

///////////////////////////////////////////////////////////////////////////////
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
	ddChar path[DD_MAXPATH] = {0};
	if ( ddProcess::getModulePath(path, DD_MAXPATH) ) {
		strcat(path, "ddServer");
		ddBool ret = ddProcess::createProcess(path, NULL);
		dd_log_d("startupServer,name:%s,ret:%d\n", path, ret);
	}
}

ddVoid ddService::startup(ddpCChar pName, ddBool isServer/* = no*/)
{
	GError *pConnError = NULL;
	GDBusConnection *pConnection = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, &pConnError);
	if ( pConnError ) {
		dd_log_w("startup,failed to connect to dbus,error:%s\n", pConnError->message);
		g_error_free(pConnError);
		pConnError = NULL;
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
		dd_log_i("startup,dbus-launch name:%s, data:%s\n", name, data);
		setenv(name, /*data*/value, 0);
		// ----- //
		usleep(20* 1000);
		pConnection = g_bus_get_sync(G_BUS_TYPE_SESSION, NULL, &pConnError);
		if ( pConnError ) {
			dd_log_w("ddService,startup,failed to connect to dbus again,error:%s\n", pConnError->message);
			g_error_free(pConnError);
			pConnError = NULL;
		}
	}
	if ( pConnection ) {
		g_object_unref(pConnection);
	   	pConnection = nil;
	}
	if ( pName ) {
		if ( ddServicePrivate *pInstance = ddGlobalInstance<ddServicePrivate>::instance() ) {
			ddBool isOk = pInstance->startup(pName, isServer);
			if ( isOk ) {
				if ( isServer ) {
					ddGlobalInstance<ddDevManager>::instance();
				} else {
					ddGlobalInstance<ddSrvManager>::instance();
				}
			}
		}
	}
}

ddVoid ddService::ioctl(ddUInt16 iocmd, ddCPointer pIn, ddUInt16 uIn, ddPointer pOut/* = nil*/, ddUInt16 uOut/* = 0*/)
{
	if ( pIn && uIn ) {
		DD_GLOBAL_INSTANCE_DO(ddServicePrivate, ioctl(iocmd, pIn, uIn, pOut, uOut));
	}
}

ddVoid ddService::download(ddpCByte data, ddUInt16 len)
{
	if ( data && len ) {
		DD_GLOBAL_INSTANCE_DO(ddServicePrivate, ioctl(DDDEF_IOCOMMAND_PROTOCOL, data, len, nil, 0));
	}
}

ddVoid ddService::commit(ddpCByte data, ddUInt16 len)
{
	if ( data && len ) {
		DD_GLOBAL_INSTANCE_DO(ddServicePrivate, notify(data, len));
	}
}

