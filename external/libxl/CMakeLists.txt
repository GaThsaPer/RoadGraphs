add_library(libxl SHARED IMPORTED GLOBAL)

#Set localization of Library
if(APPLE)
    set(LIBXL_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/libxl.dylib")
elseif(UNIX AND NOT APPLE)
    set(LIBXL_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/libxl.so")
elseif(WIN32)
    set(LIBXL_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/libxl.dll")
endif()

#Set properties
set_target_properties(libxl PROPERTIES
    IMPORTED_LOCATION "${LIBXL_LIB_PATH}"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include_cpp"
)

#Copy files to compile directory
add_custom_target(copy_libxl_libs ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${LIBXL_LIB_PATH}
    ${CMAKE_BINARY_DIR}
    COMMENT "Copying libxl library to build directory"
)