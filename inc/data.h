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


#ifndef __HOME_SETTING_UG_DATA_H__
#define __HOME_SETTING_UG_DATA_H__

#include "home-setting.h"

#define HOMESCREEN_SETTING_DEFAULT_PKGNAME	"org.tizen.homescreen"
#define HOMESCREEN_SETTING_MENU_PKGNAME	"org.tizen.menu-screen"

typedef struct _homescreen_setting_data_list homescreen_setting_data_list_t;
struct _homescreen_setting_data_list
{
	homescreen_setting_data_list_t *next;
	int id;
	int index;
	char *appid;
	char *name;
	char *icon;
};

void homescreen_setting_data_set_selected_homeapp(const char *appid);
char *homescreen_setting_data_get_selected_homeapp(void);

homescreen_setting_data_list_t *homescreen_setting_data_get_homeapp_list(int *count);

#endif /* __HOME_SETTING_UG_DATA_H__ */
