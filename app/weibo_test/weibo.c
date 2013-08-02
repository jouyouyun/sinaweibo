#include <string.h>
#include "weibo.h"
#include "jsextension.h"    //send signal to html

#define BUFFER  512
#define WEIBO_CONFIG    "/.config/speech_weibo.ini"

gchar *sina_access_token = NULL;
gchar *sina_msg = NULL;
GMainLoop *dbus_loop = NULL;

char *weibotest_CheckToken()
{
    FILE *fp = NULL;
    char str_token[BUFFER];
    gchar *file_path = NULL;
    const gchar *home_path = NULL;

    home_path = g_get_home_dir();
    file_path = g_strdup_printf("%s%s", home_path, WEIBO_CONFIG);
    if ( file_path == NULL ) {
        g_printerr("got file path error...\n");
        return NULL;
    }
    g_printerr("-------------file path: %s\n", file_path);
    fp = fopen(file_path, "r");
    if ( fp == NULL ) {
        g_printerr("open %s failed in check_token...\n", WEIBO_CONFIG);
        return NULL;
    }

    memset(str_token, 0, BUFFER * sizeof(char));
    fscanf(fp, "sina_token:%s", str_token);
    if ( str_token[0] == '\0' ) {
        g_printerr("got token error...\n");
        fclose(fp);
        return NULL;
    }
    fclose(fp);
    g_printerr("str token: %s\n", str_token);
    int len = strlen(str_token);
    if ( str_token[len - 1] == '\n' ) {
        str_token[len - 1] = '\0';
    }
    g_printerr("str token: %s\n", str_token);
    g_printerr("-------------str token: \n");
    sina_access_token = g_strdup_printf("%s", str_token);
    g_printerr("***********token: %s\n", sina_access_token);
    //js_post_message_simply("TokenExist", NULL);

    return sina_access_token;
}

void weibotest_exit()
{
    if ( sina_access_token != NULL ) {
        g_free(sina_access_token);
        sina_access_token = NULL;
        g_free(sina_msg);
        sina_msg = NULL;
    }

    //gtk_main_quit();
    g_main_loop_quit(dbus_loop);

    return ;
}

void weibotest_SaveToken(char *access_token)
{
    FILE *fp = NULL;
    const gchar *home_path = NULL;
    gchar *file_path = NULL;

    if ( access_token == NULL ) {
        g_printerr("argument error in SaveToken...\n");
        return ;
    }

    sina_access_token = g_strdup_printf("%s", access_token);

    home_path = g_get_home_dir();
    file_path = g_strdup_printf("%s%s", home_path, WEIBO_CONFIG);
    if ( file_path == NULL ) {
        g_printerr("got file path error...\n");
        return ;
    }
    g_printerr("-------------file path: %s\n", file_path);
    fp = fopen(file_path, "w");
    if ( fp == NULL ) {
        g_printerr("open file failed in save token...\n");
        return ;
    }
    fprintf(fp, "sina_token:%s", sina_access_token);
    fclose(fp);
    g_free(file_path);
    file_path = NULL;

    return ;
}

void weibotest_SaveMsg(char *msg)
{
    if ( msg == NULL ) {
        sina_msg = g_strdup("LinuxDeepin");
        return ;
    }

    sina_msg = g_strdup_printf("%s", msg);

    return ;
}

void catch_int(int signo)
{
    if ( dbus_loop != NULL ) {
        g_main_loop_quit(dbus_loop);
    }

    if ( signo == SIGINT ) {
        weibotest_exit();
    }
}

void weibotest_SinaUpload()
{
    gchar *curl_cmd = NULL;

    if ( sina_access_token == NULL ) {
        g_printerr("arguments error in SinaUpload...\n");
        return ;
    }
    if ( sina_msg[0] == '\0' ) {
        g_printerr("-------------msg NULL\n");
        sina_msg = g_strdup("LinuxDeepin");
    }

    g_printerr("-----------------------------------------\n");
    g_printerr("token : %s\n", sina_access_token);
    g_printerr("msg: %s\n", sina_msg);
    g_printerr("-----------------------------------------\n");

    curl_cmd = g_strdup_printf("curl -k -v -F \"pic=@%s\" -F 'access_token=%s' -F 'status=%s' \"https://upload.api.weibo.com/2/statuses/upload.json\"", IMG_PATH, sina_access_token, sina_msg);
    if ( curl_cmd == NULL ) {
        g_printerr("constructor curl cmd failed...\n");
        return ;
    }
    g_printerr("curl cmd : %s\n", curl_cmd);
    system(curl_cmd);
    g_free(curl_cmd);
    curl_cmd = NULL;
    g_free(sina_access_token);
    sina_access_token = NULL;
    g_free(sina_msg);
    sina_msg = NULL;

    js_post_message_simply("SinaUploadComplete", NULL);

    return ;
}
