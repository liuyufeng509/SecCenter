#include "audsvrctlpage.h"
#include "ui_audsvrctlpage.h"

AudSvrCtlPage::AudSvrCtlPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudSvrCtlPage)
{
    ui->setupUi(this);
}

AudSvrCtlPage::~AudSvrCtlPage()
{
    delete ui;
}
