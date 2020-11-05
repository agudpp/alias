
# Configure the install to be in a specific folder the binary and the libs that is not
# the /usr/bin for now


# check the default destination folder
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  if(WIN32)
    set(USER_HOME_DIR "C:/alias/")
  else(WIN32)
    set(USER_HOME_DIR $ENV{HOME})
  endif(WIN32)  
  if(NOT USER_HOME_DIR)
    message(FATAL_ERROR "We couldn't find the home folder where we want to install the app")
  else()
    set(CMAKE_INSTALL_PREFIX  "${USER_HOME_DIR}/alias" CACHE PATH "..." FORCE)
  endif()
endif()
message(STATUS "We will install the app in ${CMAKE_INSTALL_PREFIX}")

# avoid pointing to local link folder
set(CMAKE_SKIP_BUILD_RPATH  FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}" isSystemDir)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}")

set(DEST_INSTALL_BIN_PATH ${CMAKE_INSTALL_PREFIX})


# set the install targets for all the project libs
# TODO: we can iterate on the global variable of all targets here instead?
set(INSTALL_TARGETS_LIBS_LIST   
  toolbox
  encryption
  data
  actions
  protos
  storage
  service
)

if (UNIX)
  set(INSTALL_TARGETS_LIBS_LIST ${INSTALL_TARGETS_LIBS_LIST} uuid)
endif(UNIX)

# libraries
install(TARGETS ${INSTALL_TARGETS_LIBS_LIST} DESTINATION / COMPONENT libraries)

# applicationes
install(TARGETS qt_client DESTINATION / COMPONENT applications)


# TODO: add the post install target to copy the third party libs (all the deps folder
#       into the destination folder?
# install(CODE "cp -rf ${ALIAS_DEP_ROOT}/lib/*.so /")

if(WIN32)
  ##
  ## Windows installations
  ##

  # check the env vars are set
  set(MINGW64_ROOT $ENV{MINGW64_ROOT})
  set(QT_MINGW_ROOT $ENV{QT_MINGW_ROOT})
  assert_def_exists(MINGW64_ROOT)
  assert_def_exists(QT_MINGW_ROOT)

  function(copy_libs ROOT_FOLDER LIB_NAMES)
    set(full_list_files "")
    foreach(the_lib IN LISTS ${LIB_NAMES})
      set(full_lib_path "${ROOT_FOLDER}/${the_lib}.dll")
      set(full_list_files ${full_list_files} ${full_lib_path})
    endforeach()
    install(FILES ${full_list_files} DESTINATION / COMPONENT libraries)
  endfunction()

  # we need to copy all the dependencies libs from the bin folder
  install(DIRECTORY ${ALIAS_DEP_ROOT}/bin/ DESTINATION /
          COMPONENT libraries FILES_MATCHING PATTERN "*.dll" )

  # we need to install qt dependencies
  if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(QT_DEP_LIBS Qt5Cored Qt5Guid Qt5Widgetsd)
  else()
    set(QT_DEP_LIBS Qt5Core Qt5Gui Qt5Widgets)
  endif()
  copy_libs("${QT_MINGW_ROOT}/bin" QT_DEP_LIBS)

  # We need to copy the mingw c++ related libs
  set(MINGW_DEP_LIBS libgcc_s_seh-1 libstdc++-6 libwinpthread-1)
  copy_libs("${MINGW64_ROOT}/bin" MINGW_DEP_LIBS)

else(WIN32)
  install(DIRECTORY ${ALIAS_DEP_ROOT}/lib/ DESTINATION /
          COMPONENT libraries FILES_MATCHING PATTERN "*.so*")
endif(WIN32)

# Install the folder where we will install the configuration data
if(WIN32)
  set(DEST_INSTALL_DATA_PATH "C:/Users/$ENV{USERNAME}/alias")
else(WIN32)
  set(DEST_INSTALL_DATA_PATH $ENV{HOME}/alias)
endif(WIN32)


# install the config file
install(FILES "${ROOT_PROJECT_DIR}/resources/config/init.json" DESTINATION / COMPONENT libraries)


##
## CPACK macros below here
##

set (CPACK_PACKAGE_NAME "alias")
set (CPACK_PACKAGE_VENDOR "agudpp")
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "the tagging content tool")
set (CPACK_PACKAGE_VERSION "0.0.1")
set (CPACK_PACKAGE_VERSION_MAJOR "0")
set (CPACK_PACKAGE_VERSION_MINOR "0")
set (CPACK_PACKAGE_VERSION_PATCH "1")
set (CPACK_PACKAGE_INSTALL_DIRECTORY "alias")

# Define components and their display names
set (CPACK_COMPONENTS_ALL applications libraries)
set (CPACK_COMPONENT_APPLICATIONS_DISPLAY_NAME "Alias app")
set (CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "Required libraries")
#set (CPACK_COMPONENT_HEADERS_DISPLAY_NAME "C++ Headers")

# Human readable component descriptions
set (CPACK_COMPONENT_APPLICATIONS_DESCRIPTION
  "Alias qt client app")
set (CPACK_COMPONENT_LIBRARIES_DESCRIPTION
  "Required libraries to run qt client app")
#set (CPACK_COMPONENT_HEADERS_DESCRIPTION
#  "C/C++ header files for use with MyLib")

# Define dependencies between components
set (CPACK_COMPONENT_APPLICATIONS_DEPENDS libraries)

# Define groups
set(CPACK_COMPONENT_APPLICATIONS_GROUP "Runtime")
set(CPACK_COMPONENT_LIBRARIES_GROUP "Runtime")
#set(CPACK_COMPONENT_HEADERS_GROUP "Development")

set(CPACK_COMPONENT_GROUP_DEVELOPMENT_DESCRIPTION
   "All of the tools you'll ever need to develop software")

# Define NSIS installation types
set(CPACK_ALL_INSTALL_TYPES Full)
set(CPACK_COMPONENT_LIBRARIES_INSTALL_TYPES Full)
#set(CPACK_COMPONENT_HEADERS_INSTALL_TYPES Developer Full)
set(CPACK_COMPONENT_APPLICATIONS_INSTALL_TYPES Full)
 
# Must be after the last CPACK macros
include(CPack)



