cmake_minimum_required ( VERSION 2.6 )

# Используем утилиту PkgConfig (pkg-config)
FIND_PACKAGE(PkgConfig)
PKG_CHECK_MODULES(SDL2 sdl2)
PKG_CHECK_MODULES(SDL2_IMG SDL2_image)
PKG_CHECK_MODULES(SDL2_TTF SDL2_ttf)

# Укажите здесь имя вашего проекта.
# Учитываете установку переменных 
# <project_name>_BINARY_DIR и  <project_name>_SOURCE_DIR
# в значение имени каталога с проектом.
project ( happy_orc )

add_subdirectory("${PROJECT_SOURCE_DIR}/src/overlaptester")

set ( SRC_DIR ${PROJECT_SOURCE_DIR}/src )

if (WIN32)
    if (DEFINED ENV{SDL2_IncludePath})
        message(STATUS " SDL2 Includes are: " $ENV{SDL2_IncludePath})
        set (SDL2_INCLUDE_DIRS $ENV{SDL2_IncludePath})
    endif(DEFINED ENV{SDL2_IncludePath})
    
    if (DEFINED ENV{SDL2_x86_LibrariesPath})
        message(STATUS "SDL2 Libraries path: " $ENV{SDL2_x86_LibrariesPath} )
        include_directories($ENV{SDL2_x86_LibrariesPath})
    endif(DEFINED ENV{SDL2_x86_LibrariesPath})
    
    set(CMAKE_PREFIX_PATH $ENV{SDL2_x86_LibrariesPath})
    
    find_library(SDL2_LIBRARIES 
        NAME SDL2.lib
        PATHS $ENV{SDL2_x86_LibrariesPath})
    message(STATUS "SDL2 LIBRARIES: " ${SDL2_LIBRARIES} )
            
    find_library(SDL2_IMG_LIBRARIES 
        NAME SDL2_image.lib
        PATHS $ENV{SDL2_x86_LibrariesPath})
    message(STATUS "SDL2 IMAGE LIBRARIES: " ${SDL2_IMG_LIBRARIES} )

    find_library(SDL2_TTF_LIBRARIES
        NAME SDL2_ttf.lib
        PATHS $ENV{SDL2_x86_LibrariesPath})
    message(STATUS "SDL2 TTF LIBRARIES: " ${SDL2_TTF_LIBRARIES} )
endif(WIN32)

set (PROJECT_INCLUDE_DIRS
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMG_INCLUDE_DIRS}
	${SDL2_TTF_INCLUDE_DIRS}
)
set (PROJECT_LIBS
    ${SDL2_LIBRARIES}
    ${SDL2_IMG_LIBRARIES}
	${SDL2_TTF_LIBRARIES}
    overlaptester
)

include_directories(${PROJECT_INCLUDE_DIRS})

# Укажите вместо my-project имя своего приложения
# - имя исполняемого файла
set ( APP_NAME ho )

# Управление типом сборки через задание значения
# предопределенной переменной CMAKE_BUILD_TYPE
# Возможные значения:
# DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL
# выполняется установка через скрипты do_release.sh или do_debug.sh
list( APPEND CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

SET(SRC_LIST 
	${SRC_DIR}/game.cpp
	${SRC_DIR}/CBaseSprite.cpp
	${SRC_DIR}/COrcSprite.cpp
	${SRC_DIR}/CHamSprite.cpp
	${SRC_DIR}/CGame.cpp
	${SRC_DIR}/CMainDispatcher.cpp
	${SRC_DIR}/CBaseScreen.cpp
	${SRC_DIR}/CMenu.cpp
	${SRC_DIR}/COptions.cpp)
	
ADD_EXECUTABLE(${APP_NAME} ${SRC_LIST})
TARGET_LINK_LIBRARIES(${APP_NAME} ${PROJECT_LIBS})


