if(ARGOS_BUILD_FOR_SIMULATOR)
  include_directories(${CMAKE_SOURCE_DIR}/plugins/robots/kilobot/control_interface)

  #
  # Naming game
  #
  add_executable(naming_game naming_game.c)
  target_link_libraries(naming_game argos3plugin_simulator_kilolib)
endif(ARGOS_BUILD_FOR_SIMULATOR)
