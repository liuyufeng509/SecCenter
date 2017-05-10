#include "customruledialog.h"
#include "ui_customruledialog.h"

CustomRuleDialog::CustomRuleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomRuleDialog)
{
    ui->setupUi(this);
}

CustomRuleDialog::~CustomRuleDialog()
{
    delete ui;
}
