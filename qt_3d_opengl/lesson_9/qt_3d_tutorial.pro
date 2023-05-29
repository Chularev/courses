QT       += core gui openglwidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camera3d.cpp \
    group3d.cpp \
    main.cpp \
    material.cpp \
    materiallibrary.cpp \
    objectengine3d.cpp \
    simpleobject3d.cpp \
    skybox.cpp \
    widget.cpp

HEADERS += \
    camera3d.h \
    group3d.h \
    material.h \
    materiallibrary.h \
    objectengine3d.h \
    simpleobject3d.h \
    skybox.h \
    transformational.h \
    widget.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

RESOURCES += \
    models.qrc \
    shaders.qrc \
    texture.qrc

win32-g* {
    LIBS += -lopengl32
}

win32-msvc* {
    LIBS += opengl32.lib
}

