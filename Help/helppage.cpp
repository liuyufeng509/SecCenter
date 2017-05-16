#include "helppage.h"
#include "ui_helppage.h"
#include <QHBoxLayout>
HelpPage::HelpPage(ROLE curRole,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelpPage)
{
    ui->setupUi(this);
    switch(curRole)
    {
    case ROOT:
        break;
    case SECADMIN:
        break;
    case SYSADMIN:
        break;
    case AUDADMIN:
        break;
    default:
        break;
    }
    QString urlName("/root/Program/SecCenter/helpdoc/frame_a.html");

    ui->muluBrowser->setOpenLinks(false);
    ui->muluBrowser->setSource(urlName);
    ui->muluBrowser->setStyleSheet("");
    ui->contentBrowser->setStyleSheet("");
    connect(ui->muluBrowser, SIGNAL(anchorClicked(const QUrl &)),ui->contentBrowser, SLOT(setSource(QUrl)));
}

HelpPage::~HelpPage()
{
    delete ui;
}
