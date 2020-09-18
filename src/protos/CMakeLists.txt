################################################################################
cmake_minimum_required(VERSION  3.2.2)

# project name
project(protos)

# build protos
set(PROTO_AUTOGEN_OUT_FOLDER ${CMAKE_CURRENT_BINARY_DIR})
set(PROTO_INPUT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR})

# define the module here, we expect to be called from the main cmake
# which include and define the main variables
include(${GLOBAL_CMAKE_CONFIGS_DIR}/define_module.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/build_protos.cmake)


# Define the source list
add_mod_sources(
  ${PROTO_SRCS}
  ${SRC_ROOT_DIR}/convert_utils.cpp
)

# Define the header list
add_mod_headers(
  ${PROTO_HDRS}
  ${INCLUDE_ROOT_DIR}/protos/convert_utils.h
)

# Define the library dependencies if any
add_mod_dependencies(
  ${PROTOBUF_LIBRARIES}
)

# module definitions
add_mod_definitions(
  ${PROTOBUF_DEFINITIONS}
)

# Define the modules depenndecies
add_dep_module(
  data
)

# Add the include directories if any
add_mod_include_dirs(
  ${PROTO_AUTOGEN_OUT_FOLDER}
  ${PROTOBUF_INCLUDE_DIRS}
)


# build the module
create_module_lib()

# expose definitions
expose_definitions()



# enable or not testing for this module
#add_module_test(
#    # files here of testing if not the default one will be used
#)


