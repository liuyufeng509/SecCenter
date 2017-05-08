#include "kernalparampage.h"
#include "ui_kernalparampage.h"

KernalParamPage::KernalParamPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KernalParamPage)
{
    ui->setupUi(this);
}

KernalParamPage::~KernalParamPage()
{
    delete ui;
}
