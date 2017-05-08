#include "audrulepage.h"
#include "ui_audrulepage.h"

AudRulePage::AudRulePage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AudRulePage)
{
    ui->setupUi(this);
}

AudRulePage::~AudRulePage()
{
    delete ui;
}
