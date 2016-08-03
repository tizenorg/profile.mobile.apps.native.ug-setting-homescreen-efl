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

#define HOMESET_ERR(fmt, arg...)  dlog_print(DLOG_ERROR, LOG_TAG, " "fmt, ##arg)
#define HOMESET_WARN(fmt, arg...) dlog_print(DLOG_WARN, LOG_TAG, " "fmt, ##arg)
#define HOMESET_DBG(fmt, arg...)  dlog_print(DLOG_DEBUG, LOG_TAG, " "fmt, ##arg)

#define HOMESET_SECURE_ERR(fmt, arg...)  dlog_print(DLOG_ERROR, LOG_TAG, " "fmt, ##arg)
#define HOMESET_SECURE_WARN(fmt, arg...) dlog_print(DLOG_WARN, LOG_TAG, " "fmt, ##arg)
#define HOMESET_SECURE_DBG(fmt, arg...)  dlog_print(DLOG_DEBUG, LOG_TAG, " "fmt, ##arg)

#include <Elementary.h>

#define HOMESET_DOMAIN "setting-homescreen"
#define HOMESET_TEXT(str) dgettext(HOMESET_DOMAIN, str)

/**
 * @brief Main application's data structure.
 */
struct ug_data
{
	Evas_Object *base;          /**< Main application's layout.*/

	Evas_Object *naviframe;     /**< Naviframe.*/
	Elm_Object_Item *navi_item; /**< Naviframe-item*/

	// for main view
	Evas_Object *list_main;     /**< Genlist with homescreen-applications available to switch.*/

	Evas_Object *back_button;   /**< Back-button to exit from application.*/
};

#endif /* __HOME_SETTING_UG_H__ */
