/**
 * Copyright (c) 2011 ~ 2012 Deepin, Inc.
 *               2011 ~ 2012 snyh
 *
 * Author:      snyh <snyh@snyh.org>
 * Maintainer:  snyh <snyh@snyh.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 **/
#include <glib.h>
#include "jsextension.h"

static GHashTable* signals = NULL;

void js_post_message(const char* name, JSValueRef json)
{
    if (signals == NULL) {
        g_warning("signals %s has not init!\n", name);
        return;
    }

    JSContextRef ctx = get_global_context();
    g_return_if_fail(ctx != NULL);
    JSObjectRef cb = g_hash_table_lookup(signals, name);

    JSValueRef js_args[1];
    js_args[0] = json;

    if (cb != NULL) {
        JSObjectCallAsFunction(ctx, cb, NULL, 1, js_args, NULL);
    } else {
        g_warning("signal %s has not connected!\n", name);
    }
}

void js_post_message_simply(const char* name, const char* format, ...)
{
    JSContextRef ctx = get_global_context();
    if (ctx == NULL) {
        g_warning("send js message [%s] failed beacause js runtime hasn't be prepared.", name);
        return;
    }
    if (format == NULL) {
        js_post_message(name, JSValueMakeNull(ctx));
    } else {
        va_list args;
        va_start(args, format);
        char* json_str = g_strdup_vprintf(format, args);
        va_end(args);

        js_post_message(name, json_from_cstr(ctx, json_str));
        g_free(json_str);
    }
}

void unprotect(gpointer data)
{
    JSContextRef ctx = get_global_context();
    JSValueUnprotect(ctx, (JSValueRef)data);
}

JS_EXPORT_API
void dcore_signal_connect(const char* type, JSValueRef value, JSData* js)
{
    if (signals == NULL) {
        signals = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, unprotect);
    }
    JSObjectRef cb = JSValueToObject(js->ctx, value, js->exception);
    if (cb != NULL || !JSObjectIsFunction(js->ctx, cb)) {
        JSValueProtect(js->ctx, cb);
        g_hash_table_insert(signals, g_strdup(type), (gpointer)value);
        /*g_message("signal connect %s \n", type);*/
    } else {
        g_warning("signal_connect's second parameter must be an function object");
    }
}
