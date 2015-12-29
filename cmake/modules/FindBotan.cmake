# Try to find the Botan library
#  BOTAN_FOUND - system has Botan lib
#  BOTAN_INCLUDE_DIRS - the include directories needed
#  BOTAN_LIBRARIES - Libraries needed to use Botan

find_path(BOTAN_INCLUDE_DIR NAMES botan/botan.h PATH_SUFFIXES botan-1.10)
find_library(BOTAN_LIBRARY NAMES botan botan-1.10)

set(BOTAN_INCLUDE_DIRS ${BOTAN_INCLUDE_DIR})
set(BOTAN_LIBRARIES ${BOTAN_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(BOTAN DEFAULT_MSG BOTAN_LIBRARY BOTAN_INCLUDE_DIR)

mark_as_advanced(BOTAN_INCLUDE_DIR BOTAN_LIBRARY)
