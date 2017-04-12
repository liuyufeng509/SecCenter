#include "kernalparammanager.h"
#include "ui_kernalparammanager.h"

KernParmMngWidget::KernParmMngWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::KernalParamManager)
{
    ui->setupUi(this);
}

KernParmMngWidget::~KernParmMngWidget()
{
    delete ui;
}

void KernParmMngWidget::on_pushButton_clicked()
{
    QString cmd = "echo "+ui->lineEdit->text()+">"+ui->param_comboBox->currentText()+";echo $?";
    cmd = GetCmdRes(cmd).trimmed();
    QStringList list = cmd.split('\n');
    if(list.last().toInt()!=0)
    {
        QMessageBox::information(this, tr("提示"), tr("设置失败,错误码:")+list.last());
    }else
        QMessageBox::information(this, tr("提示"), tr("设置成功"));
}
