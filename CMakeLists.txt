cmake_minimum_required ( VERSION 2.6 )

# Используем утилиту PkgConfig (pkg-config)
FIND_PACKAGE(PkgConfig)
PKG_CHECK_MODULES(SDL2 sdl2)
PKG_CHECK_MODULES(SDL2_IMG SDL2_image)

# Укажите здесь имя вашего проекта.
# Учитываете установку переменных 
# <project_name>_BINARY_DIR и  <project_name>_SOURCE_DIR
# в значение имени каталога с проектом.
project ( happy_orc )

set ( SRC_DIR ${PROJECT_SOURCE_DIR}/src )

set (PROJECT_INCLUDE_DIRS
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMG_INCLUDE_DIRS}
)
set (PROJECT_LIBS
    ${SDL2_LIBRARIES}
    ${SDL2_IMG_LIBRARIES}
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
	${SRC_DIR}/CGame.cpp)
ADD_EXECUTABLE(${APP_NAME} ${SRC_LIST})
TARGET_LINK_LIBRARIES(${APP_NAME} ${PROJECT_LIBS})


