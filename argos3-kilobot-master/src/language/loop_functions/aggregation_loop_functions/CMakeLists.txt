add_library(aggregation_loop_functions MODULE 
  aggregation.h
  aggregation.cpp)

target_link_libraries(aggregation_loop_functions
  argos3core_simulator
  argos3plugin_simulator_entities
  argos3plugin_simulator_kilolib
  argos3plugin_simulator_kilobot)
