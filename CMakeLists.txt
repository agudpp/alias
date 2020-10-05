# run it cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_VERBOSE_MAKEFILE=1
# FOLLOWING STRUCTURE OF CMAKE FROM https://stackoverflow.com/questions/16398937/cmake-and-finding-other-projects-and-their-dependencies/16404000
################################################################################

# project name
project(alias)

cmake_minimum_required(VERSION 3.2.2)

include(general_defs.cmake)
include(revision_creator.cmake)

# include dependencies as root folder
list(APPEND CMAKE_FIND_ROOT_PATH "${ALIAS_DEP_ROOT}")
include_directories(BEFORE SYSTEM "${ALIAS_DEP_ROOT}/include")
include_directories("${ROOT_PROJECT_DIR}/src/global/include")
link_directories("${ALIAS_DEP_ROOT}/lib")

# enable debug messages for now
add_definitions("-DDEBUG_LOG_ENABLE")
if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "Debug")
endif()


# include the cmake project utils framework
include(${THIRD_PARTY_DIR}/cmake-project-utils/cmake_configs/global_definitions.cmake)
include(${THIRD_PARTY_DIR}/cmake-project-utils/cmake_configs/testing.cmake)

# enable debug messages for now
add_definitions("-DDEBUG_LOG_ENABLE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# read libs from the current folder also
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
SET(CMAKE_INSTALL_RPATH ".")


# add google test third party
add_subdirectory(third_party/googletest)


################################
# Unit Tests
################################
enable_global_testing()



# When done tweaking common stuff, configure the components (subprojects).
# NOTE: The order matters! The most independent ones should go first.
add_subdirectory(src/uuid)
add_subdirectory(src/toolbox)
add_subdirectory(src/encryption)
add_subdirectory(src/data)
add_subdirectory(src/actions)
add_subdirectory(src/protos)
add_subdirectory(src/storage)
add_subdirectory(src/service)
add_subdirectory(src/qt_client)

