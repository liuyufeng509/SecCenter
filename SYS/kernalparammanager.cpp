#include "kernalparammanager.h"
#include "ui_kernalparammanager.h"

KernParmMngWidget::KernParmMngWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KernalParamManager)
{
    ui->setupUi(this);
//    QRegExp regExp("^\\d{1,}$");   //^[1-9][0-9]*$ 和 ^[1-9]{1}[/d]*$
//    ui->lineEdit->setValidator(new QRegExpValidator(regExp,this));
}

KernParmMngWidget::~KernParmMngWidget()
{
    delete ui;
}

void KernParmMngWidget::on_pushButton_clicked()
{
    try
    {
        m_sysFunModel.setKernelParam(ui->param_comboBox->currentText(), ui->lineEdit->text());
        infoMsgBox(tr("设置成功"));
    }catch(Exception exp)
            {
        errMsgBox(exp.getErroWhat());
    }
}
