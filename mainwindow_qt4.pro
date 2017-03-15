#-------------------------------------------------
#
# Project created by QtCreator 2017-02-27T09:53:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mainwindow_qt4
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        common.cpp \
       AUD/tabauditpage.cpp \
        DAC_ACL/alcdialog.cpp \
        DAC_ACL/commtab.cpp \
        INDEX/indexwidget.cpp \
        SEC/tabsecritypage.cpp \
        SYS/tabsyspage.cpp \
    SYS/userinfodialog.cpp \
    SYS/logmainwindow.cpp

HEADERS  += mainwindow.h \
               common.h \
               DAC_ACL/DacDemo.h \
            AUD/tabauditpage.h \
            DAC_ACL/alcdialog.h \
            DAC_ACL/commtab.h \
            INDEX/indexwidget.h \
            SEC/tabsecritypage.h \
            SYS/tabsyspage.h \
    SYS/userinfodialog.h \
    SYS/logmainwindow.h

FORMS    += mainwindow.ui \
     AUD/tabauditpage.ui \
    DAC_ACL/alcdialog.ui \
    DAC_ACL/commtab.ui \
    INDEX/indexwidget.ui \
    SEC/tabsecritypage.ui \
    SYS/tabsyspage.ui \
    SYS/userinfodialog.ui \
    SYS/logmainwindow.ui


TRANSLATIONS += securityCenter.ts

INCLUDEPATH += DAC_ACL \
                    AUD \
                    INDEX \
                    SEC \
                    SYS

RESOURCES += \
    mainwindow.qrc \
    rc/rc.qrc

DISTFILES += \
    testQtCreator_english.qm \
    testQtCreator_english.ts \

unix:!macx: LIBS +=  -lDacDemo


LIBS+= -lglib-2.0 -lgtop-2.0

INCLUDEPATH += /usr/include/glib-2.0 \
                          /usr/lib64/glib-2.0/include   \
                         /usr/include/libgtop-2.0   \

