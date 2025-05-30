QT = core network

CONFIG += c++17 cmdline

COMMON_DIR = ../common

SOURCES += \
        client.cpp \
        main.cpp \
        responseHandler.cpp \
        $$COMMON_DIR/tools.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    client.hpp \
    responseHandler.hpp \
    $$COMMON_DIR/tools.hpp \
    $$COMMON_DIR/constants.hpp

INCLUDEPATH += $$COMMON_DIR
