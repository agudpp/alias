
# Configure the install to be in a specific folder the binary and the libs that is not
# the /usr/bin for now




# check the default destination folder
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  # TODO: check on windows?
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
list(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/app" isSystemDir)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/app")

set(DEST_INSTALL_BIN_PATH ${CMAKE_INSTALL_PREFIX}/app)


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
  qt_client
)

if (UNIX)
  set(INSTALL_TARGETS_LIBS_LIST ${INSTALL_TARGETS_LIBS_LIST} uuid)
endif(UNIX)

install(TARGETS ${INSTALL_TARGETS_LIBS_LIST} DESTINATION app)


# TODO: add the post install target to copy the third party libs (all the deps folder
#       into the destination folder?
# install(CODE "cp -rf ${ALIAS_DEP_ROOT}/lib/*.so ${CMAKE_INSTALL_PREFIX}/app")

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
    install(FILES ${full_list_files} DESTINATION "${DEST_INSTALL_BIN_PATH}/")
  endfunction()

  # we need to copy all the dependencies libs from the bin folder
  install(DIRECTORY ${ALIAS_DEP_ROOT}/bin/ DESTINATION ${DEST_INSTALL_BIN_PATH}
          FILES_MATCHING PATTERN "*.dll")

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
  install(DIRECTORY ${ALIAS_DEP_ROOT}/lib/ DESTINATION ${DEST_INSTALL_BIN_PATH}
          FILES_MATCHING PATTERN "*.so*")
endif(WIN32)

# install the config file
install(FILES "${ROOT_PROJECT_DIR}/resources/config/init.json" DESTINATION ${CMAKE_INSTALL_PREFIX}/)

# create the storage empty folder
install(DIRECTORY DESTINATION ${CMAKE_INSTALL_PREFIX}/storage)


