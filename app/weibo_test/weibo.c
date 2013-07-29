#include "weibo.h"
#include <stdlib.h>

void weibotest_exit()
{
    if ( sina_access_token != NULL ) {
        g_free(sina_access_token);
        sina_access_token = NULL;
        g_free(sina_msg);
        sina_msg = NULL;
    }

    gtk_main_quit();

    return ;
}

void weibotest_SaveToken(char *access_token)
{
    if ( access_token == NULL ) {
        g_printerr("argument error in SaveToken...\n");
        return ;
    }

    sina_access_token = g_strdup_printf("%s", access_token);

    return ;
}

void weibotest_SaveMsg(char *msg)
{
    if ( msg == NULL ) {
        g_printerr("arguments error in SaveMsg...\n");
        return ;
    }

    sina_msg = g_strdup_printf("%s", msg);

    return ;
}

int weibotest_SinaUpload()
{
    gchar *curl_cmd = NULL;

    if ( sina_access_token == NULL || sina_msg == NULL ) {
        g_printerr("arguments error in SinaUpload...\n");
        return -1;
    }

    curl_cmd = g_strdup_printf("curl -k -v -F \"pic=@%s\" -F 'access_token=%s' -F 'status=%s' \"https://upload.api.weibo.com/2/statuses/upload.json\"", IMG_PATH, sina_access_token, sina_msg);
    if ( curl_cmd == NULL ) {
        g_printerr("constructor curl cmd failed...\n");
        return -1;
    }
    g_printerr("curl cmd : %s\n", curl_cmd);
    system(curl_cmd);
    g_free(curl_cmd);
    curl_cmd = NULL;
    g_free(sina_access_token);
    sina_access_token = NULL;
    g_free(sina_msg);
    sina_msg = NULL;

    return 0;
}

void catch_int(int signo)
{
    if ( signo == SIGINT ) {
        weibotest_exit();
    }
}
