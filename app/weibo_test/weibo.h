#ifndef __MAIN_H__
#define __MAIN_H__

#include <gtk/gtk.h>
#include <glib.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define IMG_PATH    "../resources/weibo_test/img/cap_pic.png"

gchar *sina_access_token;
gchar *sina_msg;

void weibotest_exit();
void weibotest_SaveToken(char *access_token);
void weibotest_SaveMsg(char *msg);
int weibotest_SinaUpload();
void catch_int(int signo);
void weibotest_MoveWindow(char *ax, char *ay);

#endif
