
####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was lingao_ntrip_sdk-config.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

include(CMakeFindDependencyMacro)
find_dependency(Threads)

string(TOLOWER "${CMAKE_SYSTEM_PROCESSOR}" _lingao_cpu)
set(_lingao_arch_tokens "")
if(_lingao_cpu MATCHES "^(x86_64|amd64)$")
  list(APPEND _lingao_arch_tokens "amd64" "x86_64" "x64")
elseif(_lingao_cpu MATCHES "^(x86|i[3-6]86)$")
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    list(APPEND _lingao_arch_tokens "amd64" "x86_64" "x64")
  else()
    list(APPEND _lingao_arch_tokens "i386" "x86")
  endif()
elseif(_lingao_cpu MATCHES "^(aarch64|arm64)$")
  list(APPEND _lingao_arch_tokens "arm64v8" "aarch64" "arm64")
elseif(_lingao_cpu MATCHES "^(arm|armv7|armv7l|armhf)$")
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    list(APPEND _lingao_arch_tokens "arm64v8" "aarch64" "arm64")
  else()
    list(APPEND _lingao_arch_tokens "arm32v7" "armv7l" "armv7" "armhf" "arm")
  endif()
else()
  list(APPEND _lingao_arch_tokens "${_lingao_cpu}")
endif()
list(REMOVE_DUPLICATES _lingao_arch_tokens)

set(_lingao_lib_candidates "")
foreach(_arch IN LISTS _lingao_arch_tokens)
  list(APPEND _lingao_lib_candidates
    "${PACKAGE_PREFIX_DIR}/lib/${_arch}/liblingao_ntrip_sdk.so"
    "${PACKAGE_PREFIX_DIR}/lib/${_arch}/liblingao_ntrip_sdk.a"
  )
endforeach()
list(APPEND _lingao_lib_candidates
  "${PACKAGE_PREFIX_DIR}/lib/liblingao_ntrip_sdk.so"
  "${PACKAGE_PREFIX_DIR}/lib/liblingao_ntrip_sdk.a"
)

set(_lingao_selected_lib "")
foreach(_candidate IN LISTS _lingao_lib_candidates)
  if(EXISTS "${_candidate}")
    set(_lingao_selected_lib "${_candidate}")
    break()
  endif()
endforeach()

if("${_lingao_selected_lib}" STREQUAL "")
  foreach(_arch IN LISTS _lingao_arch_tokens)
    file(GLOB _versioned_arch "${PACKAGE_PREFIX_DIR}/lib/${_arch}/liblingao_ntrip_sdk.so.*")
    if(NOT "${_versioned_arch}" STREQUAL "")
      list(GET _versioned_arch 0 _lingao_selected_lib)
      break()
    endif()
  endforeach()
endif()
if("${_lingao_selected_lib}" STREQUAL "")
  file(GLOB _versioned_root "${PACKAGE_PREFIX_DIR}/lib/liblingao_ntrip_sdk.so.*")
  if(NOT "${_versioned_root}" STREQUAL "")
    list(GET _versioned_root 0 _lingao_selected_lib)
  endif()
endif()

if("${_lingao_selected_lib}" STREQUAL "")
  string(REPLACE ";" "\n  " _lingao_candidates_fmt "${_lingao_lib_candidates}")
  message(FATAL_ERROR
    "lingao_ntrip_sdk library not found for architecture '${CMAKE_SYSTEM_PROCESSOR}'.\n"
    "Checked:\n  ${_lingao_candidates_fmt}"
  )
endif()

if(NOT TARGET lingao::lingao_ntrip_sdk)
  get_filename_component(_lingao_lib_ext "${_lingao_selected_lib}" EXT)
  if(_lingao_lib_ext STREQUAL ".a")
    add_library(lingao::lingao_ntrip_sdk STATIC IMPORTED GLOBAL)
  else()
    add_library(lingao::lingao_ntrip_sdk SHARED IMPORTED GLOBAL)
  endif()

  set_target_properties(lingao::lingao_ntrip_sdk PROPERTIES
    IMPORTED_LOCATION "${_lingao_selected_lib}"
    INTERFACE_INCLUDE_DIRECTORIES "${PACKAGE_PREFIX_DIR}/include"
    INTERFACE_LINK_LIBRARIES "Threads::Threads"
  )
endif()

if(NOT TARGET lingao_ntrip_sdk)
  add_library(lingao_ntrip_sdk INTERFACE IMPORTED)
  target_link_libraries(lingao_ntrip_sdk INTERFACE lingao::lingao_ntrip_sdk)
endif()

check_required_components(lingao_ntrip_sdk)
