cmake_minimum_required ( VERSION 2.6 )

list(APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

include_directories(
	$(CMAKE_SOURCE_DIR)/src/overlaptester
)

add_library(overlaptester
	CCircle.cpp
	CRectangle.cpp
	CVector2.cpp
	COverlapTester.cpp
)

