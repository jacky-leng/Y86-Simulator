QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    src/CPU.cpp \
    src/control.cpp \
    src/decode.cpp \
    src/execute.cpp \
    src/fetch.cpp \
    src/forward.cpp \
    src/memory.cpp \
    src/memory_access.cpp \
    src/pass_params.cpp \
    src/write_back.cpp \
    widget.cpp

HEADERS += \
    include/CPU.h \
    include/fifo_map.hpp \
    include/httplib.h \
    include/json.hpp \
    include/memory.h \
    include/utils.h \
    widget.h

FORMS += \
    widget.ui

INCLUDEPATH+= ./include

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    11.qrc
