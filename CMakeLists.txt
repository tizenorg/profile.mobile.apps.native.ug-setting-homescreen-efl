CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(setting-homescreen C)

# subdir
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(res/po)

CONFIGURE_FILE(setting-homescreen.manifest-3.0.in ${PKGNAME}.manifest)

INSTALL(FILES setting-homescreen.manifest.xml DESTINATION ${MANIFESTDIR} RENAME ${PKGNAME}.xml)
INSTALL(FILES shared/res/setting-homescreen.png DESTINATION ${ICONDIR})
