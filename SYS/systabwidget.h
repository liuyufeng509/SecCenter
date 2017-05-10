#ifndef SYSTABWIDGET_H
#define SYSTABWIDGET_H
#include "indexwidget.h"
#include <QTabWidget>
#include "common.h"
#include "helppage.h"
#include "sysusermgrpage.h"
#include "svrmgrpage.h"
#include "syslogpage.h"
#include "kernalparampage.h"
class SysTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    SysTabWidget(QWidget *parent=0);
    ~SysTabWidget();
    void UpdateIndex();

public slots:
    void tabChanged(int index);
private:
    IndexWidget *indexPage;
    HelpPage *helpPage;
    SysUserMgrPage *userMgrPage;
    SvrMgrPage *svrMgrPage;
    SysLogPage *sysLogPage;
    KernalParamPage *kernalPamPage;
};

#endif // SYSTABWIDGET_H
