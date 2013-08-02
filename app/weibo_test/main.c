#include "xdg_misc.h"
#include "dwebview.h"
#include "utils.h"
#include "i18n.h"
#include "weibo.h"
#include <gdk/gdkcursor.h>

extern gchar *sina_access_token;
extern gchar *sina_msg;
GtkWidget *container = NULL;

int main(int argc, char **argv)
{
    init_i18n();
    gtk_init(&argc, &argv);
    
    container = create_web_container(TRUE, TRUE);
    GtkWidget *webview = d_webview_new_with_uri(GET_HTML_PATH("weibo_test"));

    signal(SIGINT, catch_int);
    set_default_theme("Deepin");
    set_desktop_env_name("Deepin");

    //Capture Picture
    system("./capture-picture");

    gtk_container_add(GTK_CONTAINER(container), GTK_WIDGET(webview));

    g_signal_connect(webview, "draw", G_CALLBACK(erase_background), NULL);
    //g_signal_connect(container, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    monitor_resource_file("weibo_test", webview);

    gtk_window_set_position(GTK_WINDOW(container), GTK_WIN_POS_CENTER);
    //去除窗口修饰
    gtk_window_set_decorated(GTK_WINDOW (container), FALSE);
    gtk_widget_show_all(container);
    sina_weibo_inface();
    //gtk_main();

    return 0;
}
