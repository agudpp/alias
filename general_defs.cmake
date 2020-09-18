# We will define here all the general definitions we need in order to configure properly this project

# ENV_VARS ALIAS_DEP_ROOT
set(ALIAS_DEP_ROOT "$ENV{ALIAS_DEP_ROOT}/dependencies")

# Root source project is the same for all cases
set(ROOT_PROJECT_DIR ${PROJECT_SOURCE_DIR})

# third party is the same
set(THIRD_PARTY_DIR ${ROOT_PROJECT_DIR}/third_party)
