###############################################################################
cmake_minimum_required(VERSION 3.0)
project(VVR_OGL_LABORATORY)

###############################################################################

find_package(OpenGL REQUIRED)

if(UNIX OR APPLE)
    find_package(TBB REQUIRED)
endif()

set (CMAKE_CXX_STANDARD 17)

# c++17, -g option is used to export debug symbols for gdb
if(${CMAKE_CXX_COMPILER_ID} MATCHES GNU OR
    ${CMAKE_CXX_COMPILER_ID} MATCHES Clang)
  # Using C++17 on OSX requires using libc++ instead of libstd++.
  # libc++ is an implementation of the C++ standard library for OSX.
  if(APPLE)
    if(XCODE)
      set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++17")
      set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    else()
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -stdlib=libc++")
    endif()
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g")
  endif()
endif()

# define library postfix so that different builds will produce
# distinguished libraries
set(CMAKE_RELEASE_POSTFIX "_r" CACHE string "Release postfix")
set(CMAKE_DEBUG_POSTFIX "_d" CACHE string "Debug postfix")
set(CMAKE_RELWITHDEBINFO_POSTFIX "_rd" CACHE string
  "Release with debug info postfix")
set(CMAKE_MINSIZEREL_POSTFIX "_mr" CACHE string
  "Minimum size release postfix")
mark_as_advanced(CMAKE_RELEASE_POSTFIX)
mark_as_advanced(CMAKE_DEBUG_POSTFIX)
mark_as_advanced(CMAKE_RELWITHDEBINFO_POSTFIX)
mark_as_advanced(CMAKE_MINSIZEREL_POSTFIX)

# ctest
include(CTest)
enable_testing()

# for rdm (emacs)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# for grouping into folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if(CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
  message(FATAL_ERROR "Please select another Build Directory ! (and give it a
  clever name, like bin_Visual2012_64bits/)")
endif()
if(CMAKE_SOURCE_DIR MATCHES " ")
  message( "Your Source Directory contains spaces. If you experience problems
    when compiling, this can be the cause." )
endif()
if(CMAKE_BINARY_DIR MATCHES " ")
  message("Your Build Directory contains spaces. If you experience problems
  when compiling, this can be the cause.")
endif()

# Compile external dependencies
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
cmake_policy(SET CMP0026 OLD)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
include(CreateLaunchers)
include(MSVCMultipleProcessCompile) # /MP


########### Dear ImGui import code
set(IMGUI_PATH ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui-1.87)
add_library(imgui 
        ${IMGUI_PATH}/imgui.cpp
        ${IMGUI_PATH}/imgui_draw.cpp
        ${IMGUI_PATH}/imgui_demo.cpp
        ${IMGUI_PATH}/imgui_tables.cpp
        ${IMGUI_PATH}/imgui_widgets.cpp
        ${IMGUI_PATH}/backends/imgui_impl_glfw.cpp
        ${IMGUI_PATH}/backends/imgui_impl_opengl3.cpp)
target_compile_definitions(imgui PUBLIC -DIMGUI_IMPL_OPENGL_LOADER_GLEW)
target_include_directories(imgui PUBLIC ${IMGUI_PATH}/ ${IMGUI_PATH}/backends/)

target_link_libraries(imgui GLEW_1130 glfw)




###############################################################################

include_directories(
  external/glfw-3.2.1/include/GLFW/
  external/glm-0.9.7.1/
  external/glew-1.13.0/include/
  external/Simple-OpenGL-Image-Library/include/
  external/tinyxml2/include/
  external/tinyobjloader/include/
  #external/ImGuiFileDialog/
  .
  )

set(ALL_LIBS
  ${OPENGL_LIBRARY}
  glfw
  GLEW_1130
  SOIL
  TINYXML2
  winmm.lib
  imgui
  ${ITK_LIBRARIES}
  ${TBB_IMPORTED_TARGETS}
  )

add_definitions(
  -DTW_STATIC
  -DTW_NO_LIB_PRAGMA
  -DTW_NO_DIRECT3D
  -DGLEW_STATIC
  -D_CRT_SECURE_NO_WARNINGS
  )


###############################################################################
# chess3D
add_executable(chess3D
  chess3D/main.cpp

  common/util.cpp
  common/util.h
  common/shader.cpp
  common/shader.h
  common/camera.cpp
  common/camera.h
  common/model.cpp
  common/model.h
  common/texture.cpp
  common/texture.h
  common/light.cpp
  common/light.h

  chess3D/resources/shaders/ShadowMapping.fragmentshader
  chess3D/resources/shaders/ShadowMapping.vertexshader
  chess3D/resources/shaders/Depth.fragmentshader
  chess3D/resources/shaders/Depth.vertexshader
  chess3D/resources/shaders/Depth.geometryshader
  chess3D/resources/shaders/Selector.fragmentshader
  chess3D/resources/shaders/Selector.vertexshader
  chess3D/resources/shaders/Skybox.fragmentshader
  chess3D/resources/shaders/Skybox.vertexshader
  chess3D/resources/shaders/Water.fragmentshader
  chess3D/resources/shaders/Water.vertexshader
  chess3D/Piece.cpp
  chess3D/Piece.h
  chess3D/Rook.cpp
  chess3D/Rook.h
  chess3D/Knight.cpp
  chess3D/Knight.h
  chess3D/Bishop.cpp
  chess3D/Bishop.h
  chess3D/Queen.cpp
  chess3D/Queen.h
  chess3D/King.cpp
  chess3D/King.h
  chess3D/Pawn.cpp
  chess3D/Pawn.h

  external/ImGuiFileDialog-0.6.4/ImGuiFileDialog.cpp
  )
target_link_libraries(chess3D
  ${ALL_LIBS}
  )
# Xcode and Visual working directories
set_target_properties(chess3D
  PROPERTIES
  XCODE_ATTRIBUTE_CONFIGURATION_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/chess3D/"
  PROJECT_LABEL "Project 4 - 3D Chess 1"
  FOLDER "Demo"
  )
create_target_launcher(chess3D WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/chess3D/")
create_default_target_launcher(chess3D WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/chess3D/")

###############################################################################

SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )
SOURCE_GROUP(obj REGULAR_EXPRESSION ".*/.*obj$" )

###############################################################################
# copy
if (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )



  add_custom_command(
    TARGET chess3D POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/chess3D${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/chess3D/"
    )
    


elseif (${CMAKE_GENERATOR} MATCHES "Xcode" )

endif (NOT ${CMAKE_GENERATOR} MATCHES "Xcode" )
