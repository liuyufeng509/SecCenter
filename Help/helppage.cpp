#include "helppage.h"
#include "ui_helppage.h"
#include <QHBoxLayout>
HelpPage::HelpPage(ROLE curRole,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HelpPage)
{
    ui->setupUi(this);
    webView = new QWebView(this);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(webView);
    ui->groupBox->setLayout(layout);

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
    QString urlName("/root/Program/SecCenter/helpdoc/help.html");
    QUrl url = QUrl::fromUserInput(urlName);
    webView->load(url);

}

HelpPage::~HelpPage()
{
    delete ui;
}
