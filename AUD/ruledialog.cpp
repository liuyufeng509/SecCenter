#include "ruledialog.h"
#include "ui_ruledialog.h"

RuleDialog::RuleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RuleDialog)
{
    ui->setupUi(this);
}

RuleDialog::~RuleDialog()
{
    delete ui;
}
