#include "weibo.h"
#include <gio/gio.h>
#include <dbus/dbus-glib.h>

#define sys_err(args ...)   { fprintf(stderr, args); }
#define sys_says(args ...)   { fprintf(stdout, args); }

#define DBUS_WEIBO_NAME "com.deepin.sina_weibo"
#define DBUS_WEIBO_PATH "/com/deepin/sina_weibo"
#define DBUS_WEIBO_INFACE "com.deepin.sina_weibo"

//internal functions
static gboolean _retry_registration (gpointer user_data);
static void _on_bus_acquired (GDBusConnection * connection, const gchar * name, gpointer user_data);
static void _on_name_acquired (GDBusConnection * connection, const gchar * name, gpointer user_data);
static void _on_name_lost (GDBusConnection * connection, const gchar * name, gpointer user_data);
static void _bus_method_call (GDBusConnection * connection, const gchar * sender,
                             const gchar * object_path, const gchar * interface,
                             const gchar * method, GVariant * params,
                             GDBusMethodInvocation * invocation, gpointer user_data);
static gboolean do_exit(gpointer user_data);

//extern GMainLoop *dbus_loop;
static guint lock_service_owner_id;
static guint lock_service_reg_id;        //used for unregister an object path
static guint retry_reg_timeout_id;   //timer used for retrying dbus name registration.
GDBusConnection* lock_connection;

static GDBusNodeInfo *      node_info = NULL;
static GDBusInterfaceInfo *interface_info = NULL;
static GDBusInterfaceVTable interface_table = {
    method_call:   _bus_method_call,
    get_property:   NULL, // No properties 
    set_property:   NULL  // No properties 
};

const char *_lock_dbus_iface_xml = 
"<?xml version=\"1.0\"?>\n"
"<node>\n"
"	<interface name=\""DBUS_WEIBO_INFACE"\">\n"
"		<method name=\"weibotest_SinaUpload\">\n"
"		</method>"
"		<method name=\"hello\">\n"
"		</method>"
"	</interface>\n"
"</node>\n"
;

void hello()
{
	sys_says("Hello World!\n");

	return;
}

void sina_weibo_inface()
{
	GError *error = NULL;
	
	node_info = g_dbus_node_info_new_for_xml (_lock_dbus_iface_xml, &error);
	if ( error != NULL ) {
		g_critical("Unable to parse interface xml: %s\n", error->message);
		g_error_free(error);
	}

	interface_info = g_dbus_node_info_lookup_interface(node_info, 
			DBUS_WEIBO_INFACE );
	if ( interface_info == NULL ) {
		g_critical("Unable to find interface '"DBUS_WEIBO_INFACE"'");
	}

	lock_service_owner_id = 0;
	lock_service_reg_id = 0;
	retry_reg_timeout_id = 0;
	_retry_registration(NULL);

	//g_timeout_add_seconds( 15, do_exit, NULL );
    /*dbus_loop = g_main_loop_new(NULL, FALSE);
    g_main_loop_run(dbus_loop);
    g_main_loop_unref(dbus_loop);*/

	return ;
}

static gboolean
_retry_registration ( gpointer user_data )
{
	lock_service_owner_id = g_bus_own_name( G_BUS_TYPE_SESSION, 
				DBUS_WEIBO_NAME, 
				G_BUS_NAME_OWNER_FLAGS_NONE, 
				lock_service_reg_id ? NULL : _on_bus_acquired,
				_on_name_acquired,
				_on_name_lost, 
				NULL, 
				NULL);
	return 0;
}

static void
_on_bus_acquired ( GDBusConnection *connection, 
		const gchar *name, 
		gpointer user_data )
{
	g_debug(" on bus acquired ...\n");

	lock_connection = connection;

	//register object
	GError *error = NULL;
	lock_service_reg_id = g_dbus_connection_register_object( connection, 
			DBUS_WEIBO_PATH, 
			interface_info, 
			&interface_table, 
			user_data, 
			NULL, 
			&error );

	if ( error != NULL ) {
		g_critical ( "Unable to register object to the dbus: %s\n", 
				error->message );
		g_error_free(error);
		g_bus_unown_name(lock_service_owner_id);
		lock_service_owner_id = 0;
		retry_reg_timeout_id = g_timeout_add_seconds(1, 
				_retry_registration, NULL );
		return;
	}

	return;
}

static void
_on_name_acquired ( GDBusConnection *connection, 
		const gchar* name, 
		gpointer user_data )
{
	g_debug ( "Dbus name acquired ... \n" );
}

static void
_on_name_lost ( GDBusConnection *connection, 
		const gchar *name, 
		gpointer user_data )
{
	if ( connection == NULL ) {
		g_critical ( "Unable to get a connection to DBus...\n" );
	} else {
		g_critical ( "Unable to claim the name %s\n", DBUS_WEIBO_NAME );
	}

	lock_service_owner_id = 0;
}

static void
_bus_method_call (GDBusConnection * connection,
                 const gchar * sender, const gchar * object_path, const gchar * interface,
                 const gchar * method, GVariant * params,
                 GDBusMethodInvocation * invocation, gpointer user_data)
{
    g_debug ("bus_method_call");

    GVariant * retval = NULL;
    GError * error = NULL;

    if (g_strcmp0 (method, "weibotest_SinaUpload") == 0) {
        weibotest_SinaUpload();
    } else if (g_strcmp0 (method, "hello") == 0) {
        hello();
    } else {
        g_warning ("Calling method '%s' on lock and it's unknown", method);
    }

    if (error != NULL) {
        g_dbus_method_invocation_return_dbus_error (invocation,
                "com.deepin.dde.lock.Error",
                error->message);
        g_error_free (error);

    } else if ( retval != NULL ){
        g_dbus_method_invocation_return_value (invocation, retval);
    }
}
