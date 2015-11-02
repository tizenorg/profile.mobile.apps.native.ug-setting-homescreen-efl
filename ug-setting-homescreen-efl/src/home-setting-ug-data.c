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
#include <ail.h>
#include <pkgmgr-info.h>

#include "home-setting-ug-data.h"

static int gAppCount = 0;
static homescreen_setting_data_list_t *gDataList = NULL;

static int _homescreen_setting_data_applist_callback(const pkgmgrinfo_appinfo_h handle, void *user_data)
{
	int ret = PMINFO_R_ERROR;
	char *appid = NULL;
	char *name = NULL;
	char *icon_path = NULL;

	// get application id
	ret = pkgmgrinfo_appinfo_get_appid(handle, &appid);
	if (ret != PMINFO_R_OK)
	{
		HOMESET_DBG("Failed[%d] pkgmgrinfo_appinfo_get_appid", ret);
		return 0;
	}

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
		return 0;
	}
#endif

	name = homescreen_setting_data_get_name(appid);
	if (!name)
	{
		HOMESET_SECURE_DBG("Failed to get name from appid[%s]", appid);
		return 0;
	}


	icon_path = homescreen_setting_data_get_icon(appid);
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
		data->appid = strdup(appid);
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

	return 0;
}

static int _homescreen_setting_data_appcount_callback(const pkgmgrinfo_appinfo_h handle, void *user_data)
{
	int ret = PMINFO_R_ERROR;
	char *appid = NULL;

	// get application id
	ret = pkgmgrinfo_appinfo_get_appid(handle, &appid);
	if (ret != PMINFO_R_OK)
	{
		HOMESET_DBG("Failed[%d] pkgmgrinfo_appinfo_get_appid", ret);
		return 0;
	}

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
		return 0;
	}
#endif
	gAppCount++;
	HOMESET_SECURE_DBG("3rd home app[%s] is installed", appid);

	// stop callback
	return -1;
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

char *homescreen_setting_data_get_name(const char *appid)
{
	int ret = PMINFO_R_ERROR;
	char *name = NULL;
	char *ret_name = NULL;
	pkgmgrinfo_appinfo_h handle;

	if (!appid)
	{
		HOMESET_ERR("invalid appid");
		return NULL;
	}

	ret = pkgmgrinfo_appinfo_get_appinfo(appid, &handle);
	if (ret != PMINFO_R_OK)
	{
		HOMESET_SECURE_ERR("Failed[%d] ail_get_appinfo(%s)", ret, appid);
		return NULL;
	}

	ret = pkgmgrinfo_appinfo_get_label(handle, &name);
	if (ret != PMINFO_R_OK)
	{
		HOMESET_SECURE_ERR("Failed ail_appinfo_get_str(%s) : %d", appid, ret);
		pkgmgrinfo_appinfo_destroy_appinfo(handle);
		return NULL;
	}

	if (!name)
	{
		pkgmgrinfo_appinfo_destroy_appinfo(handle);
		return NULL;
	}

	ret_name = strdup(name);
	pkgmgrinfo_appinfo_destroy_appinfo(handle);

	return ret_name;
}

char *homescreen_setting_data_get_icon(const char *appid)
{
	int ret = PMINFO_R_ERROR;
	char *icon = NULL;
	char *ret_name = NULL;
	pkgmgrinfo_appinfo_h handle;

	if (!appid)
	{
		HOMESET_ERR("invalid appid");
		return NULL;
	}

	ret = pkgmgrinfo_appinfo_get_appinfo(appid, &handle);
	if (ret != PMINFO_R_OK)
	{
		HOMESET_SECURE_ERR("Failed[%d] pkgmgrinfo_appinfo_get_appinfo(%s) : %d", ret, appid);
		return NULL;
	}

	ret = pkgmgrinfo_appinfo_get_icon(handle, &icon);
	if (ret != PMINFO_R_OK)
	{
		HOMESET_SECURE_ERR("Failed pkgmgrinfo_appinfo_get_icon(%s) : %d", appid, ret);
		pkgmgrinfo_appinfo_destroy_appinfo(handle);
		return NULL;
	}

	if (!icon)
	{
		pkgmgrinfo_appinfo_destroy_appinfo(handle);
		return NULL;
	}

	ret_name = strdup(icon);
	pkgmgrinfo_appinfo_destroy_appinfo(handle);

	return ret_name;
}
homescreen_setting_data_list_t *homescreen_setting_data_get_homeapp_list(int *count)
{
	int ret = PMINFO_R_ERROR;
	char *value = "http://tizen.org/category/homeapp";
	pkgmgrinfo_appinfo_filter_h handle;

	// reset application count and list
	gAppCount = 0;
	gDataList = NULL;

	// create filter
	ret = pkgmgrinfo_appinfo_filter_create(&handle);
	if (ret != PMINFO_R_OK)
	{
		HOMESET_DBG("Failed[%d] pkgmgrinfo_appinfo_filter_create", ret);
		return NULL;
	}

	// set filter
	ret = pkgmgrinfo_appinfo_filter_add_string(handle, PMINFO_APPINFO_PROP_APP_CATEGORY, value);
	if (ret != PMINFO_R_OK)
	{
		HOMESET_DBG("Failed[%d] pkgmgrinfo_appinfo_filter_add_string", ret);
		pkgmgrinfo_appinfo_filter_destroy(handle);
		return NULL;
	}

	// get list
	ret = pkgmgrinfo_appinfo_filter_foreach_appinfo(handle, _homescreen_setting_data_applist_callback, NULL);
	if (ret != PMINFO_R_OK)
	{
		HOMESET_DBG("Failed[%d] pkgmgrinfo_appinfo_filter_foreach_appinfo", ret);
		pkgmgrinfo_appinfo_filter_destroy(handle);
		return NULL;
	}

	pkgmgrinfo_appinfo_filter_destroy(handle);

	// set application count
	*count = gAppCount;

	return gDataList;
}

int homescreen_setting_data_get_3rd_homeapp_installed()
{
	int ret = PMINFO_R_ERROR;
	char *value = "http://tizen.org/category/homeapp";
	pkgmgrinfo_appinfo_filter_h handle;

	// reset application count and list
	gAppCount = 0;
	gDataList = NULL;

	// create filter
	ret = pkgmgrinfo_appinfo_filter_create(&handle);
	if (ret != PMINFO_R_OK)
	{
		HOMESET_DBG("Failed[%d] pkgmgrinfo_appinfo_filter_create", ret);
		return 0;
	}

	// set filter
	ret = pkgmgrinfo_appinfo_filter_add_string(handle, PMINFO_APPINFO_PROP_APP_CATEGORY, value);
	if (ret != PMINFO_R_OK)
	{
		HOMESET_DBG("Failed[%d] pkgmgrinfo_appinfo_filter_add_string", ret);
		pkgmgrinfo_appinfo_filter_destroy(handle);
		return 0;
	}

	// get list
	ret = pkgmgrinfo_appinfo_filter_foreach_appinfo(handle, _homescreen_setting_data_appcount_callback, NULL);
	if (ret != PMINFO_R_OK)
	{
		HOMESET_DBG("Failed[%d] pkgmgrinfo_appinfo_filter_foreach_appinfo", ret);
		pkgmgrinfo_appinfo_filter_destroy(handle);
		return 0;
	}

	pkgmgrinfo_appinfo_filter_destroy(handle);

	return gAppCount;
}
