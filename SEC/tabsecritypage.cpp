#include "tabsecritypage.h"
#include "ui_tabsecritypage.h"

TabSecrityPage::TabSecrityPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabSecrityPage)
{
    ui->setupUi(this);
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
    ui->listWidget->setCurrentRow(0);

    get_services();

    ui->comboBox->addItems(services);

    QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");   //^[1-9][0-9]*$ 和 ^[1-9]{1}[/d]*$

                                                //上面的正则表达式表示只能输入大于0的正整数

    ui->tmsLineEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->secLineEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->tmsLineEdit->setText("3");
    ui->secLineEdit->setText("30");
    ui->comboBox->setCurrentIndex(0);

}

void TabSecrityPage::get_services()
{
    //enhanced-trylock  -l
    QString cmd = "enhanced-trylock  -l;echo $?";
    cmd = GetCmdRes(cmd);
    services<<"login"<<"ssh"<<"gdm";

}

TabSecrityPage::~TabSecrityPage()
{
    delete ui;
}

void TabSecrityPage::on_setusButton_clicked()
{
    QString cmd = "enhanced -d "+ui->tmsLineEdit->text() + (ui->evrCheckBox->isChecked()? " -e -r ":" -u ")  + ui->secLineEdit->text()+ " -s " + ui->comboBox->currentText();

    //call interface
    //GetCmdRes(cmd);
}

void TabSecrityPage::on_unlockButton_clicked()
{

}
