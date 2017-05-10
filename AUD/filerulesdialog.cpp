#include "filerulesdialog.h"
#include "ui_filerulesdialog.h"

FileRulesDialog::FileRulesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileRulesDialog)
{
    ui->setupUi(this);
}

FileRulesDialog::~FileRulesDialog()
{
    delete ui;
}
