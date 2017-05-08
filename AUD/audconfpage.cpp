#include "audconfpage.h"
#include "ui_audconfpage.h"

AudConfPage::AudConfPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudConfPage)
{
    ui->setupUi(this);
}

AudConfPage::~AudConfPage()
{
    delete ui;
}
