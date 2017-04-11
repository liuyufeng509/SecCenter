#include "kernalparammanager.h"
#include "ui_kernalparammanager.h"

KernParmMngWidget::KernParmMngWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KernalParamManager)
{
    ui->setupUi(this);
}

KernParmMngWidget::~KernParmMngWidget()
{
    delete ui;
}
