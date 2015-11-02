CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(ug-setting-homescreen-efl C)

SET(PREFIX ${CMAKE_INSTALL_PREFIX}/ug)
SET(LIBDIR "${PREFIX}/lib")
SET(RESDIR "${PREFIX}/res")
SET(DATADIR "${PREFIX}/data")
SET(LOCALEDIR "${RESDIR}/locale")
SET(IMAGEDIR "${RESDIR}/images")
SET(EDJEDIR "${RESDIR}/edje")
SET(ETCDIR "${RESDIR}/etc")

SET(VERSION 0.1.0)
SET(UGNAME "setting-homescreen-efl")

SET(SRCS
	src/home-setting-ug-main.c
	src/home-setting-ug-data.c
	src/home-setting-ug-view-home.c
	src/home-setting-ug-view-home-style.c
)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

INCLUDE(FindPkgConfig)
pkg_check_modules(pkgs REQUIRED elementary efl-extension ui-gadget-1 evas dlog vconf ail pkgmgr-info feedback)

FOREACH(flag ${pkgs_CFLAGS})
	SET(EXTRA_CXXFLAGS "${EXTRA_CXXFLAGS} ${flag}")
ENDFOREACH(flag)

SET(EXTRA_CXXFLAGS "${EXTRA_CXXFLAGS} -fvisibility=hidden -Wall -Werror")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXXFLAGS}")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS}")

ADD_DEFINITIONS("-DPREFIX=\"${CMAKE_INSTALL_PREFIX}\"")

ADD_LIBRARY(${PROJECT_NAME} SHARED ${SRCS})
SET_TARGET_PROPERTIES(${PROJECT_NAME} PROPERTIES SOVERSION 0.1.0)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${pkgs_LDFLAGS})

# install library
INSTALL(TARGETS ${PROJECT_NAME} DESTINATION /usr/ug/lib)
