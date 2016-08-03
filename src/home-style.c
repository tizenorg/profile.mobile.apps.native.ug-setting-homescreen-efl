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

#include "data.h"
#include "home-style.h"

#define APP_ICON_MIN_SIZE ELM_SCALE_SIZE(80)

enum
{
	INDEX_DEFAULT = 0,
	INDEX_DOWNLOADED,
	INDEX_DOWNLOADED_LAST,
};

static Elm_Genlist_Item_Class itc;
static Evas_Object *radio_group = NULL;

static char *_homescreen_setting_type_gl_text_get(void *data, Evas_Object *obj, const char *part)
{
	homescreen_setting_data_list_t *data_list = (homescreen_setting_data_list_t *) data;

	if (data_list == NULL)
	{
		HOMESET_ERR("invalid data");
		return NULL;
	}

	if (!strcmp(part, "elm.text"))
	{
		if (data_list->name != NULL)
		{
			return strdup(data_list->name);
		}
	}

	return NULL;
}

static Evas_Object *_homescreen_setting_type_gl_content_get(void *data, Evas_Object *obj, const char *part)
{
	homescreen_setting_data_list_t *data_list = (homescreen_setting_data_list_t *) data;

	if (data_list == NULL)
	{
		HOMESET_ERR("invalid data");
		return NULL;
	}

	if (!strcmp(part, "elm.swallow.end"))
	{
		char *homeapp = homescreen_setting_data_get_selected_homeapp();
		Evas_Object *radio = elm_radio_add(obj);
		elm_radio_state_value_set(radio, data_list->id);
		elm_radio_group_add(radio, radio_group);
		if (homeapp && data_list->appid)
		{
			if (!strcmp(homeapp, data_list->appid))
			{
				elm_radio_value_set(radio_group, data_list->id);
			}
		}
		if (homeapp)
		{
			free(homeapp);
		}
		return radio;
	}
	else if (!strcmp(part, "elm.swallow.icon") && (data_list->icon != NULL))
	{
		Evas_Object *icon = elm_image_add(obj);
		elm_image_file_set(icon, data_list->icon, NULL);
		evas_object_size_hint_weight_set(icon, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
		evas_object_size_hint_min_set(icon, APP_ICON_MIN_SIZE, APP_ICON_MIN_SIZE);
		return icon;
	}
	return NULL;
}

static void _homescreen_setting_type_gl_del(void *data, Evas_Object *obj)
{
	homescreen_setting_data_list_t *data_list = (homescreen_setting_data_list_t *) data;

	if (data_list)
	{
		if (data_list->appid)
		{
			free(data_list->appid);
		}
		if (data_list->name)
		{
			free(data_list->name);
		}
		if (data_list->icon)
		{
			free(data_list->icon);
		}
		free(data_list);
	}
}

static void _homescreen_setting_type_gl_sel(void *data, Evas_Object *obj, void *event_info)
{

	Elm_Object_Item *item = (Elm_Object_Item *) event_info;
	if (item == NULL)
	{
		HOMESET_ERR("invalid item data");
		return;
	}

	homescreen_setting_data_list_t *data_list = (homescreen_setting_data_list_t *) elm_object_item_data_get(item);

	if (data_list == NULL)
	{
		HOMESET_ERR("invalid type data");
		return;
	}

	elm_radio_value_set(radio_group, data_list->id);
	elm_genlist_item_selected_set(item, EINA_FALSE);

	struct ug_data *ugd = (struct ug_data *) data;
	if (ugd == NULL)
	{
		HOMESET_ERR("invalid ug data");
		return;
	}

	char *homeapp = homescreen_setting_data_get_selected_homeapp();
	if (homeapp && data_list->appid)
	{
		if (!strcmp(homeapp, data_list->appid))
		{
			HOMESET_DBG("Already set homeapp[%s]", homeapp);
		}
		else
		{
			HOMESET_DBG("Change homeapp[%s]=>[%s]", homeapp, data_list->appid);
			homescreen_setting_data_set_selected_homeapp(data_list->appid);
			elm_naviframe_item_pop(ugd->naviframe);
		}
	}

	if (homeapp)
	{
		free(homeapp);
	}
}

static void _homescreen_setting_create_gl_item(Elm_Gen_Item_Class *item)
{
	item->item_style = "type1";
	item->func.text_get = _homescreen_setting_type_gl_text_get;
	item->func.content_get = _homescreen_setting_type_gl_content_get;
	item->func.state_get = NULL;
	item->func.del = _homescreen_setting_type_gl_del;
}

Evas_Object* _homescreen_setting_type_add_list(struct ug_data *ugd)
{
	int count = 0, i = 0, id = 0;

	Evas_Object *gl = elm_genlist_add(ugd->naviframe);
	elm_genlist_mode_set(gl, ELM_LIST_COMPRESS);
	_homescreen_setting_create_gl_item(&itc);

	homescreen_setting_data_list_t *data_list = homescreen_setting_data_get_homeapp_list(&count);
	char *homeapp = homescreen_setting_data_get_selected_homeapp();

	for (i = 0; i < count; i++)
	{
		if (data_list)
		{
			data_list->id = id++;
			if (i == count - 1)
			{
				data_list->index = INDEX_DOWNLOADED_LAST;
			}
			else
			{
				data_list->index = INDEX_DOWNLOADED;
			}

			if(radio_group == NULL)
			{
				radio_group = elm_radio_add(gl);
				elm_radio_state_value_set(radio_group, 0);
			}

			Elm_Object_Item *gl_item = elm_genlist_item_append(gl,
										&itc,
										data_list, NULL,
										ELM_GENLIST_ITEM_NONE,
										_homescreen_setting_type_gl_sel,
										ugd);
			if (!gl_item)
			{
				HOMESET_ERR("Cannot allocate gl_item.");
				return NULL;
			}
			elm_object_item_data_set(gl_item, data_list);
			data_list = data_list->next;
		}
	}

	if (homeapp)
	{
		free(homeapp);
	}

	return gl;
}
