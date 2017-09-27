add_library(footbot_stopNG SHARED footbot_stopNG.h footbot_stopNG.cpp)
target_link_libraries(footbot_stopNG
  footbot_aggregation_NG
  argos3core_simulator
  argos3plugin_simulator_footbot
  argos3plugin_simulator_genericrobot)
