#include "ukeydialog.h"
#include "ui_ukeydialog.h"

UkeyDialog::UkeyDialog(int type, QString username,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UkeyDialog)
{
    ui->setupUi(this);
    ukif.user = username;
//    SysFunClass::getInstance()->moveToThread(thread);
//    thread = new QThread;
    InitUI(type);
}

UkeyDialog::UkeyDialog(int type, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UkeyDialog)
{
    ui->setupUi(this);
    InitUI(type);
}

void UkeyDialog::InitUI(int type)
{
    mType = type;
    if(type == Reset_PIN)
        ui->stackedWidget->setCurrentIndex(0);
    else
        ui->stackedWidget->setCurrentIndex(1);

    qRegisterMetaType<UkeyInfo>("UkeyInfo");
    connect(this, SIGNAL(emitSetUserOfUkeySignal(UkeyInfo )), SecFunClass::getInstance(), SLOT(setUserOfUkeySlot(UkeyInfo)));
    connect(SecFunClass::getInstance(), SIGNAL(emitSetUserOfUkeyDone(int,Exception)), this, SLOT(setUserOfUkeySlot(int ,Exception)));

    connect(this, SIGNAL(emitResetPINOfUkeySignal(UkeyInfo )), SecFunClass::getInstance(), SLOT(resetPINOfUkeySlot(UkeyInfo)));
    connect(SecFunClass::getInstance(), SIGNAL(emitResetPINOfUkeyDone(int,Exception)), this, SLOT(resetPINOfUkeySlot(int ,Exception)));

    QRegExp regx("[a-zA-Z0-9]+$");
    QValidator *validator = new QRegExpValidator(regx, this);
    ui->pinEdit->setValidator( validator );
    ui->cur_pinEdit->setValidator( validator );
    ui->new_pinlineEdit->setValidator( validator );
     ui->re_pinEdit->setValidator( validator );
}


UkeyDialog::~UkeyDialog()
{
    delete ui;
}

void UkeyDialog::waitDialogAccept()
{
    waitD->accept();
    delete waitD;
}
void UkeyDialog::setUserOfUkeySlot(int res, Exception exp)
{
     waitDialogAccept();
     if(res==0)
     {
         switch(mType)
         {
         case BUND_User:
             infoMsgBox(tr("绑定Ukey成功"), this);
             break;
         case UnBUND_User:
             infoMsgBox(tr("解绑Ukey成功"), this);
             break;
         case Clean_Ukey:
             infoMsgBox(tr("清除证书成功"), this);
             break;
         }
         accept();
     }else
     {
         errMsgBox(exp.getErroWhat(), this);
     }
}

void UkeyDialog::resetPINOfUkeySlot(int res, Exception exp)
{
    waitDialogAccept();
    if(res==0)
    {
        infoMsgBox(tr("重置PIN成功"), this);
        accept();
    }else
    {
        errMsgBox(exp.getErroWhat(), this);
    }
}

void UkeyDialog::waitDiaogAppear()
{
    waitD = new WaitDialog(this);
    waitD->exec();
    //waitD->deleteLater();
}

void UkeyDialog::on_pinokButton_clicked()
{
    if(ui->cur_pinEdit->text().isEmpty())
    {
        errMsgBox(tr("当前PIN不能为空!"), this);
        return;
    }
    if(ui->new_pinlineEdit->text().isEmpty()&&ui->re_pinEdit->text().isEmpty())
        {
        errMsgBox(tr("请输入新的PIN码!"), this);
    }
    if(ui->new_pinlineEdit->text()!=ui->re_pinEdit->text())
    {
        errMsgBox(tr("输入的两次新PIN码不一致!"), this);
        return ;
    }
    ukif.cur_pin = ui->cur_pinEdit->text();
    ukif.new_pin = ui->new_pinlineEdit->text();
    emit emitResetPINOfUkeySignal(ukif);
    waitDiaogAppear();
//    try
//    {
//        SecFunClass::getInstance()->resetPINOfUkey(ukif);
//        infoMsgBox(tr("更改PIN成功"));
//    }catch(Exception exp)
//    {
//        errMsgBox(exp.getErroWhat());
//    }

  //  QDialog::accept();
}

void UkeyDialog::setUserOfUkey(int type)
{
    if(ui->pinEdit->text().isEmpty())
    {
        errMsgBox(tr("当前PIN不能为空!"), this);
        return;
    }

    ukif.cur_pin = ui->pinEdit->text();
    ukif.type = type;
    emit emitSetUserOfUkeySignal(ukif);
    waitDiaogAppear();
}

//void UkeyDialog::on_bundingButton_clicked()
//{
//    setUserOfUkey(1);
//}

//void UkeyDialog::on_unbundButton_clicked()
//{
//    setUserOfUkey(2);
//}

void UkeyDialog::on_okButton_clicked()
{
    setUserOfUkey(mType);
}

void UkeyDialog::on_pin_cancelButton_clicked()
{
    reject();
}

void UkeyDialog::on_cancelButton_clicked()
{
    reject();
}
