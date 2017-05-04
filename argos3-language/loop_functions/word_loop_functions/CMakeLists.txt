add_library(word_loop_functions MODULE 
  word_qtuser_functions.h
  word_qtuser_functions.cpp)

target_link_libraries(word_loop_functions
  argos3core_simulator
  argos3plugin_simulator_entities
  argos3plugin_simulator_footbot
  argos3plugin_simulator_qtopengl
  ${ARGOS_QTOPENGL_LIBRARIES})
