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


#include "home-setting.h"
#include "data.h"
#include "home.h"
#include "home-style.h"

#include <app.h>

struct ug_data *g_ug_data = NULL;
static Evas_Object *g_popup = NULL;

enum
{
	MAIN_INDEX_GROUP = 0,
	MAIN_INDEX_TYPE,
};

enum
{
	MAIN_GROUP_HOMESCREEN =0,
};

static Eina_Bool _homescreen_setting_main_pop_cb(void *data, Elm_Object_Item * it)
{
	HOMESET_DBG("");
	struct ug_data *ugd = (struct ug_data *) data;

	if (ugd == NULL)
		return EINA_FALSE;

	ui_app_exit();

	return EINA_FALSE;
}

static void _homescreen_setting_back_btn_cb(void *data, Evas_Object *obj, void *event_info)
{
	HOMESET_DBG("");
	struct ug_data *ugd = (struct ug_data *) data;

	if (ugd == NULL)
		return;

	ui_app_exit();
}

void homescreen_setting_main_create_view(void *data)
{
	HOMESET_DBG("create main view");
	struct ug_data *ugd = (struct ug_data *) data;
	if (!ugd)
	{
		HOMESET_ERR("invalid ugd");
		return;
	}

	g_ug_data = ugd;
	g_popup = NULL;

	/* Create genlist */
	g_ug_data->list_main = _homescreen_setting_type_add_list(ugd);

	ugd->back_button = elm_button_add(ugd->naviframe);
	elm_object_style_set(ugd->back_button, "naviframe/back_btn/default");
	evas_object_smart_callback_add(ugd->back_button, "clicked", _homescreen_setting_back_btn_cb, ugd);

	/* Push to naviframe */
	g_ug_data->navi_item = elm_naviframe_item_push(ugd->naviframe, HOMESET_TEXT("IDS_ST_HEADER_HOME_SCREEN"), ugd->back_button, NULL, g_ug_data->list_main, NULL);
	elm_naviframe_item_pop_cb_set(g_ug_data->navi_item, _homescreen_setting_main_pop_cb, ugd);

}

void homescreen_setting_main_update_view()
{
	HOMESET_ERR("");

	if (!g_ug_data)
	{
		HOMESET_ERR("invalid ug data");
		return;
	}

	homescreen_setting_main_language_changed();
}

void homescreen_setting_main_language_changed()
{
	if (g_ug_data)
	{
		HOMESET_ERR("main language changed");
		// update title
		if (g_ug_data->navi_item)
		{
			elm_object_item_part_text_set(g_ug_data->navi_item, "elm.text.title", HOMESET_TEXT("IDS_ST_HEADER_HOME_SCREEN"));
		}
	}

}
