#include "xdg_misc.h"
#include "dwebview.h"
#include "utils.h"
#include "i18n.h"
#include <gtk/gtk.h>
#include <glib.h>

#define IMG_PATH    "../resources/weibo_test/img/cap_pic.png"
gchar *sina_access_token = NULL;

void weibotest_exit()
{
    if ( sina_access_token != NULL ) {
        g_free(sina_access_token);
        sina_access_token = NULL;
    }

    gtk_main_quit();
}

void weibotest_SaveToken(char *access_token)
{
    if ( access_token == NULL ) {
        g_printerr("argument error in SaveToken...\n");
        return ;
    }

    sina_access_token = g_strdup_printf("%s", access_token);
}

int weibotest_SinaUpload(char *msg)
{
    gchar *curl_cmd = NULL;
    /*
    if ( access_token == NULL || img_path == NULL ) {
        g_printerr("arguments error in SinaUpload...\n");
        return ;
    }
    */

    curl_cmd = g_strdup_printf("curl -k -v -F \"pic=@%s\" -F 'access_token=%s' -F 'status=%s' \"https://upload.api.weibo.com/2/statuses/upload.json\"", IMG_PATH, sina_access_token, msg);
    if ( curl_cmd == NULL ) {
        g_printerr("constructor curl cmd failed...\n");
        return -1;
    }
    g_printerr("curl cmd : %s\n", curl_cmd);
    system(curl_cmd);
    g_free(curl_cmd);
    curl_cmd = NULL;

    return 0;
}

int main(int argc, char **argv)
{
    init_i18n();
    gtk_init(&argc, &argv);

    GtkWidget *container = create_web_container(TRUE, TRUE);
    GtkWidget *webview = d_webview_new_with_uri(GET_HTML_PATH("weibo_test"));

    set_default_theme("Deepin");
    set_desktop_env_name("Deepin");

    //Capture Picture
    system("./capture-picture");

    gtk_container_add(GTK_CONTAINER(container), GTK_WIDGET(webview));

    //g_signal_connect(container, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    monitor_resource_file("weibo_test", webview);

    gtk_window_set_position(GTK_WINDOW(container), GTK_WIN_POS_CENTER);
    //去除窗口修饰
    gtk_window_set_decorated(GTK_WINDOW (container), FALSE);
    gtk_widget_show_all(container);
    gtk_main();

    return 0;
}
