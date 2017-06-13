#include "helppage.h"
#include "ui_helppage.h"
#include <QHBoxLayout>
HelpPage::HelpPage(ROLE curRole,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelpPage)
{
    ui->setupUi(this);
    QString urlName="";
    switch(curRole)
    {
    case ROOT:
        break;
    case SECADMIN:
        urlName="/etc/sysctl.d/helpdoc/helper-secadm_a.html";
        break;
    case SYSADMIN:
        urlName="/etc/sysctl.d/helpdoc/helper-sysadm_a.html";
        break;
    case AUDADMIN:
        urlName="/etc/sysctl.d/helpdoc/helper-audadm_a.html";
        break;
    default:
        break;
    }
    ui->muluBrowser->setOpenLinks(false);
    ui->muluBrowser->setSource(urlName);
    connect(ui->muluBrowser, SIGNAL(anchorClicked(const QUrl &)),ui->contentBrowser, SLOT(setSource(QUrl)));
}

HelpPage::~HelpPage()
{
    delete ui;
}
