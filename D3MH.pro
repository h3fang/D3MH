TARGET = D3MH
TEMPLATE = app

DESTDIR = ../bin

CONFIG += windows c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += ../include

LIBS += -L../lib -lgdi32 -lopengl32 -lglu32 -lglew32s -lfreetype

SOURCES += main.cpp\
    engine/engine.cpp \
    process/memoryreader.cpp \
    process/helper.cpp \
    engine/navmesh.cpp \
    minimapoverlay.cpp \
    layered/layeredwindow.cpp \
    layered/textrenderer.cpp

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
    engine/ui.h \
    layered/layeredbitmap.h \
    layered/layeredwindow.h \
    layered/layeredwindowinfo.h \
    minimapoverlay.h \
    layered/textrenderer.h

RC_FILE = default.rc
