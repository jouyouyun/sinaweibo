#ifndef __MAIN_H__
#define __MAIN_H__

#include <gtk/gtk.h>
#include <glib.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define IMG_PATH    "../resources/weibo_test/img/cap_pic.png"

char *weibotest_CheckToken();
void weibotest_exit();
void weibotest_SaveToken(char *access_token);
void weibotest_SaveMsg(char *msg);
void weibotest_SinaUpload();
void catch_int(int signo);
void sina_weibo_inface();

#endif
