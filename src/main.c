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
#include <efl_extension.h>
#include <app.h>
#include <app_control.h>
#include <system_settings.h>
#include "home-setting.h"
#include "home.h"

static void _package_changed_cb(keynode_t * node, void *data)
{
	HOMESET_DBG("package changed");
	homescreen_setting_main_update_view();
}
static Evas_Object *_create_naviframe(Evas_Object *parent)
{
	Evas_Object *naviframe = NULL;

	if (parent == NULL) {
		HOMESET_ERR("Parent is null.");
		return NULL;
	}

	naviframe = elm_naviframe_add(parent);
	if (naviframe == NULL) {
		HOMESET_ERR("Cannot add naviframe.");
		return NULL;
	}

	elm_object_part_content_set(parent, "elm.swallow.content", naviframe);

	elm_naviframe_prev_btn_auto_pushed_set(naviframe, EINA_FALSE);
	eext_object_event_callback_add(naviframe, EEXT_CALLBACK_BACK, eext_naviframe_back_cb, NULL);
	evas_object_show(naviframe);

	return naviframe;
}

static Evas_Object *_create_background(Evas_Object *parent)
{
	Evas_Object *bg = NULL;

	bg = elm_bg_add(parent);

	evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
	elm_object_style_set(bg, "group_list");

	elm_object_part_content_set(parent, "elm.swallow.bg", bg);
	evas_object_show(bg);

	return bg;
}

static Evas_Object *_create_layout(Evas_Object *parent)
{
	Evas_Object *layout = NULL;

	if (parent == NULL) {
		HOMESET_ERR("Parent is null.");
		return NULL;
	}

	layout = elm_layout_add(parent);
	if (layout == NULL) {
		HOMESET_ERR("Cannot add layout.");
		return NULL;
	}

	evas_object_size_hint_weight_set(layout, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);

	evas_object_show(layout);

	return layout;
}

static Evas_Object *_create_win()
{
	/* Window: */
    Evas_Object *win = elm_win_add(NULL, "", ELM_WIN_BASIC);
    elm_win_conformant_set(win, EINA_TRUE);
    elm_win_autodel_set(win, EINA_TRUE);
    elm_win_indicator_mode_set(win, ELM_WIN_INDICATOR_SHOW);
    elm_win_indicator_opacity_set(win, ELM_WIN_INDICATOR_OPAQUE);
    evas_object_show(win);

    /*  Conform: */
    Evas_Object *conform = elm_conformant_add(win);
    evas_object_size_hint_weight_set(conform, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_win_resize_object_add(win, conform);
    evas_object_show(conform);

    /*  Bg: */
    Evas_Object *bg = elm_bg_add(conform);
    evas_object_size_hint_weight_set(bg, EVAS_HINT_EXPAND, EVAS_HINT_EXPAND);
    elm_object_part_content_set(conform, "elm.swallow.bg", bg);
    evas_object_show(bg);

    return conform;
}

static Evas_Object *create_fullview(Evas_Object *parent, struct ug_data *ugd)
{
	HOMESET_DBG("");
	if (ugd == NULL) {
		HOMESET_ERR("Invalid ug data");
		return NULL;
	}

	/* Create Full view */
	ugd->base = _create_layout(parent);

	elm_layout_theme_set(ugd->base, "layout", "application", "default");
	elm_win_indicator_mode_set(parent, ELM_WIN_INDICATOR_SHOW);

	/* Create background */
	_create_background(ugd->base);

	/* Create navigation bar */
	ugd->naviframe = _create_naviframe(ugd->base);

	/* Create main view */
	homescreen_setting_main_create_view(ugd);

	return ugd->base;
}

static bool on_create(void *priv)
{
	HOMESET_DBG("");
	struct ug_data *ugd = priv;

	char *resPath = app_get_resource_path();
	if(resPath)
	{
		const char *localePath = "locale";
		char path[PATH_MAX] = { 0, };
		snprintf(path, sizeof(path), "%s%s", resPath, localePath);
		bindtextdomain(HOMESET_DOMAIN, path);
		free(resPath);
	}

	elm_app_base_scale_set(2.6);

	// notify event
	if (vconf_notify_key_changed(VCONFKEY_SETAPPL_SELECTED_PACKAGE_NAME, _package_changed_cb, NULL) != 0)
	{
		HOMESET_ERR("Failed to notify package changed event");
		return false;
	}

	Evas_Object *win = _create_win();
	ugd->base = create_fullview(win, ugd);
	elm_object_content_set(win, ugd->base);

	return true;
}

static void on_destroy(void *priv)
{
	HOMESET_DBG("exit");
	struct ug_data *ugd = priv;;

	if (ugd)
	{
		// ignore event
		if (vconf_ignore_key_changed(VCONFKEY_SETAPPL_SELECTED_PACKAGE_NAME, _package_changed_cb) != 0)
		{
			HOMESET_ERR("Failed to ignore package changed event");
		}
	}

	free(ugd);
}

static void on_language(app_event_info_h event_info, void *user_data)
{
	char *language_set = NULL;
	system_settings_get_value_string(SYSTEM_SETTINGS_KEY_LOCALE_LANGUAGE, &language_set);

	if (language_set)
	{
		elm_language_set(language_set);
		free(language_set);
	}

	homescreen_setting_main_language_changed();
}

int main(int argc, char *argv[])
{
	HOMESET_DBG("app init !!!");

	struct ug_data *ugd = calloc(1, sizeof(struct ug_data));
	if (!ugd)
		return -1;

	ui_app_lifecycle_callback_s event_callback;
	memset(&event_callback, 0x00, sizeof(ui_app_lifecycle_callback_s));

	event_callback.create = on_create;
	event_callback.terminate = on_destroy;

	app_event_handler_h ev = NULL;
	ui_app_add_event_handler(&ev, APP_EVENT_LANGUAGE_CHANGED, on_language, ugd);

	return ui_app_main(argc, argv, &event_callback, ugd);
}

