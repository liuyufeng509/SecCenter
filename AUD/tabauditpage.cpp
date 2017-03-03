#include "tabauditpage.h"
#include "ui_tabauditpage.h"

TabAuditPage::TabAuditPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabAuditPage)
{
    ui->setupUi(this);
}

TabAuditPage::~TabAuditPage()
{
    delete ui;
}
