add_swift_library(swiftDemangle SHARED
  SwiftDemangle.cpp
  MangleHack.cpp
  LINK_LIBRARIES swiftBasic)

swift_install_in_component(compiler
    TARGETS swiftDemangle
    LIBRARY DESTINATION "lib${LLVM_LIBDIR_SUFFIX}"
    ARCHIVE DESTINATION "lib${LLVM_LIBDIR_SUFFIX}")

# Create a compatibility symlink.

if ("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
  add_custom_command(TARGET swiftDemangle POST_BUILD
    COMMAND
      # Create_symlink doesn't work on windows. Use mklink instead.
	    COMMAND "del" "libfunctionNameDemangle.dylib"
	    COMMAND "mklink" "libfunctionNameDemangle.dylib" "libswiftDemangle.dylib"
    WORKING_DIRECTORY "${SWIFT_LIBRARY_OUTPUT_INTDIR}"
    COMMENT "Creating compatibility symlink for functionNameDemangle.dylib")
else()
  add_custom_command(TARGET swiftDemangle POST_BUILD
    COMMAND
      "${CMAKE_COMMAND}" "-E" "create_symlink"
      "libswiftDemangle.dylib"
      "libfunctionNameDemangle.dylib"
    WORKING_DIRECTORY "${SWIFT_LIBRARY_OUTPUT_INTDIR}"
    COMMENT "Creating compatibility symlink for functionNameDemangle.dylib")
endif()

swift_install_in_component(compiler
    FILES "${SWIFT_LIBRARY_OUTPUT_INTDIR}/libfunctionNameDemangle.dylib"
    DESTINATION "lib")

