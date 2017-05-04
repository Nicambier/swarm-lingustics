cmake_minimum_required(VERSION 2.6)
project(language_game)

# Check for Lua and add include path
find_package(Lua REQUIRED)
include_directories(${LUA_INCLUDE_DIR})

# Check for ARGoS3
find_package(PkgConfig)
pkg_check_modules(ARGOS REQUIRED argos3_simulator)

# Add ARGoS paths
include_directories(${ARGOS_INCLUDE_DIRS})
link_directories(${ARGOS_LIBRARY_DIRS})

# Add qt
find_package( Qt4 REQUIRED )
include_directories("/usr/include/qt4/")
INCLUDE_DIRECTORIES(${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTSVG_INCLUDE_DIR}
                    ${QT_QTXML_INCLUDE_DIR} ${QT_QTOPENGL_INCLUDE_DIR} ${OPENGL_INCLUDE_DIRS})


# Create the loop function library
add_library(language_game SHARED language_game.h language_game.cpp)
target_link_libraries(language_game
  ${ARGOS_LIBRARIES}
  ${LUA_LIBRARIES}
  argos3plugin_simulator_entities)
  
  # Create the visualisation function library
add_library(word_loop_functions MODULE 
  word_qtuser_functions.h
  word_qtuser_functions.cpp)

target_link_libraries(word_loop_functions
  argos3core_simulator
  argos3plugin_simulator_entities
  argos3plugin_simulator_footbot
  argos3plugin_simulator_qtopengl
${QT_LIBRARIES} ${GLUT_LIBRARY} ${OPENGL_LIBRARY})
