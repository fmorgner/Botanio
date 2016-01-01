find_package(PkgConfig)

pkg_check_modules(BOTAN botan-1.11 QUIET)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Botan DEFAULT_MSG BOTAN_LIBRARIES BOTAN_INCLUDE_DIRS)

if(BOTAN_LIBRARY_DIRS)
  link_directories(${BOTAN_LIBRARY_DIRS})
endif()

mark_as_advanced(BOTAN_INCLUDE_DIRS BOTAN_LIBRARIES)
