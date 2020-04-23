QT += quick multimedia

CONFIG += c++11 sdk_no_version_check

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += src/main.cpp \
    src/music.cpp \
    src/myglitem.cpp \
    src/glbody.cpp \
    src/glbodygroup.cpp \
    src/glcolorrgba.cpp \
    src/gldisc.cpp \
    src/glesrenderer.cpp \
    src/glfield.cpp \
    src/glitem.cpp \
    src/glmouseray.cpp \
    src/glmultiplebody.cpp \
    src/glpoint.cpp \
    src/shaderdebugger.cpp

RESOURCES += \
    icons.qrc \
    images.qrc \
    models.qrc \
    music.qrc \
    qml.qrc \
    shaders.qrc \
    textures.qrc \
    translations.qrc

TRANSLATIONS += translations/testmm2019_de.ts

win32 { LIBS += -lopengl32}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

HEADERS += \
    src/music.h \
    src/myglitem.h \
    src/glbody.h \
    src/glbodygroup.h \
    src/glcolorrgba.h \
    src/gldefines.h \
    src/gldisc.h \
    src/glesrenderer.h \
    src/glfield.h \
    src/glitem.h \
    src/glmouseray.h \
    src/glmultiplebody.h \
    src/glpoint.h \
    src/shaderdebugger.h
