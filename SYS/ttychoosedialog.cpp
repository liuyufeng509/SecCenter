#include "ttychoosedialog.h"
#include "ui_ttychoosedialog.h"
#include <QPushButton>
TTyChooseDialog::TTyChooseDialog(QSet<QString>ttys, QString &curTty ,QWidget *parent) :
    QDialog(parent),
    m_curTty(curTty),
    ui(new Ui::TTyChooseDialog)
{
    ui->setupUi(this);
    ui->buttonBox->addButton(QDialogButtonBox::Cancel)->setText(tr("取消"));
    ui->buttonBox->addButton(QDialogButtonBox::Ok)->setText(tr("确定"));

    foreach(const QString &value, ttys)
        ui->comboBox->addItem(value);
    ui->comboBox->setCurrentIndex(0);
}

TTyChooseDialog::~TTyChooseDialog()
{
    delete ui;
}

void TTyChooseDialog::on_comboBox_currentIndexChanged(int index)
{
    m_curTty = ui->comboBox->currentText();
    qDebug()<<index;
}

void TTyChooseDialog::on_buttonBox_clicked(QAbstractButton *button)
{
    if(ui->buttonBox->buttonRole(button)  == QDialogButtonBox::AcceptRole)
    {
        if(warnMsgBox(tr("确定要断开该终端?"), this)==QMessageBox::Cancel)
        {
            return;
        }
       accept();
    }else
    {
        reject();
    }
}
