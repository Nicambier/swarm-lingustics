add_library(footbot_pace SHARED footbot_pace.h footbot_pace.cpp)
target_link_libraries(footbot_pace
  argos3core_simulator
  argos3plugin_simulator_footbot
  argos3plugin_simulator_genericrobot)
