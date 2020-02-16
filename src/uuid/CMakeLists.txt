################################################################################
cmake_minimum_required(VERSION  3.2.2)

# project name
project(uuid)

# define the module here, we expect to be called from the main cmake
# which include and define the main variables
include(${GLOBAL_CMAKE_CONFIGS_DIR}/define_module.cmake)

# Define the source list
add_mod_sources(
  ${SRC_ROOT_DIR}/clear.c
  ${SRC_ROOT_DIR}/compare.c
  ${SRC_ROOT_DIR}/copy.c
  ${SRC_ROOT_DIR}/gen_uuid.c
  ${SRC_ROOT_DIR}/isnull.c
  ${SRC_ROOT_DIR}/pack.c
  ${SRC_ROOT_DIR}/parse.c
  ${SRC_ROOT_DIR}/unpack.c
  ${SRC_ROOT_DIR}/unparse.c
  ${SRC_ROOT_DIR}/uuid_time.c
)

# Define the header list
add_mod_headers(
  ${INCLUDE_ROOT_DIR}/uuid/config.h
  ${INCLUDE_ROOT_DIR}/uuid/uuid.h
  ${INCLUDE_ROOT_DIR}/uuid/uuidd.h
  ${INCLUDE_ROOT_DIR}/uuid/uuidP.h
  ${INCLUDE_ROOT_DIR}/uuid/uuid_types.h
)

# Define the library dependencies if any
add_mod_dependencies(
)

# Define the modules depenndecies
add_dep_module(
)

# Add the include directories if any
add_mod_include_dirs(
)

add_definitions(
        -DHAVE_INTTYPES_H
        -DHAVE_UNISTD_H
        -DHAVE_ERRNO_H
        -DHAVE_NETINET_IN_H
        -DHAVE_SYS_IOCTL_H
        -DHAVE_SYS_MMAN_H
        -DHAVE_SYS_MOUNT_H
        -DHAVE_SYS_PRCTL_H
        -DHAVE_SYS_RESOURCE_H
        -DHAVE_SYS_SELECT_H
        -DHAVE_SYS_STAT_H
        -DHAVE_SYS_TYPES_H
        -DHAVE_STDLIB_H
        -DHAVE_STRDUP
        -DHAVE_MMAP
        -DHAVE_UTIME_H
        -DHAVE_GETPAGESIZE
        -DHAVE_LSEEK64
        -DHAVE_LSEEK64_PROTOTYPE
        -DHAVE_EXT2_IOCTLS
        -DHAVE_LINUX_FD_H
        -DHAVE_TYPE_SSIZE_T
        -DHAVE_SYS_TIME_H
        -DHAVE_SYS_PARAM_H
        -DHAVE_SYSCONF
)

# build the module
create_module_lib()

# expose definitions
expose_definitions()



# enable or not testing for this module
#add_module_test(
##   files here of testing if not the default one will be used
#)


