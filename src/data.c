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

#include <vconf.h>
#include <app_manager.h>

#include "data.h"

static int gAppCount = 0;
static homescreen_setting_data_list_t *gDataList = NULL;

static bool _homescreen_setting_data_applist_callback(app_info_h app_info, void *user_data)
{
	char *appid = NULL;
	char *name = NULL;
	char *icon_path = NULL;

	app_info_get_app_id(app_info, &appid);
	if (!appid)
	{
		HOMESET_SECURE_DBG("Invalid data appid[%s]", appid);
		return 0;
	}

#if 0 /* default item */
	if (!strncmp(HOMESCREEN_SETTING_DEFAULT_PKGNAME, appid, strlen(HOMESCREEN_SETTING_DEFAULT_PKGNAME))
			|| !strncmp(HOMESCREEN_SETTING_MENU_PKGNAME, appid, strlen(HOMESCREEN_SETTING_MENU_PKGNAME)))
	{
		HOMESET_SECURE_DBG("Ignore default pakcage[%s]", appid);
		return false;
	}
#endif

	app_info_get_label(app_info, &name);
	if (!name)
	{
		HOMESET_SECURE_DBG("Failed to get name from appid[%s]", appid);
		return false;
	}

	app_info_get_icon(app_info, &icon_path);
	if (!icon_path)
	{
		HOMESET_SECURE_DBG("Failed to get icon from appid[%s]", appid);
	}

	gAppCount++;

	// create data list
	homescreen_setting_data_list_t *data = (homescreen_setting_data_list_t *) malloc(sizeof(homescreen_setting_data_list_t));
	if (data) {
		data->id = 0;
		data->index = gAppCount;
		data->appid = appid;
		data->name = name;
		data->next = NULL;
		data->icon = icon_path;

		HOMESET_SECURE_DBG("index[%d] appid[%s] name[%s] icon_path[%s]", data->index, data->appid, data->name, data->icon);
	}

	if (gDataList)
	{
		// attach to tail
		homescreen_setting_data_list_t *temp = gDataList;
		while (temp->next)
		{
			temp = temp->next;
		}

		temp->next = data;
	}
	else
	{
		// first data
		gDataList = data;
	}

	return true;
}

void homescreen_setting_data_set_selected_homeapp(const char *appid)
{
	int ret = -1;
	if (appid)
	{
		ret = vconf_set_str(VCONFKEY_SETAPPL_SELECTED_PACKAGE_NAME, appid);
		if (ret != 0)
		{
			HOMESET_SECURE_ERR("Failed[%d] vconf_set_str(VCONFKEY_SETAPPL_SELECTED_PACKAGE_NAME, %s)", ret, appid);
		}
	}
}

char *homescreen_setting_data_get_selected_homeapp(void)
{
	return vconf_get_str(VCONFKEY_SETAPPL_SELECTED_PACKAGE_NAME);
}

homescreen_setting_data_list_t *homescreen_setting_data_get_homeapp_list(int *count)
{
	int ret = 0;
	char *value = "http://tizen.org/category/homeapp";
	app_info_filter_h handle = NULL;

	// reset application count and list
	gAppCount = 0;
	gDataList = NULL;

	// create filter
	ret = app_info_filter_create(&handle);
	if (ret != 0)
	{
		HOMESET_DBG("Failed[%d] app_info_filter_create", ret);
		return NULL;
	}

	// set filter
	ret = app_info_filter_add_string(handle, PACKAGE_INFO_PROP_APP_CATEGORY, value);
	if (ret != 0)
	{
		HOMESET_DBG("Failed[%d] app_info_filter_add_string", ret);
		app_info_filter_destroy(handle);
		return NULL;
	}

	// get list
	ret = app_info_filter_foreach_appinfo(handle, _homescreen_setting_data_applist_callback, NULL);
	if (ret != 0)
	{
		HOMESET_DBG("Failed[%d] app_info_filter_foreach_appinfo", ret);
		app_info_filter_destroy(handle);
		return NULL;
	}

	app_info_filter_destroy(handle);

	// set application count
	*count = gAppCount;

	return gDataList;
}
