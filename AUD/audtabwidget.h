#ifndef AUDTABWIDGET_H
#define AUDTABWIDGET_H
#include <QTabWidget>
#include "indexwidget.h"
#include "common.h"
#include "helppage.h"
#include "audsvrctlpage.h"
#include "audinfopage.h"
#include "audrulepage.h"
#include "audkernrulepage.h"
#include "audconfpage.h"
class AudTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    AudTabWidget(QWidget *parent=0);
    ~AudTabWidget();
void UpdateIndex();
private:
    IndexWidget *indexPage;
    HelpPage *helpPage;
    AudSvrCtlPage * audSvrCtlPage;
    AudInfoPage *audInfoPage;
    AudRulePage *audRulePage;
    AudKernRulePage *audKernRulePage;
    AudConfPage *audConfPage;
};

#endif // AUDTABWIDGET_H
