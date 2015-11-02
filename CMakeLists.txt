CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(ug-setting-homescreen-efl C)

# subdir
ADD_SUBDIRECTORY(ug-setting-homescreen-efl)
ADD_SUBDIRECTORY(po)

# install xml file
INSTALL(FILES ${CMAKE_SOURCE_DIR}/ug-homescreen-setting-efl.xml DESTINATION /usr/share/packages/)

# install icon
INSTALL(FILES ${CMAKE_SOURCE_DIR}/A01-1_icon_Menu.png DESTINATION /usr/ug/res/icons/ug-setting-homescreen-efl/)
