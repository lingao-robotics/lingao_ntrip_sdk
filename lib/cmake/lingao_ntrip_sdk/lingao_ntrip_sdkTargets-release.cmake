#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "lingao::lingao_ntrip_sdk" for configuration "Release"
set_property(TARGET lingao::lingao_ntrip_sdk APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(lingao::lingao_ntrip_sdk PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/liblingao_ntrip_sdk.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS lingao::lingao_ntrip_sdk )
list(APPEND _IMPORT_CHECK_FILES_FOR_lingao::lingao_ntrip_sdk "${_IMPORT_PREFIX}/lib/liblingao_ntrip_sdk.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
