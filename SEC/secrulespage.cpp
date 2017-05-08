#include "secrulespage.h"
#include "ui_secrulespage.h"

SecRulesPage::SecRulesPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SecRulesPage)
{
    ui->setupUi(this);
}

SecRulesPage::~SecRulesPage()
{
    delete ui;
}
