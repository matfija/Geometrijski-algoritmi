QT += core gui opengl charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

LIBS += -lglut -lGLU

CONFIG += c++14

INCLUDEPATH += \
    $$PWD/algoritmi_sa_vezbi \
    $$PWD/algoritmi_studentski_projekti

SOURCES += \
    algoritambaza.cpp \
    algoritmi_sa_vezbi/ga00_demoiscrtavanja.cpp \
    algoritmi_sa_vezbi/ga01_brisucaprava.cpp \
    algoritmi_sa_vezbi/ga02_3discrtavanje.cpp \
    algoritmi_studentski_projekti/ga06_presekPravougaonika.cpp \
    animacijanit.cpp \
    main.cpp \
    mainwindow.cpp \
    oblastcrtanja.cpp \
    oblastcrtanjaopengl.cpp \
    timemeasurementthread.cpp

HEADERS += \
    algoritambaza.h \
    algoritmi_sa_vezbi/ga00_demoiscrtavanja.h \
    algoritmi_sa_vezbi/ga01_brisucaprava.h \
    algoritmi_sa_vezbi/ga02_3discrtavanje.h \
    algoritmi_studentski_projekti/ga06_presekPravougaonika.h \
    animacijanit.h \
    config.h \
    mainwindow.h \
    oblastcrtanja.h \
    oblastcrtanjaopengl.h \
    timemeasurementthread.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
