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


#ifndef __HOME_SETTING_UG_VIEW_HOME_H__
#define __HOME_SETTING_UG_VIEW_HOME_H__

#include "home-setting.h"

/**
 * @brief Creates main application's view.
 * @param[in] ugd application's data structure.
 */
void homescreen_setting_main_create_view(struct ug_data *ugd);

/**
 * @brief Updates GUI when VCONFKEY_SETAPPL_SELECTED_PACKAGE_NAME vconf-key value has been changed.
 */
void homescreen_setting_main_update_view();

/**
 * @brief Updates GUI when system language has been changed.
 */
void homescreen_setting_main_language_changed();

#endif /* __HOME_SETTING_UG_VIEW_HOME_H__ */
