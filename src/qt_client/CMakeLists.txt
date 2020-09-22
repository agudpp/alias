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
  ${SRC_ROOT_DIR}/mainwindow.ui
  ${SRC_ROOT_DIR}/mainwindow.cpp

  ${SRC_ROOT_DIR}/main.cpp
)

# Define the header list
add_mod_headers(
  ${INCLUDE_ROOT_DIR}/qt_client/mainwindow.h
)

# Define the library dependencies if any
add_mod_dependencies(
  Qt5::Widgets
)

# Define the modules depenndecies
add_dep_module(
  toolbox
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
