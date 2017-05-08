#include "sectagpage.h"
#include "ui_sectagpage.h"

SecTagPage::SecTagPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SecTagPage)
{
    ui->setupUi(this);
}

SecTagPage::~SecTagPage()
{
    delete ui;
}
