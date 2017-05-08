#include "audinfopage.h"
#include "ui_audinfopage.h"

AudInfoPage::AudInfoPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudInfoPage)
{
    ui->setupUi(this);
}

AudInfoPage::~AudInfoPage()
{
    delete ui;
}
