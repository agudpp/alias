################################################################################
cmake_minimum_required(VERSION  3.2.2)

# project name
project(toolbox)

# define the module here, we expect to be called from the main cmake
# which include and define the main variables
include(${GLOBAL_CMAKE_CONFIGS_DIR}/define_module.cmake)


set(PLATFORM_DEPENDENCIES "")
if (UNIX)
	set(PLATFORM_DEPENDENCIES uuid)
endif(UNIX)

# Define the source list
add_mod_sources(
  ${SRC_ROOT_DIR}/utils/string_utils.cpp
  ${SRC_ROOT_DIR}/config/config.cpp
  ${SRC_ROOT_DIR}/utils/json_utils.cpp
  ${SRC_ROOT_DIR}/os/os_helper.cpp
  ${SRC_ROOT_DIR}/logger/logger_utils.cpp
  ${SRC_ROOT_DIR}/types/trie.cpp
)

# Define the header list
add_mod_headers(
)

# Define the library dependencies if any
add_mod_dependencies(
	crossguid
)

# Define the modules depenndecies
add_dep_module(
  ${PLATFORM_DEPENDENCIES}
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
#add_module_test(
#    # files here of testing if not the default one will be used
#)
