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

	static ddVoid onGBusNameAppearedCallback(GDBusConnection *connection, const gchar *name, const gchar *name_owner, gpointer user_data) {
		g_print("server,onGBusNameAppearedCallback,name:%s,name_owner:%s\n", name, name_owner);
	}

	ddVoid onGBusNameVanishedCallback(GDBusConnection *connection, const gchar *name, gpointer user_data) {
		g_print("server,onGBusNameVanishedCallback,name:%s\n", name);
	}

	gboolean on_handle_function1(ComEddyGdbusDemo *object, GDBusMethodInvocation *invocation, guint arg_uin) {
		g_print("server,on_handle_function1,uin:%d\n", arg_uin);
		guint uout = arg_uin + 1;
		com_eddy_gdbus_demo_complete_function1(object, invocation, uout);

		com_eddy_gdbus_demo_emit_signal1(g_pSkeleton, 1);
		return true;
	} 

	gboolean on_handle_function2(ComEddyGdbusDemo *object, GDBusMethodInvocation *invocation, const gchar *arg_pin)
	{
		g_print("server,on_handle_function2,pin:%s\n", arg_pin);
		gchar out[256] = {0};
		strcpy(out, "I have recv your msg");
		sprintf(out, "%s:%s", out, arg_pin);
		com_eddy_gdbus_demo_complete_function2 (object,invocation, out);

		com_eddy_gdbus_demo_emit_signal2(g_pSkeleton, "Hi, I send a signal to you, do you copy?");
		return true;
	}

	void onGBusAcquiredCallback(GDBusConnection *connection, const gchar *name, gpointer user_data)
	{
		g_print("server,onGBusAcquiredCallback,name:%s\n", name);
		/** Second step: Try to get a connection to the given bus. */
		g_pSkeleton = com_eddy_gdbus_demo_skeleton_new();
		if ( !g_pSkeleton ) {
			g_print("server,onGBusAcquiredCallback,new skeleton failed\n");
			return;
		}

		/** Third step: Attach to dbus signals. */
		g_signal_connect(g_pSkeleton, "handle-function1", G_CALLBACK(on_handle_function1), NULL);
		g_signal_connect(g_pSkeleton, "handle-function2", G_CALLBACK(on_handle_function2), NULL);

		/** Fourth step: Export interface skeleton. */
		GError *error = NULL;
		g_dbus_interface_skeleton_export(G_DBUS_INTERFACE_SKELETON(g_pSkeleton), connection, OBJECT_PATH, &error);
		if ( error ) {
			g_print("server,onGBusAcquiredCallback,g_dbus_interface_skeleton_export,error:%s\n", error->message);
			g_error_free(error);
			g_main_loop_quit(g_pMainLoop);
		}
	}

	void onGBusNameAcquiredCallback(GDBusConnection *connection, const gchar *name, gpointer user_data)
	{
		g_print("server,onGBusNameAcquiredCallback,name:%s\n", name);
	}

	void onGBusNameLostCallback(GDBusConnection *connection, const gchar *name, gpointer user_data)
	{
		g_print("server,onGBusNameLostCallback,name:%s\n", name);
	}
	ddServicePrivate() :m_isServer(no) {
	}
	~ddServicePrivate();
	ddVoid startup(ddpCChar pName, ddBool isServer) {
		guint busWatchId = g_bus_watch_name (G_BUS_TYPE_SESSION, BUS_NAME, G_BUS_NAME_WATCHER_FLAGS_NONE,
                  onGBusNameAppearedCallback,
                  onGBusNameVanishedCallback,
                  NULL, NULL);

		/** first step: connect to dbus */
		guint busOwnId = g_bus_own_name(G_BUS_TYPE_SESSION,
		   	BUS_NAME,
		   	G_BUS_NAME_OWNER_FLAGS_NONE,
			onGBusAcquiredCallback,
			onGBusNameAcquiredCallback,
			onGBusNameLostCallback,
			NULL, NULL);
	}
private:
	ddBool m_isServer;
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
		g_print("server,dbus-launch name:%s, value:%s\n", name, value);
		setenv(name, value, 0);
		// ----- //
		usleep(20* 1000);
	}
	dPtr()->startup(pName, isServer);
}
