# Find Sodium
# Find Sodium library and include directory
# Very rudimentary. Expects Root of Sodium either in
# CMake variable SODIUM_ROOT or in system environment
# variable SODIUM_ROOT

find_library(
        SODIUM_LIBRARY_DIR NAMES sodium libsodium
        HINTS
            ${SODIUM_ROOT} ENV SODIUM_ROOT
        PATH_SUFFIXES lib)
find_path(
        SODIUM_INCLUDE_DIR NAMES sodium.h
        HINTS
            ${SODIUM_ROOT} ENV SODIUM_ROOT
        PATH_SUFFIXES include)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Sodium DEFAULT_MSG SODIUM_LIBRARY_DIR SODIUM_INCLUDE_DIR)