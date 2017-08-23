add_library(visualise_functions MODULE 
  visualise.h
  visualise.cpp)

target_link_libraries(visualise_functions
  argos3core_simulator
  argos3plugin_simulator_entities
  argos3plugin_simulator_footbot)
