include(./.env.cmake OPTIONAL RESULT_VARIABLE LOCAL_ENV)
message(STATUS "Local .env.cmake: ${LOCAL_ENV}")

cmake_minimum_required(VERSION 3.11.0)

set(NAME Vulki)

message(STATUS "using ${CMAKE_GENERATOR}")
if (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
    if (NOT MINGW_PATH) 
      message(FATAL_ERROR "MINGW_PATH not set in .env.cmake")
    endif()
    set(USE_MINGW "True")
    set(CMAKE_C_COMPILER ${MINGW_PATH}/bin/gcc.exe)
    set(CMAKE_CXX_COMPILER  ${MINGW_PATH}/bin/g++.exe)
endif()

project(${NAME} VERSION 0.1.0)

# 1. Set VULKAN_SDK_PATH in .env.cmake to target specific vulkan version
if (DEFINED VULKAN_SDK_PATH)
  set(Vulkan_INCLUDE_DIRS "${VULKAN_SDK_PATH}/Include")
  set(Vulkan_LIBRARIES "${VULKAN_SDK_PATH}/Lib")
  set(Vulkan_FOUND "True")
else()
  find_package(Vulkan REQUIRED)
  message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
endif()
if (NOT Vulkan_FOUND)
	message(FATAL_ERROR "Could not find Vulkan library!")
else()
	message(STATUS "Using vulkan lib at: ${Vulkan_LIBRARIES}")
endif()

# 2. Set GLFW_PATH in .env.cmake to target specific glfw
if (DEFINED GLFW_PATH)
  message(STATUS "Using GLFW path specified in .env")
  set(GLFW_INCLUDE_DIRS "${GLFW_PATH}/include")
  if (MSVC)
    set(GLFW_LIB "${GLFW_PATH}/lib-vc2022") 
  elseif (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
    message(STATUS "USING MINGW")
    set(GLFW_LIB "${GLFW_PATH}/lib-mingw-w64")
  endif()
else()
  set(GLFW_PATH "${PROJECT_SOURCE_DIR}/external/glfw-3.3.9.bin.WIN64")
  if (EXISTS "${GLFW_PATH}")
    message(STATUS "Using GLFW from ${GLFW_PATH}")
    set(GLFW_INCLUDE_DIRS "${GLFW_PATH}/include")
    if (MSVC)
      set(GLFW_LIB "${GLFW_PATH}/lib-vc2022")
    elseif (CMAKE_GENERATOR STREQUAL "MinGW Makefiles")
      set(GLFW_LIB "${GLFW_PATH}/lib-mingw-w64")
    endif()
  else()
    find_package(glfw3 3.3 REQUIRED)
    set(GLFW_LIB glfw)
    message(STATUS "Found GLFW")
  endif()
endif()
if (NOT GLFW_LIB)
	message(FATAL_ERROR "Could not find glfw library!")
else()
	message(STATUS "Using glfw lib at: ${GLFW_LIB}")
endif()

include_directories(external)

# Check for GLM_PATH
if (NOT DEFINED GLM_PATH)
  set(GLM_PATH "${PROJECT_SOURCE_DIR}/external/glm")
  if (EXISTS "${GLM_PATH}")
    message(STATUS "Using GLM from ${GLM_PATH}")
  else()
    message(FATAL_ERROR "GLM not found. Please set GLM_PATH in .env.cmake or ensure it's in ${GLM_PATH}")
  endif()
endif()

# Check for TINYOBJ_PATH
if (NOT DEFINED TINYOBJ_PATH)
  set(TINYOBJ_PATH "${PROJECT_SOURCE_DIR}/external/tinyobjloader")
  if (NOT EXISTS "${TINYOBJ_PATH}")
    set(TINYOBJ_PATH "${PROJECT_SOURCE_DIR}/external/tinyobjectloader")
  endif()
  if (EXISTS "${TINYOBJ_PATH}")
    message(STATUS "Using tinyobjloader from ${TINYOBJ_PATH}")
  else()
    message(STATUS "TINYOBJ_PATH not specified and not found in expected locations, using external/tinyobjloader")
    set(TINYOBJ_PATH external/tinyobjloader)
  endif()
endif()

# Gather all source files
file(GLOB_RECURSE SOURCES 
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/src/*.h
    ${PROJECT_SOURCE_DIR}/src/*.hpp
)

# Separate source and header files
set(CPP_SOURCES "")
set(HEADER_FILES "")
foreach(SOURCE ${SOURCES})
    if(${SOURCE} MATCHES ".*\\.cpp$")
        list(APPEND CPP_SOURCES ${SOURCE})
    elseif(${SOURCE} MATCHES ".*\\.(h|hpp)$")
        list(APPEND HEADER_FILES ${SOURCE})
    endif()
endforeach()

add_executable(${PROJECT_NAME} ${CPP_SOURCES} ${HEADER_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Source Files" FILES ${CPP_SOURCES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src PREFIX "Header Files" FILES ${HEADER_FILES})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

if (WIN32)
  message(STATUS "CREATING BUILD FOR WINDOWS")

  if (USE_MINGW)
    target_include_directories(${PROJECT_NAME} PUBLIC
      ${MINGW_PATH}/include
    )
    target_link_directories(${PROJECT_NAME} PUBLIC
      ${MINGW_PATH}/lib
    )
  endif()

  target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCE_DIR}/src
    ${Vulkan_INCLUDE_DIRS}
    ${TINYOBJ_PATH}
    ${GLFW_INCLUDE_DIRS}
    ${GLM_PATH}
    )

  target_link_directories(${PROJECT_NAME} PUBLIC
    ${Vulkan_LIBRARIES}
    ${GLFW_LIB}
  )

  target_link_libraries(${PROJECT_NAME} glfw3 vulkan-1)
elseif (UNIX)
    message(STATUS "CREATING BUILD FOR UNIX")
    target_include_directories(${PROJECT_NAME} PUBLIC
      ${PROJECT_SOURCE_DIR}/src
      ${TINYOBJ_PATH}
    )
    target_link_libraries(${PROJECT_NAME} glfw ${Vulkan_LIBRARIES})
endif()

############## Build SHADERS #######################

# Find the glslangValidator executable
# This tool is used to compile GLSL shaders to SPIR-V
find_program(GLSL_VALIDATOR glslangValidator HINTS 
  ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} 
  /usr/bin 
  /usr/local/bin 
  ${VULKAN_SDK_PATH}/Bin
  ${VULKAN_SDK_PATH}/Bin32
  $ENV{VULKAN_SDK}/Bin/ 
  $ENV{VULKAN_SDK}/Bin32/
)

# Find all vertex and fragment shader source files in the shaders directory
# GLOB_RECURSE is used to search subdirectories as well
file(GLOB_RECURSE GLSL_SOURCE_FILES
  "${PROJECT_SOURCE_DIR}/shaders/*.frag"
  "${PROJECT_SOURCE_DIR}/shaders/*.vert"
)

# Loop through each shader source file
foreach(GLSL ${GLSL_SOURCE_FILES})
  # Extract the filename from the full path
  get_filename_component(FILE_NAME ${GLSL} NAME)
  
  # Set the output path for the compiled SPIR-V file
  # It will have the same name as the input file but with .spv extension
  set(SPIRV "${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME}.spv")
  
  # Add a custom command to compile the shader
  add_custom_command(
    OUTPUT ${SPIRV}  # The output file
    COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}  # The compile command
    DEPENDS ${GLSL}  # This shader output depends on its source file
    COMMENT "Compiling shader ${FILE_NAME}"  # Message to display during build
  )
  
  # Add the output file to a list of all compiled shaders
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

# Create a custom target that depends on all compiled shaders
# The ALL keyword ensures this target is built by default
add_custom_target(Shaders ALL DEPENDS ${SPIRV_BINARY_FILES})