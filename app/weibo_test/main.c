#include "xdg_misc.h"
#include "dwebview.h"
#include "utils.h"
#include "i18n.h"
#include "weibo.h"
#include <gdk/gdkcursor.h>

GtkWidget *container = NULL;

void weibotest_MoveWindow(char *ax, char *ay, char *rx, char *ry)
{
    int x = 0, y = 0;
    int cx = 0, cy = 0;    //鼠标在桌面的位置
    int nx = 0, ny = 0; //鼠标在页面的位置

    if ( ax == NULL || ay == NULL || rx == NULL || ry == NULL ) {
        g_printerr("axy NULL\n");
        return ;
    }
    //g_printerr("\naxy: %s\t%s\n", ax, ay);
    cx = atoi(ax);
    cy = atoi(ay);
    nx = atoi(rx);
    ny = atoi(ry);

    gtk_window_get_position((GtkWindow*)container, &x, &y);
    g_printerr("position: x = %d\ty = %d\n", x, y);
    g_printerr("event: x = %d\ty = %d\n", cx, cy);
    g_printerr("nxy: x = %d\ty = %d\n", nx, ny);
    g_printerr("location: x = %d\ty = %d\n", x + cx -nx, y + cy -ny);
    gtk_window_move((GtkWindow*)container, x + cx -nx, y + cy -ny);
}

int main(int argc, char **argv)
{
    init_i18n();
    gtk_init(&argc, &argv);

    sina_access_token = NULL;
    sina_msg = NULL;

    
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
    gtk_main();

    return 0;
}