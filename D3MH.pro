TARGET = D3MH
TEMPLATE = app

DESTDIR = ../bin

CONFIG += windows c++11
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lGdiplus -lgdi32

SOURCES += main.cpp\
    engine/engine.cpp \
    process/memoryreader.cpp \
    process/helper.cpp \
    engine/navmesh.cpp

HEADERS  += \
	engine/engine.h \
    engine/objectmanager.h \
    engine/structs.h \
    engine/types.h \
    engine/localdata.h \
    engine/scene.h \
    engine/actor.h \
    engine/sno.h \
    engine/snoenum.h \
    process/helper.h \
    process/memoryreader.h \
    engine/navmesh.h \
    process/pointer.h \
    engine/levelarea.h \
    engine/marker.h \
    engine/ui.h

RC_FILE = default.rc
