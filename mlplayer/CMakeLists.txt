cmake_minimum_required(VERSION 2.6)

project(MLPLAYER)
include(CMakeProjectFiles.txt)


# Config
configure_file(${PROJECT_SOURCE_DIR}/config.hpp.in ${PROJECT_BINARY_DIR}/config.hpp)


# Includes
include_directories(${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR})


# Develop mode
if(DEVELOP_BUILD)
	add_definitions(-DMLPLAYER_DEBUG_MODE=1 -DMLPLAYER_DEVELOP_MODE=1)
else()
	add_definitions(-DMLPLAYER_DEBUG_MODE=0 -DMLPLAYER_DEVELOP_MODE=0)
endif()


# Qt's tools
QT4_WRAP_CPP(MLPLAYER_MOC_CPP ${MLPLAYER_MOC_SOURCES})
QT4_WRAP_UI(MLPLAYER_UI_CPP ${MLPLAYER_UIS})


# TODO
## Desktop files
#if(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
#	configure_file(${PROJECT_SOURCE_DIR}/${APP_UNIX_NAME}.desktop.in ${PROJECT_BINARY_DIR}/${APP_UNIX_NAME}.desktop)
#	install(FILES ${PROJECT_BINARY_DIR}/${APP_UNIX_NAME}.desktop DESTINATION share/applications)
#endif()
#
#
## Mans
#if(UNIX)
#	install(FILES ${PROJECT_SOURCE_DIR}/mans/${APP_UNIX_NAME}.1.gz DESTINATION share/man/man1)
#	install(FILES ${PROJECT_SOURCE_DIR}/mans/ru/${APP_UNIX_NAME}.1.gz DESTINATION share/man/ru/man1)
#endif()
#
#
## Icons
#install(DIRECTORY icons/ DESTINATION ${APP_ICONS_DIR})
#
#
## Translations
#QT4_ADD_TRANSLATION(MLPLAYER_QM ${MLPLAYER_TRANSLATIONS})
#install(FILES ${MLPLAYER_QM} DESTINATION ${APP_TRANSLATIONS_DIR})


# Binaries
add_executable(${APP_UNIX_NAME} ${MLPLAYER_SOURCES} ${MLPLAYER_MOC_CPP} ${MLPLAYER_QM} ${MLPLAYER_UI_CPP})
# TODO
target_link_libraries(${APP_UNIX_NAME} medialib ${MLIB_LIBRARIES})
install(TARGETS ${APP_UNIX_NAME} DESTINATION ${APP_BIN_DIR})
