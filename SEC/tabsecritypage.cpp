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

    QRegExp regExp("[1-9][0-9]{0,2}");   //^[1-9][0-9]*$ 和 ^[1-9]{1}[/d]*$

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
    if(!get_trylock_services(services))
    {
        services<<"login"<<"gdm"<<"ssh";
    }

}

TabSecrityPage::~TabSecrityPage()
{
    delete ui;
}

void TabSecrityPage::on_setusButton_clicked()
{
    QString cmd = "enhanced-trylock -d "+ui->tmsLineEdit->text() + (ui->evrCheckBox->isChecked()? " -e -r ":" -u ")
            + ui->secLineEdit->text()+ " -s " + ui->comboBox->currentText() + "; echo $?";

    if(trylock_service(cmd))
    {
        QMessageBox::information(this, tr("提示"), tr("操作成功"));
    }else
        QMessageBox::warning(this, tr("提示"), tr("操作失败"));
}

void TabSecrityPage::on_unlockButton_clicked()
{
    if(unlock_all_users())
    {
        QMessageBox::information(this, tr("提示"), tr("操作成功"));
    }else
        QMessageBox::warning(this, tr("提示"), tr("操作失败"));
}

void TabSecrityPage::on_setPwButton_clicked()
{
    QString cmd = "enhanced-passwd "+
            (ui->minlenEdit->text().toInt()!=0? ("-m "+ui->minlenEdit->text()+ " "): " ")+
            (ui->dlenEdit->text().toInt()!=0? ("-d -"+ui->dlenEdit->text()+ " "): " ")+
            (ui->uplenEdit->text().toInt()!=0? ("-u -"+ui->uplenEdit->text()+ " "): " ")+
            (ui->lowlenEdit->text().toInt()!=0? ("-l -"+ui->lowlenEdit->text()+ " "): " ")+
            (ui->othlenEdit->text().toInt()!=0? ("-o -"+ui->othlenEdit->text()+ " "): " ")+
            (ui->evrCheckBox_2->isChecked()? "-e ": " ")+ " ; echo $?";

    if(set_pwd_rule(cmd))
    {
        QMessageBox::information(this, tr("提示"), tr("操作成功"));
    }else
        QMessageBox::warning(this, tr("提示"), tr("操作失败"));
}


void TabSecrityPage::on_open_closeButton_clicked()
{

}

