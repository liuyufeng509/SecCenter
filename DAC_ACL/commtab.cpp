#include "commtab.h"
#include "ui_commtab.h"
#include <QFileDialog>
#include <QMessageBox>
#include"common.h"
#include<QFileInfo>
#include"alcdialog.h"
#include"qreadconfig.h"
CommTab::CommTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommTab)
{
    ui->setupUi(this);
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
    ui->listWidget->setCurrentRow(0);
    SetUiEnable(false);
    //clean the commpent
    cleanUI();
//    connect(ui->rCheckBox, SIGNAL(clicked(bool)), this, SLOT(persionSet(bool)));
//    connect(ui->wCheckBox, SIGNAL(clicked(bool)), this, SLOT(persionSet(bool)));
//    connect(ui->exeCheckBox, SIGNAL(clicked(bool)), this, SLOT(persionSet(bool)));

//    connect(ui->rCheckBox_2, SIGNAL(clicked(bool)), this, SLOT(persionSet(bool)));
//    connect(ui->wCheckBox_2, SIGNAL(clicked(bool)), this, SLOT(persionSet(bool)));
//    connect(ui->exeCheckBox_2, SIGNAL(clicked(bool)), this, SLOT(persionSet(bool)));

//    connect(ui->rCheckBox_3, SIGNAL(clicked(bool)), this, SLOT(persionSet(bool)));
//    connect(ui->wCheckBox_3, SIGNAL(clicked(bool)), this, SLOT(persionSet(bool)));
//    connect(ui->exeCheckBox_3, SIGNAL(clicked(bool)), this, SLOT(persionSet(bool)));

    ui->listWidget->item(0)->setHidden(QReadConfig::getInstance()->comInfo.dac_hidden);
    ui->listWidget->item(1)->setHidden(QReadConfig::getInstance()->comInfo.other_hidden);
}

void CommTab::SetUiEnable(bool enable)
{
    ui->ownerComboBox->setEnabled(enable);
    ui->groupComboBox->setEnabled(enable);
    ui->rCheckBox->setEnabled(enable);
    ui->wCheckBox->setEnabled(enable);
    ui->exeCheckBox->setEnabled(enable);
    ui->rCheckBox_2->setEnabled(enable);
    ui->wCheckBox_2->setEnabled(enable);
    ui->exeCheckBox_2->setEnabled(enable);
     ui->rCheckBox_3->setEnabled(enable);
     ui->wCheckBox_3->setEnabled(enable);
     ui->exeCheckBox_3->setEnabled(enable);
     ui->setMaskButton->setEnabled(enable);
     ui->setALCButton->setEnabled(enable);
}

void CommTab::updateUI()
{
    ui->ownerComboBox->clear();
    ui->groupComboBox->clear();
    switch (get_user_role()) {
    case ROOT:
    {
        QStringList users = get_users();
        for(int i=0; i<users.length(); i++)
        {
            ui->ownerComboBox->addItem(users[i]);
        }
        if(ui->ownerComboBox->findText(fileAttr.owner)==-1)
            qDebug()<<"ownerCombox can't find owner.";
        else
            ui->ownerComboBox->setCurrentIndex(ui->ownerComboBox->findText(fileAttr.owner));
        ui->rCheckBox->setChecked(fileAttr.ownerauth & 0x4);
        ui->wCheckBox->setChecked(fileAttr.ownerauth & 0x2);
        ui->exeCheckBox->setChecked(fileAttr.ownerauth & 0x1);

        QStringList groups = get_groups();
        for(int i=0; i<groups.length(); i++)
        {
            ui->groupComboBox->addItem(groups[i]);
        }
        if(ui->groupComboBox->findText(fileAttr.groupName)==-1)
            qDebug()<<"groupCombox can't find groupName.";
        else
            ui->groupComboBox->setCurrentIndex(ui->groupComboBox->findText(fileAttr.groupName));

        ui->rCheckBox_2->setChecked(fileAttr.groupauth & 0x4);
        ui->wCheckBox_2->setChecked(fileAttr.groupauth & 0x2);
        ui->exeCheckBox_2->setChecked(fileAttr.groupauth & 0x1);

        ui->rCheckBox_3->setChecked(fileAttr.ohterauth & 0x4);
        ui->wCheckBox_3->setChecked(fileAttr.ohterauth & 0x2);
        ui->exeCheckBox_3->setChecked(fileAttr.ohterauth & 0x1);
        SetUiEnable(true);
    }
        break;
    default:
    {
        ui->ownerComboBox->addItem(fileAttr.owner);
        ui->ownerComboBox->setCurrentIndex(ui->ownerComboBox->findText(fileAttr.owner));
        ui->groupComboBox->setCurrentIndex(ui->groupComboBox->findText(fileAttr.groupName));

        if(fileAttr.owner == get_cur_user() )
        {
            SetUiEnable(true);
        }else
        {
            SetUiEnable(false);
        }
    }
        break;
    }

}

void CommTab::cleanUI()
{
    ui->ownerComboBox->clear();
    ui->rCheckBox->setChecked(false);
    ui->wCheckBox->setChecked(false);
    ui->exeCheckBox->setChecked(false);
    ui->rCheckBox_2->setChecked(false);
    ui->wCheckBox_2->setChecked(false);
    ui->exeCheckBox_2->setChecked(false);
    ui->rCheckBox_3->setChecked(false);
    ui->wCheckBox_3->setChecked(false);
    ui->exeCheckBox_3->setChecked(false);
    ui->groupComboBox->clear();
}

CommTab::~CommTab()
{
    delete ui;
}
bool CommTab::getFileAtrribute(QString path)
{
    //call dac interface
    QString cmd = "stat -c %U "+path;
    cmd = GetCmdRes(cmd);
    fileAttr.fileName = path;
    fileAttr.owner = cmd.trimmed();
    cmd = "stat -c %a " + path;
    cmd = GetCmdRes(cmd);
    int res = cmd.toInt();
    fileAttr.ownerauth =res/100;
    fileAttr.groupauth = (res/10)%10;
    fileAttr.ohterauth = res%10;
    cmd = "stat -c %G " + path;
    cmd = GetCmdRes(cmd);
    fileAttr.groupName = cmd.trimmed();
    cmd = "umask";
    fileAttr.umask = GetCmdRes(cmd).trimmed();
    return true;
}

void CommTab::on_pushButton_clicked()
{
    QString path = "";
    if(ui->isDirheckBox->isChecked())
    {
        path = QFileDialog::getExistingDirectory(this, tr("选择目录"), "./");
    }else
    {
          path = QFileDialog::getOpenFileName(this, tr("打开文件"), ".", NULL);
    }

    if(path.length() == 0) {
            QMessageBox::information(NULL, tr("路径"), tr("你未选择任何文件"));
    } else {
            QMessageBox::information(NULL, tr("路径"), tr("你选择了") + path);
            ui->lineEdit->setText(path);
            if(getFileAtrribute(path))
            {
                qDebug()<<"get file attr success";
                //显示
                updateUI();
            }else
                qDebug()<<"get file attr failed";

    }
}

void CommTab::messageInfo(QString info)
{
    if(info.trimmed().length()>0)
    {
        QMessageBox::information(this, tr("提示"), info);
    }
}

void CommTab::on_ownerComboBox_currentIndexChanged(int index)
{
    fileAttr.owner =ui->ownerComboBox->currentText().trimmed() ;
    QString cmd = "chown "+fileAttr.owner +  " " + fileAttr.fileName;
    cmd = GetCmdRes(cmd);
    messageInfo(cmd);
}

void CommTab::on_groupComboBox_currentIndexChanged(int index)
{
    fileAttr.groupName =ui->groupComboBox->currentText() ;
    QString cmd = "chgrp "+fileAttr.groupName +  " " + fileAttr.fileName;
    cmd = GetCmdRes(cmd);
    messageInfo(cmd);
}

void CommTab::persionSet(bool)
{
    ui->rCheckBox->isChecked()? fileAttr.ownerauth = fileAttr.ownerauth | 0x4 : fileAttr.ownerauth = fileAttr.ownerauth & ~0x4;
    ui->wCheckBox->isChecked()? fileAttr.ownerauth = fileAttr.ownerauth | 0x2 : fileAttr.ownerauth = fileAttr.ownerauth & ~0x2;
    ui->exeCheckBox->isChecked()? fileAttr.ownerauth = fileAttr.ownerauth | 0x1 : fileAttr.ownerauth = fileAttr.ownerauth & ~0x1;

    ui->rCheckBox_2->isChecked()? fileAttr.groupauth = fileAttr.groupauth | 0x4 : fileAttr.groupauth = fileAttr.groupauth & ~0x4;
    ui->wCheckBox_2->isChecked()? fileAttr.groupauth = fileAttr.groupauth | 0x2 : fileAttr.groupauth = fileAttr.groupauth & ~0x2;
    ui->exeCheckBox_2->isChecked()? fileAttr.groupauth = fileAttr.groupauth | 0x1 : fileAttr.groupauth = fileAttr.groupauth & ~0x1;

    ui->rCheckBox_3->isChecked()? fileAttr.ohterauth = fileAttr.ohterauth | 0x4 : fileAttr.ohterauth = fileAttr.ohterauth & ~0x4;
    ui->wCheckBox_3->isChecked()? fileAttr.ohterauth = fileAttr.ohterauth | 0x2 : fileAttr.ohterauth = fileAttr.ohterauth & ~0x2;
    ui->exeCheckBox_3->isChecked()? fileAttr.ohterauth = fileAttr.ohterauth | 0x1 : fileAttr.ohterauth = fileAttr.ohterauth & ~0x1;

    QFileInfo finfo(fileAttr.fileName);
     QString cmd="";
    if(finfo.isDir())
    {
        if(!ui->subCheckBox->isChecked())
        {
            cmd = "chmod "+QString::number(fileAttr.ownerauth*100 + fileAttr.groupauth*10 + fileAttr.ohterauth) + " " +fileAttr.fileName;
            cmd = GetCmdRes(cmd);
        }
        else
        {
            cmd = "chmod -R "+QString::number(fileAttr.ownerauth*100 + fileAttr.groupauth*10 + fileAttr.ohterauth) + " "+ fileAttr.fileName;
            cmd = GetCmdRes(cmd);
        }
    }else
    {
        cmd = "chmod "+QString::number(fileAttr.ownerauth*100 + fileAttr.groupauth*10 + fileAttr.ohterauth)+ " " + fileAttr.fileName;
        cmd = GetCmdRes(cmd);
    }

    messageInfo(cmd);
}

void CommTab::on_setMaskButton_clicked()
{

    on_ownerComboBox_currentIndexChanged(0);

    on_groupComboBox_currentIndexChanged(0);

    persionSet(true);

    QMessageBox::information(this, tr("提示"), tr("操作结束"));
}

void CommTab::on_isDirheckBox_clicked()
{
       ui->subCheckBox->setEnabled(ui->isDirheckBox->isChecked());
}

void CommTab::on_setALCButton_clicked()
{
    ALCDialog alcD(fileAttr);
    alcD.exec();
}
