/*
 * Copyright (c) 2000 - 2015 Samsung Electronics Co., Ltd. All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */


#ifndef __HOME_SETTING_UG_H__
#define __HOME_SETTING_UG_H__

#ifdef  LOG_TAG
#undef  LOG_TAG
#endif
#define LOG_TAG "ug-setting-homescreen"

#include <dlog.h>

#define HOMESET_ERR(fmt, arg...)  LOGE(" "fmt, ##arg)
#define HOMESET_WARN(fmt, arg...) LOGW(" "fmt, ##arg)
#define HOMESET_DBG(fmt, arg...)  LOGD(" "fmt, ##arg)

#define HOMESET_SECURE_ERR(fmt, arg...)  SECURE_LOGE(" "fmt, ##arg)
#define HOMESET_SECURE_WARN(fmt, arg...) SECURE_LOGW(" "fmt, ##arg)
#define HOMESET_SECURE_DBG(fmt, arg...)  SECURE_LOGD(" "fmt, ##arg)

#include <Elementary.h>
#include <libintl.h>
#include <ui-gadget-module.h>

#define HOMESET_DOMAIN "ug-setting-homescreen-efl"
#define HOMESET_TEXT(str) dgettext(HOMESET_DOMAIN, str)
#define _NOT_LOCALIZED(str) (str)

// launch mode
enum
{
	LAUNCH_MODE_NORMAL = 0,
};

/* User created ug data */
struct ug_data
{
	Evas_Object *base;
	ui_gadget_h ug;

	int mode;

	Evas_Object *naviframe;
	Elm_Object_Item *navi_item;

	// for main view
	Evas_Object *list_main;

	Evas_Object *back_button;
};

#endif /* __HOME_SETTING_UG_H__ */
