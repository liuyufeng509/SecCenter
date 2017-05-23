#include "kernalparammanager.h"
#include "ui_kernalparammanager.h"
#include "qreadconfig.h"
KernParmMngWidget::KernParmMngWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KernalParamManager)
{
    ui->setupUi(this);
//    QRegExp regExp("^\\d{1,}$");   //^[1-9][0-9]*$ 和 ^[1-9]{1}[/d]*$
//    ui->lineEdit->setValidator(new QRegExpValidator(regExp,this));
     ui->param_comboBox->clear();
    for(int i=0; i<QReadConfig::getInstance()->kernCfgInfoList.size;i++)
    {
         ui->param_comboBox->addItem(QReadConfig::getInstance()->kernCfgInfoList.list[i].name);
    }
}

KernParmMngWidget::~KernParmMngWidget()
{
    delete ui;
}

void KernParmMngWidget::on_pushButton_clicked()
{
    if(ui->param_comboBox->currentText().isEmpty())
        {
        errMsgBox(tr("参数名称为空"));
        return;
    }
    if(ui->lineEdit->text().isEmpty())
        {
        errMsgBox(tr("参数值不能为空"));
        return;
    }
    try
    {
        SysFunClass::getInstance()->setKernelParam(ui->param_comboBox->currentText(), ui->lineEdit->text());
        infoMsgBox(tr("设置成功"));
    }catch(Exception exp)
            {
        errMsgBox(exp.getErroWhat());
    }
}

void KernParmMngWidget::on_param_comboBox_currentIndexChanged(const QString &arg1)
{
    //可以日后设置对应的默认值
    for(int i=0; i<QReadConfig::getInstance()->kernCfgInfoList.size;i++)
    {
         if(QReadConfig::getInstance()->kernCfgInfoList.list[i].name==arg1)
             {
             ui->lineEdit->setText(QReadConfig::getInstance()->kernCfgInfoList.list[i].defaultValue);
             break;
         }
    }
}
