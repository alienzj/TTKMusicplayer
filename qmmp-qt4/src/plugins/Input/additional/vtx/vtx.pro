include($$PWD/../additional.pri)
include($$PWD/../common/common.pri)

HEADERS += decodervtxfactory.h \
           vtxmetadatamodel.h \
           decoder_vtx.h \
           vtxhelper.h \
           ayemu.h \
           ayemu_8912.h \
           ayemu_vtxfile.h
    
SOURCES += decoder_vtx.cpp \
           vtxmetadatamodel.cpp \
           decodervtxfactory.cpp \
           vtxhelper.cpp \
           ay8912.c \
           lh5dec.c \
           vtxfile.c

DESTDIR = $$PLUGINS_PREFIX/Input
TARGET = vtx

win32 {
HEADERS += mman.h

SOURCES += mman.c
}

unix{
    QMAKE_CLEAN = $$DESTDIR/$$DESTDIR/libvtx.so
}