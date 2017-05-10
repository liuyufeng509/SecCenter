#include "syscalldialog.h"
#include "ui_syscalldialog.h"
#include "common.h"
SysCallDialog::SysCallDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SysCallDialog)
{
    ui->setupUi(this);
    this->setMaximumHeight(367);
    this->setMaximumWidth(555);
}

//void SysCallDialog::mouseReleaseEvent(QMouseEvent *e)
//{
//    qDebug()<<this->size().width()<<"  "<<this->size().height();
//}

SysCallDialog::~SysCallDialog()
{
    delete ui;
}
