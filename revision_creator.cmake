# XXX, ${revision_h_fake} is used here,
# because we rely on that file being detected as missing
# every build so that the real header "revision.h" is updated.
#
# Keep this until we find a better way to resolve!

set(revision_h_real "${ROOT_PROJECT_DIR}/src/global/include/global/revision.h")
set(revision_h_fake "${ROOT_PROJECT_DIR}/src/global/include/global/revision.h_fake")

if(EXISTS ${revision_h_fake})
    message(FATAL_ERROR "File \"${revision_h_fake}\" found, this should never be created, remove!")
endif()

# a custom target that is always built
add_custom_target(revision ALL
    DEPENDS ${revision_h_fake})

# creates revision.h using cmake script
add_custom_command(
    OUTPUT
        ${revision_h_fake}  # ensure we always run
        ${revision_h_real}
    COMMAND ${ROOT_PROJECT_DIR}/scripts/revision_gen.sh
    WORKING_DIRECTORY ${ROOT_PROJECT_DIR}/scripts/)

# revision.h is a generated file
set_source_files_properties(
    ${revision_h_real}
    PROPERTIES GENERATED TRUE
    HEADER_FILE_ONLY TRUE)

unset(revision_h_real)
unset(revision_h_fake)