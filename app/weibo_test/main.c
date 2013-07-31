#include "xdg_misc.h"
#include "dwebview.h"
#include "utils.h"
#include "i18n.h"
#include "weibo.h"
#include <gdk/gdkcursor.h>

int nx = 0;
int ny = 0;
gboolean drag = FALSE;
GtkWidget *container = NULL;

void weibotest_MoveWindow(char *ax, char *ay)
{
    int cx = 0;
    int cy = 0;
    int x = 0, y = 0;

    if ( ax == NULL || ay == NULL ) {
        g_printerr("axy NULL\n");
        return ;
    }
    //g_printerr("\naxy: %s\t%s\n", ax, ay);
    cx = atoi(ax);
    cy = atoi(ay);

    gtk_window_get_position((GtkWindow*)container, &x, &y);
    g_printerr("global nxy: x = %d\ty = %d\n", nx, ny);
    g_printerr("position: x = %d\ty = %d\n", x, y);
    g_printerr("event: x = %d\ty = %d\n", cx, cy);
    g_printerr("location: x = %d\ty = %d\n", x + cx -nx, y + cy -ny);
    gtk_window_move((GtkWindow*)container, x + cx -nx, y + cy -ny);
}
/*
static gint button_press_event(GtkWidget *widget, 
        GdkEventButton *event, gpointer data)
{
    if ( event->button == 1 ) {
        g_printerr("Cursor Press...\n");
        drag = TRUE;
        nx = event->x;
        ny = event->y;
    }

    return TRUE;
}

static gint button_release_event(GtkWidget *widget, 
        GdkEventButton *event, gpointer data)
{
    if ( event->button == 1 ) {
        g_printerr("Cursor Release...\n");
        drag = FALSE;
    }

    return TRUE;
}

static gint motion_notify_event(GtkWidget *widget, 
        GdkEventButton *event, gpointer data)
{
    if ( drag ) {
        g_printerr("Move Window...\n");
        int x, y;
        GtkWidget *window = (GtkWidget*)data;

        gtk_window_get_position((GtkWindow*)window, &x, &y);
        gtk_window_move((GtkWindow*)window, x + event->x - nx, 
                y + event->y - ny);
    }

    return TRUE;
}
*/
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
    /*gtk_widget_set_events(container, 
        GDK_EXPOSURE_MASK | GDK_LEAVE_NOTIFY_MASK |
        GDK_BUTTON_PRESS_MASK | GDK_BUTTON_RELEASE_MASK | 
        GDK_POINTER_MOTION_MASK | GDK_POINTER_MOTION_HINT_MASK );
    g_signal_connect(G_OBJECT(container), "button_press_event", 
            G_CALLBACK(button_press_event), container);
    g_signal_connect(G_OBJECT(container), "button_release_event", 
            G_CALLBACK(button_release_event), container);
    g_signal_connect(G_OBJECT(container), "motion_notify_event", 
            G_CALLBACK(motion_notify_event), container);*/
    //g_signal_connect(container, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    monitor_resource_file("weibo_test", webview);

    gtk_window_set_position(GTK_WINDOW(container), GTK_WIN_POS_CENTER);
    //去除窗口修饰
    gtk_window_set_decorated(GTK_WINDOW (container), FALSE);
    gtk_widget_show_all(container);
    gtk_main();

    return 0;
}