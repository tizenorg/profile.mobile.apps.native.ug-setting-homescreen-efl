CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
PROJECT(setting-homescreen C)

SET(PREFIX ${CMAKE_INSTALL_PREFIX})
SET(LIBDIR "${PREFIX}/lib")

SET(VERSION 0.1.0)

SET(SRCS
	data.c
	home.c
	home-style.c
	main.c
)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/inc/)

INCLUDE(FindPkgConfig)
pkg_check_modules(pkgs REQUIRED 
	capi-appfw-application
	capi-appfw-app-manager
	capi-system-info
	elementary 
	efl-extension 
	evas 
	dlog 
	vconf 
	pkgmgr-info 
	feedback 
	libtzplatform-config 
	capi-system-system-settings)

FOREACH(flag ${pkgs_CFLAGS})
	SET(EXTRA_CXXFLAGS "${EXTRA_CXXFLAGS} ${flag}")
ENDFOREACH(flag)

SET(EXTRA_CXXFLAGS "${EXTRA_CXXFLAGS} -fvisibility=hidden -Wall -Werror")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_CXXFLAGS}")
SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS}")

#Set executable
ADD_EXECUTABLE(${PROJECT_NAME} ${SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${pkgs_LDFLAGS})

# install library
INSTALL(TARGETS ${PROJECT_NAME} DESTINATION ${BINDIR})
