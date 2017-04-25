#include "ukeydialog.h"
#include "ui_ukeydialog.h"

UkeyDialog::UkeyDialog(int type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UkeyDialog)
{
    ui->setupUi(this);
    get_userinfo(users);
    ui->usercomboBox->addItem("");
    for(int i=0; i<users.size();i++)
    {
        if(users[i].isShow)
            ui->usercomboBox->addItem(users[i].uname);
    }
    if(type == Reset_PIN)
        ui->stackedWidget->setCurrentIndex(0);
    else
        ui->stackedWidget->setCurrentIndex(1);
}

UkeyDialog::~UkeyDialog()
{
    delete ui;
}

void UkeyDialog::on_pinokButton_clicked()
{
    if(ui->cur_pinEdit->text().isEmpty())
    {
        errMsgBox(tr("当前PIN不能为空!"));
        return;
    }
    if(ui->new_pinlineEdit->text()!=ui->re_pinEdit->text())
    {
        errMsgBox(tr("新PIN两次输入不相同!"));
        return ;
    }
    ukif.cur_pin = ui->cur_pinEdit->text();
    ukif.new_pin = ui->new_pinlineEdit->text();
    if(!modify_pin_of_ukey(ukif, err))
    {
        errMsgBox( tr("更改PIN失败，错误码：")+err.errorno+tr(" 错误内容：")+err.err_str);
    }else
    {
        infoMsgBox(tr("更改PIN成功"));
    }
    QDialog::accept();
}

void UkeyDialog::setUserOfUkey(int type)
{
    if(ui->pinEdit->text().isEmpty())
    {
        errMsgBox(tr("当前PIN不能为空!"));
        return;
    }

    if(ui->usercomboBox->currentText().isEmpty())
    {
        errMsgBox(tr("用户名不能为空"));
        return;
    }

    ukif.cur_pin = ui->pinEdit->text();
    ukif.user = ui->usercomboBox->currentText();
    ukif.type = type;
    if(!set_user_of_ukey(ukif, err))
    {
        errMsgBox( tr("绑定用户失败，错误码：")+err.errorno+tr(" 错误内容：")+err.err_str);
    }else
    {
        infoMsgBox(tr("绑定用户成功"));
    }
    accept();
}

void UkeyDialog::on_bundingButton_clicked()
{
    setUserOfUkey(1);
}

void UkeyDialog::on_unbundButton_clicked()
{
    setUserOfUkey(2);
}
