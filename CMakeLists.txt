cmake_minimum_required(VERSION 3.1)
project(irmf)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ./bin)

# source code
set(SOURCES
	dllmain.cpp
	irmf.cpp

# libraries
	libs/json11/json11.cpp
	libs/pugixml/src/pugixml.cpp
	libs/imgui/imgui_draw.cpp
	libs/imgui/imgui_widgets.cpp
	libs/imgui/imgui.cpp
)

# cmake toolchain
if(CMAKE_TOOLCHAIN_FILE)
    include(${CMAKE_TOOLCHAIN_FILE})
endif(CMAKE_TOOLCHAIN_FILE)

# openssl
find_package(OpenSSL REQUIRED)

# create executable
add_library(irmf SHARED ${SOURCES})

set_target_properties(irmf PROPERTIES OUTPUT_NAME "plugin")
set_target_properties(irmf PROPERTIES PREFIX "")

# include directories
target_include_directories(irmf PRIVATE ${OPENSSL_INCLUDE_DIR} libs inc)

target_link_libraries(irmf ${OPENSSL_LIBRARIES})

if (NOT MSVC)
	target_compile_options(irmf PRIVATE -Wno-narrowing)
endif()