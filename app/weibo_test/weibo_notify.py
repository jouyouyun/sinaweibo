#! /usr/bin/env python
# -*- coding: utf-8 -*-

from dtk.ui.dbus_notify import DbusNotify

ntf = DbusNotify("weibo-test")
ntf.set_summary("weibo send")
ntf.set_body("Sina Weibo Send Over")
ntf.notify()
