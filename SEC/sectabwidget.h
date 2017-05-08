#ifndef SECTABWIDGET_H
#define SECTABWIDGET_H
#include <QTabWidget>
#include "indexwidget.h"
#include "helppage.h"
#include "common.h"
#include "usermgrpage.h"
#include "secstatuspage.h"
#include "sectagpage.h"
#include "secrulespage.h"
#include "svrctrlpage.h"
class SecTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    SecTabWidget(QWidget *parent=0);
    ~SecTabWidget();
    void UpdateIndex();

private:
    IndexWidget *indexPage;
    HelpPage *helpPage;
    SecStatusPage *secStaPage;
    UserMgrPage *usrMgrPage;
    SecTagPage *secTagPage;
    SecRulesPage *secRulesPage;
    SvrCtrlPage * svrCtrlPage;
};

#endif // SECTABWIDGET_H
