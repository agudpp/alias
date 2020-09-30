################################################################################
cmake_minimum_required(VERSION  3.2.2)

# project name
project(qt_client)

# define the module here, we expect to be called from the main cmake
# which include and define the main variables
include(${GLOBAL_CMAKE_CONFIGS_DIR}/define_module.cmake)

##
## Qt client specifics
##

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5Widgets CONFIG REQUIRED)


##
## Module
##

# Define the source list
add_mod_sources(
  ${SRC_ROOT_DIR}/../resources.qrc

  ${SRC_ROOT_DIR}/mainwindow.ui
  ${SRC_ROOT_DIR}/mainwindow.cpp

  ${SRC_ROOT_DIR}/data_widgets/tag_widget.ui
  ${SRC_ROOT_DIR}/data_widgets/tag_widget.cpp

  ${SRC_ROOT_DIR}/data_widgets/tag_list_widget.ui
  ${SRC_ROOT_DIR}/data_widgets/tag_list_widget.cpp
  ${SRC_ROOT_DIR}/data_widgets/tag_suggestion_list_widget.ui
  ${SRC_ROOT_DIR}/data_widgets/tag_suggestion_list_widget.cpp

  ${SRC_ROOT_DIR}/common/widget_line_edit.cpp
  ${SRC_ROOT_DIR}/common/key_trigger.cpp
  ${SRC_ROOT_DIR}/common/converter_utils.cpp

  ${SRC_ROOT_DIR}/handlers/input_text_validator.cpp
  ${SRC_ROOT_DIR}/handlers/tag_logic_handler.cpp
  ${SRC_ROOT_DIR}/handlers/tag_search_widget.ui
  ${SRC_ROOT_DIR}/handlers/tag_search_widget.cpp
  ${SRC_ROOT_DIR}/handlers/content_editor_widget.ui
  ${SRC_ROOT_DIR}/handlers/content_editor_widget.cpp
  ${SRC_ROOT_DIR}/handlers/tagger_widget.ui
  ${SRC_ROOT_DIR}/handlers/tagger_widget.cpp

  ${SRC_ROOT_DIR}/content/content_table_widget.cpp
  ${SRC_ROOT_DIR}/content/content_table_widget_item.cpp
  ${SRC_ROOT_DIR}/content/content_widget_interface.cpp
  ${SRC_ROOT_DIR}/content/text_content_widget.ui
  ${SRC_ROOT_DIR}/content/text_content_widget.cpp
  ${SRC_ROOT_DIR}/content/content_widget_builder.cpp
  ${SRC_ROOT_DIR}/content/content_processor.cpp


  ${SRC_ROOT_DIR}/main.cpp
)

# Define the header list
add_mod_headers(
  ${INCLUDE_ROOT_DIR}/qt_client/mainwindow.h

  ${INCLUDE_ROOT_DIR}/qt_client/data_widgets/tag_widget.h
  ${INCLUDE_ROOT_DIR}/qt_client/data_widgets/tag_list_widget.h
  ${INCLUDE_ROOT_DIR}/qt_client/data_widgets/tag_suggestion_list_widget.h

  ${INCLUDE_ROOT_DIR}/qt_client/common/widget_line_edit.h
  ${INCLUDE_ROOT_DIR}/qt_client/common/key_trigger.h
  ${INCLUDE_ROOT_DIR}/qt_client/common/function_key_trigger.h
  ${INCLUDE_ROOT_DIR}/qt_client/common/converter_utils.h

  ${INCLUDE_ROOT_DIR}/qt_client/handlers/input_text_validator.h
  ${INCLUDE_ROOT_DIR}/qt_client/handlers/tag_logic_handler.h
  ${INCLUDE_ROOT_DIR}/qt_client/handlers/tag_search_widget.h
  ${INCLUDE_ROOT_DIR}/qt_client/handlers/content_editor_widget.h
  ${INCLUDE_ROOT_DIR}/qt_client/handlers/tagger_widget.h

  ${INCLUDE_ROOT_DIR}/qt_client/content/content_table_widget.h
  ${INCLUDE_ROOT_DIR}/qt_client/content/content_table_widget_item.h
  ${INCLUDE_ROOT_DIR}/qt_client/content/content_widget_interface.h
  ${INCLUDE_ROOT_DIR}/qt_client/content/text_content_widget.h
  ${INCLUDE_ROOT_DIR}/qt_client/content/content_widget_builder.h
  ${INCLUDE_ROOT_DIR}/qt_client/content/content_processor.h

)

# Define the library dependencies if any
add_mod_dependencies(
  Qt5::Widgets
)

# Define the modules depenndecies
add_dep_module(
  toolbox
  data
  actions
  storage
  service
)

# Add the include directories if any
add_mod_include_dirs(
#   include_dir_1
)


# build the module
create_module_executable()


# enable or not testing for this module
#add_module_test(
  # files here of testing if not the default one will be used
#)
