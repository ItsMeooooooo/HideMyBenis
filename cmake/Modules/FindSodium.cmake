# Find Sodium
# Find Sodium library and include directory
# Very rudimentary. Expects Root of Sodium either in
# CMake variable SODIUM_ROOT or in system environment
# variable SODIUM_ROOT.
if( CMAKE_SIZEOF_VOID_P EQUAL 8 )
    MESSAGE( "64 bits compiler detected" )
	find_library(
		SODIUM_LIBRARY_DIR NAMES sodium libsodium
		HINTS
			${SODIUM_ROOT} ENV SODIUM_ROOT
			${CMAKE_SOURCE_DIR}/libsodium/x64/Release/v140/dynamic
		PATH_SUFFIXES lib)
else( CMAKE_SIZEOF_VOID_P EQUAL 8 ) 
    MESSAGE( "32 bits compiler detected" )
	find_library(
		SODIUM_LIBRARY_DIR NAMES sodium libsodium
		HINTS
			${SODIUM_ROOT} ENV SODIUM_ROOT
			${CMAKE_SOURCE_DIR}/libsodium/Win32/Release/v140/dynamic
		PATH_SUFFIXES lib)
endif( CMAKE_SIZEOF_VOID_P EQUAL 8 )

find_path(
        SODIUM_INCLUDE_DIR NAMES sodium.h
        HINTS
            ${SODIUM_ROOT} ENV SODIUM_ROOT
			${CMAKE_SOURCE_DIR}/libsodium
        PATH_SUFFIXES include)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Sodium DEFAULT_MSG SODIUM_LIBRARY_DIR SODIUM_INCLUDE_DIR)