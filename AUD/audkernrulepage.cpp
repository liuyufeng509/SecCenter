#include "audkernrulepage.h"
#include "ui_audkernrulepage.h"

AudKernRulePage::AudKernRulePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudKernRulePage)
{
    ui->setupUi(this);
}

AudKernRulePage::~AudKernRulePage()
{
    delete ui;
}
