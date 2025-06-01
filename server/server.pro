QT = core network

CONFIG += c++17 cmdline

COMMON_DIR = ../common

SOURCES += \
        main.cpp \
        randomUniqueArray.cpp \
        requestHandler.cpp \
        udpServer.cpp \
        udpReceiver.cpp \
        udpSender.cpp \
        $$COMMON_DIR/tools.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    randomUniqueArray.hpp \
    requestHandler.hpp \
    udpReceiver.hpp \
    udpSender.hpp \
    $$COMMON_DIR/tools.hpp \
    $$COMMON_DIR/constants.hpp \
    udpServer.hpp

INCLUDEPATH += ../common
