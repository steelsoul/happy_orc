cmake_minimum_required ( VERSION 2.6 )

FIND_PACKAGE(PkgConfig)
PKG_CHECK_MODULES(SDL2 sdl2)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

if (WIN32)
    if (DEFINED ENV{SDL2_IncludePath})
        message(STATUS " SDL2 Includes are: " $ENV{SDL2_IncludePath})
        set (SDL2_INCLUDE_DIRS $ENV{SDL2_IncludePath})
    endif(DEFINED ENV{SDL2_IncludePath})
endif(WIN32)

include_directories(
	${SDL2_INCLUDE_DIRS}
	$(CMAKE_SOURCE_DIR)/src/overlaptester
)

add_library(overlaptester
	CCircle.cpp
	CRectangle.cpp
	CVector2.cpp
	COverlapTester.cpp
)

