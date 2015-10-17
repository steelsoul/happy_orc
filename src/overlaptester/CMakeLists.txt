cmake_minimum_required ( VERSION 2.6 )

FIND_PACKAGE(PkgConfig)
PKG_CHECK_MODULES(SDL2 sdl2)

list(APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

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

