# run it cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_VERBOSE_MAKEFILE=1
################################################################################

# project name
project(tag_be)
cmake_minimum_required(VERSION 2.8)

################################################################################
#                           Helper methods
################################################################################
# detect the required environment variables
function (checkEnvVariable varName)
    set(var ${varName})
    message("checking environment variable ${varName} -> $ENV{${varName}}")
    if ("$ENV{${varName}}" STREQUAL "")
        message(FATAL_ERROR "environment variable \"${varName}\" doesn't exists")
    else()
        message("environment variable exists: ${varName} -> $ENV{${varName}}")
    endif()
endfunction (checkEnvVariable)


################################################################################
# preconditions and set global variables

# TAG_BACKEND_GIT
checkEnvVariable(TAG_BACKEND_GIT)
set(TAG_BACKEND_GIT $ENV{TAG_BACKEND_GIT})

# TAG_BACKEND_DEPS
#checkEnvVariable(TAG_BACKEND_DEPS)
#set(TAG_BACKEND_DEPS $ENV{TAG_BACKEND_DEPS})

# TAG_BACKEND_BUILDS
checkEnvVariable(TAG_BACKEND_BUILDS)
set(TAG_BACKEND_BUILDS $ENV{TAG_BACKEND_BUILDS})

################################################################################
# check the configuration and unify if
set(DEBUG_MODE false)
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(DEBUG_MODE true)
    message("WE ARE IN DEBUG MODE")
endif ()

message("Current configuration: debug = ${DEBUG_MODE}: string: ${CMAKE_BUILD_TYPE}")


################################################################################
# output folder and other vars
if (DEBUG_MODE)
    set(OUTPUT_FOLDER ${TAG_BACKEND_BUILDS}/debug/)
else()
    set(OUTPUT_FOLDER ${TAG_BACKEND_BUILDS}/release/)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_FOLDER}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_FOLDER}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_FOLDER}/bin)

# source root dir
set(PRJ_ROOT_DIR ${TAG_BACKEND_GIT})
set(SRC_ROOT_DIR ${TAG_BACKEND_GIT}/src)

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

# source
aux_source_directory(${SRC_ROOT_DIR} SRC_LIST)
set(SRC_LIST ${SRC_LIST}
  ${CORE_SRC_LIST}
  ${ELEMENTS_SRC_LIST}
  ${TAGS_SRC_LIST}
  ${STORAGE_SRC_LIST}
  ${SERVICE_API_SRC_LIST}
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

    # consts
    ${SRC_ROOT_DIR}/consts.h

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

#    ${SRC_ROOT_DIR}/server/server.h
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
add_test(element_tester element_tester)

# Tags
add_executable(tag_tester ${SRC_ROOT_DIR}/tests/tag_test.cpp ${CORE_SRC_LIST} ${TAGS_SRC_LIST})
target_link_libraries(tag_tester gtest gtest_main ${LIBS_DEPS})
add_test(tag_tester tag_tester)

# Storage
add_executable(storage_tester ${SRC_ROOT_DIR}/tests/storage_test.cpp
  ${CORE_SRC_LIST}
  ${STORAGE_SRC_LIST}
  ${TAGS_SRC_LIST}
  ${ELEMENTS_SRC_LIST})

target_link_libraries(storage_tester gtest gtest_main ${LIBS_DEPS})
add_test(storage_tester storage_tester)
