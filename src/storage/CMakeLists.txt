################################################################################
cmake_minimum_required(VERSION  3.2.2)

# project name
project(storage)

# define the module here, we expect to be called from the main cmake
# which include and define the main variables
include(${GLOBAL_CMAKE_CONFIGS_DIR}/define_module.cmake)

# Define the source list
add_mod_sources(
  ${SRC_ROOT_DIR}/file_storage.cpp
)

# Define the header list
add_mod_headers(
  ${INCLUDE_ROOT_DIR}/storage/data_storage.h
  ${INCLUDE_ROOT_DIR}/storage/file_storage.h
)

# Define the library dependencies if any
add_mod_dependencies(
)

# Define the modules depenndecies
add_dep_module(
  toolbox
  data
  protos
)

# Add the include directories if any
add_mod_include_dirs(
#   include_dir_1
)


# build the module
create_module_lib()

# expose definitions
expose_definitions()



# enable or not testing for this module
add_module_test(
  # files here of testing if not the default one will be used
)
