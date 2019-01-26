# run it cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_VERBOSE_MAKEFILE=1
################################################################################

# project name
project(alias)
cmake_minimum_required(VERSION 2.8)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)

################################################################################
# check the configuration and unify if
set(DEBUG_MODE false)
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(DEBUG_MODE true)
    message("WE ARE IN DEBUG MODE")
endif ()

message("Current configuration: debug = ${DEBUG_MODE}: string: ${CMAKE_BUILD_TYPE}")

# source root dir
set(PRJ_ROOT_DIR ${PROJECT_SOURCE_DIR})
set(SRC_ROOT_DIR ${PRJ_ROOT_DIR}/src)

# deps root folder
set(DEPS_INCLUDE ${TAG_BACKEND_DEPS}/include)
set(DEPS_LIB ${TAG_BACKEND_DEPS}/lib)

################################################################################
# define global variables for headers | sources | libs | etc

set(COMMON_CXX_FLAGS "-std=c++11 -Wall ")
set(COMMON_DEFINITIONS "-DRAPIDJSON_HAS_STDSTRING=1")
if (DEBUG_MODE)
    set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} -ggdb3")
    set(COMMON_DEFINITIONS "${COMMON_DEFINITIONS} -DDEBUG -DUSE_DEBUG")
else()
    set(COMMON_CXX_FLAGS "${COMMON_CXX_FLAGS} -O3 -fwrapv")
    set(COMMON_DEFINITIONS "${COMMON_DEFINITIONS}")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_CXX_FLAGS}")
add_definitions(${COMMON_DEFINITIONS})



################################################################################
# include directories.
# This will not change normally during the development since we want to use
# "absolute" include directives using this two folders as roots so there is
# no ambiguity when adding libraries / etc.
#
include_directories(${SRC_ROOT_DIR}/)
include_directories(${SRC_ROOT_DIR}/ext-includes)
include_directories(${DEPS_INCLUDE}/)

# libs
#
link_directories(${DEPS_LIB})

# source directories
# here we should add all the folders where we have .cpp in the project.
# if you create a new folder and add sources files (.cpp) then you should
# list it here as well.

# core
aux_source_directory(${SRC_ROOT_DIR}/core/types CORE_SRC_LIST)
aux_source_directory(${SRC_ROOT_DIR}/core/utils CORE_SRC_LIST)

# elements
aux_source_directory(${SRC_ROOT_DIR}/elements ELEMENTS_SRC_LIST)

# tags
aux_source_directory(${SRC_ROOT_DIR}/tags TAGS_SRC_LIST)

# storage
aux_source_directory(${SRC_ROOT_DIR}/storage STORAGE_SRC_LIST)

# service api
aux_source_directory(${SRC_ROOT_DIR}/service_api SERVICE_API_SRC_LIST)

# server
#aux_source_directory(${SRC_ROOT_DIR}/server SERVER_SRC_LIST)
#aux_source_directory(${SRC_ROOT_DIR}/server/endpoints SERVER_SRC_LIST)

# client
set(UI_CLIENT_SRC_LIST
  ${SRC_ROOT_DIR}/ui_client/mainwindow.cpp
  ${SRC_ROOT_DIR}/ui_client/mainwindow.ui
  ${SRC_ROOT_DIR}/ui_client/qt_client.cpp
#  ${SRC_ROOT_DIR}/ui_client/input_window.ui
  ${SRC_ROOT_DIR}/ui_client/tag/tag_widget.ui
  ${SRC_ROOT_DIR}/ui_client/tag/tag_widget.cpp
  ${SRC_ROOT_DIR}/ui_client/tag/tag_handler_widget.ui
  ${SRC_ROOT_DIR}/ui_client/tag/tag_handler_widget.cpp
  ${SRC_ROOT_DIR}/ui_client/tag/tag_list_handler.ui
  ${SRC_ROOT_DIR}/ui_client/tag/tag_list_handler.cpp
  ${SRC_ROOT_DIR}/ui_client/tag/input_text_validator.cpp
  ${SRC_ROOT_DIR}/ui_client/elements/element_widget.cpp
  ${SRC_ROOT_DIR}/ui_client/elements/element_executor.cpp
  ${SRC_ROOT_DIR}/ui_client/elements/element_handler.cpp
  ${SRC_ROOT_DIR}/ui_client/elements/element_handler.ui
  ${SRC_ROOT_DIR}/ui_client/elements/editors/simple_text_editor.ui
  ${SRC_ROOT_DIR}/ui_client/elements/editors/simple_text_editor.cpp
  ${SRC_ROOT_DIR}/ui_client/elements/editors/element_editor.ui
  ${SRC_ROOT_DIR}/ui_client/elements/editors/element_editor.cpp
  ${SRC_ROOT_DIR}/ui_client/elements/editors/element_editor_interface.h
  ${SRC_ROOT_DIR}/ui_client/utils/key_trigger.cpp
)


# source
aux_source_directory(${SRC_ROOT_DIR} SRC_LIST)
set(SRC_LIST ${SRC_LIST}
  ${CORE_SRC_LIST}
  ${ELEMENTS_SRC_LIST}
  ${TAGS_SRC_LIST}
  ${STORAGE_SRC_LIST}
  ${SERVICE_API_SRC_LIST}
#  ${SERVER_SRC_LIST}
  ${UI_CLIENT_SRC_LIST}
)

#aux_source_directory(${SRC_ROOT_DIR}/core/types SRC_LIST)
#aux_source_directory(${SRC_ROOT_DIR}/core/utils SRC_LIST)
#aux_source_directory(${SRC_ROOT_DIR}/tags SRC_LIST)
#aux_source_directory(${SRC_ROOT_DIR}/storage SRC_LIST)
#aux_source_directory(${SRC_ROOT_DIR}/server SRC_LIST)
#aux_source_directory(${SRC_ROOT_DIR}/service_api SRC_LIST)


# headers
# here we should list each module and the new headers files. This is not
# strictely necessary for compiling but is good if we list all of them here
# so we can see them in the IDE (some of them) and also track them.
# Be sure of adding a comment # module_name if you are adding a new folder.
# this will avoid a couple of collisions when merging :)
#
set(HEADERS_LIST ${HEADERS_LIST}
    # core
    ${SRC_ROOT_DIR}/core/debug/Debug.h
    ${SRC_ROOT_DIR}/core/types/basics.h
    ${SRC_ROOT_DIR}/core/types/MinimalIDHandler.h
    ${SRC_ROOT_DIR}/core/types/trie.h
    ${SRC_ROOT_DIR}/core/logger/Loggers.h
    ${SRC_ROOT_DIR}/core/logger/LoggerManager.h
    ${SRC_ROOT_DIR}/core/utils/json_utils.h
    ${SRC_ROOT_DIR}/core/utils/string_utils.h

    # consts
    ${SRC_ROOT_DIR}/consts.h
    ${SRC_ROOT_DIR}/backend_config.h

    # tags
    ${SRC_ROOT_DIR}/tags/tag.h
    ${SRC_ROOT_DIR}/tags/tagmanager.h

    # elements
    ${SRC_ROOT_DIR}/elements/element.h
    ${SRC_ROOT_DIR}/elements/elementmanager.h
    ${SRC_ROOT_DIR}/elements/simple_text_element.h
    ${SRC_ROOT_DIR}/elements/element_builder.h

    # service
    ${SRC_ROOT_DIR}/service_api/serviceapi.h

    # server
#    ${SRC_ROOT_DIR}/server/server.h
#    ${SRC_ROOT_DIR}/server/server_utils.h
#    ${SRC_ROOT_DIR}/server/endpoint_interface.h
#    ${SRC_ROOT_DIR}/server/endpoints/search_endpoint.h
#    ${SRC_ROOT_DIR}/server/endpoints/get_tags_endpoint.h
#    ${SRC_ROOT_DIR}/server/mongoose.h


    # data storage
    ${SRC_ROOT_DIR}/storage/datastorage.h
    ${SRC_ROOT_DIR}/storage/file_storage.h
)


################################################################################
# dependencies list

# libs
set(LIBS_DEPS ${LIBS_DEPS}
  uuid
  qxtglobalshortcut
  Qt5::Widgets
)



# read libs from the current folder also
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
SET(CMAKE_INSTALL_RPATH ".")

add_executable(${PROJECT_NAME} ${SRC_LIST} ${HEADERS_LIST})
target_link_libraries(${PROJECT_NAME} ${LIBS_DEPS})



################################
# Unit Tests
################################
enable_testing()
add_subdirectory(${PRJ_ROOT_DIR}/lib/googletest)
include_directories(${CMAKE_SOURCE_DIR}/googletest/include)

# ELEMENTS FILES
# elements
add_executable(element_tester ${SRC_ROOT_DIR}/tests/element_test.cpp ${CORE_SRC_LIST} ${ELEMENTS_SRC_LIST})
target_link_libraries(element_tester gtest gtest_main ${LIBS_DEPS})
add_test(NAME element_tester COMMAND element_tester)

# Tags
add_executable(tag_tester ${SRC_ROOT_DIR}/tests/tag_test.cpp ${CORE_SRC_LIST} ${TAGS_SRC_LIST})
target_link_libraries(tag_tester gtest gtest_main ${LIBS_DEPS})
add_test(NAME tag_tester COMMAND tag_tester)

# Storage
add_executable(storage_tester ${SRC_ROOT_DIR}/tests/storage_test.cpp
  ${CORE_SRC_LIST}
  ${STORAGE_SRC_LIST}
  ${TAGS_SRC_LIST}
  ${ELEMENTS_SRC_LIST})

target_link_libraries(storage_tester gtest gtest_main ${LIBS_DEPS})
add_test(NAME storage_tester COMMAND storage_tester)
