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
    SYS/kernalparammanager.cpp \
    SEC/ukeydialog.cpp \
    mainfunclass.cpp \
    SYS/sysfunclass.cpp \
    waitdialog.cpp \
    SEC/secfunclass.cpp \
    mytitlebar.cpp \
    INDEX/indexfunclass.cpp \
    AUD/audfunclass.cpp \
    SEC/sectabwidget.cpp \
    SYS/systabwidget.cpp \
    AUD/audtabwidget.cpp \
    Help/helppage.cpp \
    SEC/usermgrpage.cpp \
    SEC/secstatuspage.cpp \
    SEC/sectagpage.cpp \
    SEC/secrulespage.cpp \
    SEC/svrctrlpage.cpp \
    SYS/sysusermgrpage.cpp \
    SYS/svrmgrpage.cpp \
    SYS/syslogpage.cpp \
    AUD/audsvrctlpage.cpp \
    AUD/audinfopage.cpp \
    AUD/audrulepage.cpp \
    AUD/audkernrulepage.cpp \
    AUD/audconfpage.cpp \
    AUD/filerulesdialog.cpp \
    AUD/syscalldialog.cpp \
    AUD/customruledialog.cpp \
    SYS/ttychoosedialog.cpp \
    SYS/sysadmform.cpp

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
    SYS/kernalparammanager.h \
    SEC/ukeydialog.h \
    mainfunclass.h \
    datadefine.h \
    SYS/sysfunclass.h \
    waitdialog.h \
    SEC/secfunclass.h \
    mytitlebar.h \
    INDEX/indexfunclass.h \
    AUD/audfunclass.h \
    SEC/sectabwidget.h \
    SYS/systabwidget.h \
    Help/helppage.h \
    SEC/usermgrpage.h \
    SEC/secstatuspage.h \
    SEC/sectagpage.h \
    SEC/secrulespage.h \
    SEC/svrctrlpage.h \
    SYS/sysusermgrpage.h \
    SYS/svrmgrpage.h \
    SYS/syslogpage.h \
    AUD/audsvrctlpage.h \
    AUD/audinfopage.h \
    AUD/audrulepage.h \
    AUD/audkernrulepage.h \
    AUD/audtabwidget.h \
    AUD/audconfpage.h \
    AUD/filerulesdialog.h \
    AUD/syscalldialog.h \
    AUD/customruledialog.h \
    SYS/ttychoosedialog.h \
    SYS/sysadmform.h

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
    SYS/kernalparammanager.ui \
    SEC/ukeydialog.ui \
    Help/helppage.ui \
    SEC/usermgrpage.ui \
    SEC/secstatuspage.ui \
    SEC/sectagpage.ui \
    SEC/secrulespage.ui \
    SEC/svrctrlpage.ui \
    SYS/sysusermgrpage.ui \
    SYS/svrmgrpage.ui \
    SYS/syslogpage.ui \
    AUD/audsvrctlpage.ui \
    AUD/audinfopage.ui \
    AUD/audrulepage.ui \
    AUD/audkernrulepage.ui \
    AUD/audconfpage.ui \
    AUD/filerulesdialog.ui \
    AUD/syscalldialog.ui \
    AUD/customruledialog.ui \
    SYS/ttychoosedialog.ui \
    SYS/sysadmform.ui


TRANSLATIONS += securityCenter.ts

INCLUDEPATH += DAC_ACL \
                    AUD \
                    INDEX \
                    SEC \
                    SYS \
                    Help

RESOURCES += \
    rc/rc.qrc \
    mainwindow.qrc

DISTFILES += \
    testQtCreator_english.qm \
    testQtCreator_english.ts \

CONFIG += thread
#LIBS+= -lglib-2.0 -lgtop-2.0

#INCLUDEPATH += /usr/include/glib-2.0 \
#                          /usr/lib64/glib-2.0/include   \
#                         /usr/include/libgtop-2.0   \

