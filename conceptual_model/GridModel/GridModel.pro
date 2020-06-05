QT += widgets
LIBS += -lgsl -lgslcblas

QMAKE_CXXFLAGS += -std=c++11

HEADERS += \
    gridwindow.h \
    aggregationagent.h \
    agentfactory.h \
    ngaggagent.h \
    eeaggagent.h \
    agent.h \
    world.h \
    randomaggagent.h \
    erosion.h \
    infectiousagent.h \
    mazeworld.h \
    entity.h \
    infection.h

SOURCES += \
    gridwindow.cpp \
    aggregationagent.cpp \
    agentfactory.cpp \
    ngaggagent.cpp \
    eeaggagent.cpp \
    main.cpp \
    agent.cpp \
    world.cpp \
    randomaggagent.cpp \
    erosion.cpp \
    infectiousagent.cpp \
    mazeworld.cpp \
    entity.cpp \
    infection.cpp
