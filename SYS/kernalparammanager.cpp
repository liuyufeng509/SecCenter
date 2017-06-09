#include "kernalparammanager.h"
#include "ui_kernalparammanager.h"
#include "qreadconfig.h"
KernParmMngWidget::KernParmMngWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KernalParamManager)
{
    ui->setupUi(this);
    QString exp ="^\\d+$";
    QRegExp regExp( exp);
    ui->lineEdit->setValidator(new QRegExpValidator(regExp,this));
     ui->param_comboBox->clear();
    for(int i=0; i<QGlobalClass::getInstance()->kernCfgInfoList.size;i++)
    {
         ui->param_comboBox->addItem(QGlobalClass::getInstance()->kernCfgInfoList.list[i].name);
    }
    ui->param_comboBox->setCurrentIndex(0);
}

KernParmMngWidget::~KernParmMngWidget()
{
    delete ui;
}

void KernParmMngWidget::on_pushButton_clicked()
{
    if((!ui->valueComboBox->isHidden())&&ui->valueComboBox->currentText().isEmpty())
        {
        errMsgBox(tr("参数名称为空"), this);
        return;
    }
    if((!ui->lineEdit->isHidden())&&ui->lineEdit->text().isEmpty())
        {
        errMsgBox(tr("参数值不能为空"), this);
        return;
    }
    try
    {
        SysFunClass::getInstance()->setKernelParam(ui->param_comboBox->currentText(), ui->lineEdit->isHidden()?ui->valueComboBox->currentText():ui->lineEdit->text());
        infoMsgBox(tr("设置成功"), this);
    }catch(Exception exp)
            {
        errMsgBox(exp.getErroWhat(), this);
    }
}

void KernParmMngWidget::on_param_comboBox_currentIndexChanged(const QString &arg1)
{
    try
    {
        for(int i=0; i<QGlobalClass::getInstance()->kernCfgInfoList.size;i++)
        {
             if(QGlobalClass::getInstance()->kernCfgInfoList.list[i].name==arg1)
              {
           //      ui->lineEdit->setText(QReadConfig::getInstance()->kernCfgInfoList.list[i].value);
                 QString value="";
                 SysFunClass::getInstance()->getKernelParam(arg1, value);
                 if(QGlobalClass::getInstance()->kernCfgInfoList.list[i].type==0)
                 {
                    ui->lineEdit->setHidden(true);
                    ui->valueComboBox->setHidden(false);
                    ui->valueComboBox->clear();
                    QStringList valueList = QGlobalClass::getInstance()->kernCfgInfoList.list[i].values.split("/");
                    int index = 0;
                    for(int i=0; i<valueList.count();i++)
                    {
                        ui->valueComboBox->addItem(valueList[i]);
                        if(value==valueList[i])
                        {
                            index = i;
                        }
                    }
                    ui->valueComboBox->setCurrentIndex(index);
                 }else
                     {
                     ui->lineEdit->setHidden(false);
                     ui->valueComboBox->setHidden(true);
                     ui->lineEdit->setText(value);
                 }
                 ui->label_3->setText(tr("参数说明:\n  ")+QGlobalClass::getInstance()->kernCfgInfoList.list[i].desc);
                 break;
             }
        }
    }catch(Exception exp)
            {
        errMsgBox(exp.getErroWhat(), this);
    }

    //qDebug()<<ui->groupBox->width()<<",  "<<ui->groupBox->height();
}
