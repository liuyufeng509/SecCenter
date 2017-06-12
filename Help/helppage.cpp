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

        ui->muluBrowser->setOpenLinks(false);
        ui->muluBrowser->setSource(urlName);
        break;
    case SYSADMIN:
        break;
    case AUDADMIN:
        break;
    default:
        break;
    }

    connect(ui->muluBrowser, SIGNAL(anchorClicked(const QUrl &)),ui->contentBrowser, SLOT(setSource(QUrl)));
}

HelpPage::~HelpPage()
{
    delete ui;
}
