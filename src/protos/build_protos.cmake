################################################################################
# This file will provide the interface to build all the .proto files into a specific
# folder and return the sources and headers of them
#

cmake_minimum_required(VERSION  3.2.2)

# define the module here, we expect to be called from the main cmake
# which include and define the main variables
include(${GLOBAL_CMAKE_CONFIGS_DIR}/define_module.cmake)

# ensure we have defined the input and output folder for this
assert_def_exists(PROTO_AUTOGEN_OUT_FOLDER)
assert_def_exists(PROTO_INPUT_FOLDER)    # ${CMAKE_CURRENT_SOURCE_DIR}/defs
set(PROTOC_COMPILER "")
if (WIN32)
    set(PROTOC_COMPILER "${ALIAS_DEP_ROOT}/bin/protoc.exe")
else(WIN32)
    set(PROTOC_COMPILER "${ALIAS_DEP_ROOT}/bin/protoc")
endif(WIN32)
assert_file_exists("${PROTOC_COMPILER}")

message("+++++ USING PROTOC COMPILER: " ${PROTOC_COMPILER})

# Search proto depending if we are on android or not
set(PROTOBUF_LIBRARIES "")
set(PROTOBUF_INCLUDE_DIRS "")
set(PROTOBUF_PROTO_DIR ${PROTO_INPUT_FOLDER})

# ensure we have defined the output folder for this
assert_def_exists(PROTO_AUTOGEN_OUT_FOLDER)

#if (CMAKE_SYSTEM_NAME STREQUAL Android)

set(PROTOBUF_LIBRARIES ${PROTOBUF_LIBRARIES} protobuf-lite)
set(PROTOBUF_INCLUDE_DIRS ${ALIAS_DEP_ROOT})

# TODO: define properly here the path to the protoc
set(PROTOC_OUT_PATH ${PROTO_AUTOGEN_OUT_FOLDER})
# check exists
execute_process(COMMAND ${PROTOC_COMPILER} --version)
execute_process(COMMAND "mkdir" -p ${PROTOC_OUT_PATH})
# append the gen path as include dir
set(PROTOBUF_INCLUDE_DIRS ${PROTOBUF_INCLUDE_DIRS} ${PROTOC_OUT_PATH})


function(PROTOBUF_GENERATE_CPP SRCS HDRS)
    cmake_parse_arguments(protobuf_generate_cpp "" "EXPORT_MACRO" "" ${ARGN})

    set(_proto_files "${protobuf_generate_cpp_UNPARSED_ARGUMENTS}")
    if(NOT _proto_files)
        message(SEND_ERROR "Error: PROTOBUF_GENERATE_CPP() called without any proto files")
        return()
    endif()

    if(PROTOBUF_GENERATE_CPP_APPEND_PATH)
        set(_append_arg APPEND_PATH)
    endif()

    if(DEFINED Protobuf_IMPORT_DIRS)
        set(_import_arg IMPORT_DIRS ${Protobuf_IMPORT_DIRS})
    endif()

    execute_process(COMMAND ${PROTOC_COMPILER} "--proto_path=${PROTOBUF_PROTO_DIR}" "--cpp_out=${PROTOC_OUT_PATH}" ${ARGN})
    set(${SRCS})
    set(${HDRS})
    file(GLOB ProtoBufGenFiles "${PROTOC_OUT_PATH}/*")

    foreach(_file ${ProtoBufGenFiles})
        if(_file MATCHES "cc$")
            list(APPEND ${SRCS} ${_file})
        elseif(_file MATCHES "h$")
            list(APPEND ${HDRS} ${_file})
        else()
            # nothing
        endif()
    endforeach()
    set(${SRCS} ${${SRCS}} PARENT_SCOPE)
    set(${HDRS} ${${HDRS}} PARENT_SCOPE)
endfunction()

# else()
#     find_package(Protobuf REQUIRED PATHS "${ALIAS_DEP_ROOT}/lib/cmake/protobuf/")
#     # check if protobuf was found
#     if(Protobuf_FOUND)
#         message ("protobuf found")
#     else()
#         message (FATAL_ERROR "Cannot find Protobuf")
#     endif()
# endif()

message("   --> PROTOBUF LIB: ${PROTOBUF_LIBRARIES}")



# Generate the .h and .cxx files
file(GLOB ProtoFiles "${PROTOBUF_PROTO_DIR}/*.proto")
message("   ----> ProtoFiles: ${ProtoFiles}")
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS
  ${ProtoFiles}
)

# Print path to generated files
message ("PROTO_SRCS = ${PROTO_SRCS}")
message ("PROTO_HDRS = ${PROTO_HDRS}")
message ("PROTOBUF_LIBRARIES = ${PROTOBUF_LIBRARIES}")
message ("PROTO_AUTOGEN_OUT_FOLDER = ${PROTO_AUTOGEN_OUT_FOLDER}")

