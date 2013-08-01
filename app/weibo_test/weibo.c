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

void catch_int(int signo)
{
    if ( signo == SIGINT ) {
        weibotest_exit();
    }
}
