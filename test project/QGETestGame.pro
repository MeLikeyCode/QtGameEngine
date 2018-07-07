include(../qge/qge.pri)
INCLUDEPATH += ../

TARGET = qgeTestGame # this is what our executable is called
TEMPLATE = app # make an executable (not static or dynamic lib)

SOURCES += \
    main.cpp \
    MyEventHandler.cpp \
    SpiderCreator.cpp \

HEADERS += \
    MyEventHandler.h \
    SpiderCreator.h \
