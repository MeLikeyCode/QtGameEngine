include(qge/qge.pri)

TARGET = qgeTestGame # this is what our executable is called
TEMPLATE = app # make an executable (not static or dynamic lib)

INCLUDEPATH += "C:/Program Files (x86)/Windows Kits/10/Include/10.0.10240.0/ucrt" # search this directory for headers
LIBS += -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.10240.0/ucrt/x64" # search this directory for .lib files (static libraries)

SOURCES += \
    main.cpp \
    MyEventHandler.cpp \
    SpiderCreator.cpp \

HEADERS += \
    MyEventHandler.h \
    SpiderCreator.h \
