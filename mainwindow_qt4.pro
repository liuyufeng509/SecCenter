#-------------------------------------------------
#
# Project created by QtCreator 2017-02-27T09:53:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nfs-security-center
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
    SYS/logmainwindow.cpp \
    DAC_ACL/DacDemo.cpp \
    qreadconfig.cpp \
    AUD/querydialog.cpp \
    AUD/reportdialog.cpp \
    AUD/ruledialog.cpp \
    AUD/configdialog.cpp \
    SYS/kernalparammanager.cpp

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
    SYS/logmainwindow.h \
    qreadconfig.h \
    AUD/querydialog.h \
    AUD/reportdialog.h \
    AUD/ruledialog.h \
    AUD/configdialog.h \
    SYS/kernalparammanager.h

FORMS    += mainwindow.ui \
     AUD/tabauditpage.ui \
    DAC_ACL/alcdialog.ui \
    DAC_ACL/commtab.ui \
    INDEX/indexwidget.ui \
    SEC/tabsecritypage.ui \
    SYS/tabsyspage.ui \
    SYS/userinfodialog.ui \
    SYS/logmainwindow.ui \
    AUD/querydialog.ui \
    AUD/reportdialog.ui \
    AUD/ruledialog.ui \
    AUD/configdialog.ui \
    SYS/kernalparammanager.ui


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


#LIBS+= -lglib-2.0 -lgtop-2.0

#INCLUDEPATH += /usr/include/glib-2.0 \
#                          /usr/lib64/glib-2.0/include   \
#                         /usr/include/libgtop-2.0   \

