cmake_minimum_required(VERSION 3.12)
project(PositronEngine)

set(PROJECT_NAME PositronEngine)
set(CMAKE_PROJECT_INCLUDE_BEFORE "${CMAKE_CURRENT_SOURCE_DIR}/cmake/EnableCMP0048.cmake")

# Add subdirectories
add_subdirectory(subproj/)
add_subdirectory(PositronEngineCore)
add_subdirectory(PositronEngineEditor)
add_subdirectory(Tests)

# Specify the installation directory for executables and libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Install targets directly into bin
install(TARGETS PositronEngineCore PositronEngineEditor
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

if(WIN32)
  # Install required DLLs (adjust the paths accordingly)
  install(FILES
          "C:/Windows/System32/VCRUNTIME140.dll"
          "C:/Windows/System32/VCRUNTIME140_1.dll"
          "C:/Windows/System32/MSVCP140.dll"
          DESTINATION bin)
endif()

# Install resource files
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/ResourceFiles
        DESTINATION .)

unset(CMAKE_PROJECT_INCLUDE_BEFORE)

# CPack configuration
include(CPack)

# Set CPack variables

if(WIN32)

  set(CPACK_GENERATOR "ZIP")
  set(CPACK_PACKAGE_NAME "PositronEngine_WINDOWS")
  set(CPACK_PACKAGE_VERSION "1.0.0")
  set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
elseif(UNIX)
  set(CPACK_GENERATOR "ZIP")
  set(CPACK_PACKAGE_NAME "PositronEngine_UNIX")
  set(CPACK_PACKAGE_VERSION "1.0.0")
  set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
else()
  set(CPACK_GENERATOR "ZIP")
  set(CPACK_PACKAGE_NAME "PositronEngine")
  set(CPACK_PACKAGE_VERSION "1.0.0")
  set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
endif()

# Include CPack
include(CPack)
