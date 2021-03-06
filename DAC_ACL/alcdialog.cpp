#include "alcdialog.h"
#include "ui_alcdialog.h"
#include "DacDemo.h"

ALCDialog::ALCDialog(FileAttr &fileAttr, QWidget *parent) :
     QDialog(parent),
     ui(new Ui::ALCDialog),
     m_fileAttr(fileAttr)
{
    ui->setupUi(this);
    ui->lineEdit->setText(fileAttr.fileName);
    QStringList users = get_users();
    ui->userComboBox->addItem(STR_WU);
    for(int i=0; i<users.length(); i++)
    {
        ui->userComboBox->addItem(users[i]);
    }
    ui->userComboBox->setCurrentIndex(0);

    QStringList groups = get_groups();
    ui->groupComboBox->addItem(STR_WU);
    for(int i=0; i<groups.length(); i++)
    {
        ui->groupComboBox->addItem(groups[i]);
    }

    ui->groupComboBox->setCurrentIndex(0);

    QFileInfo finfo(fileAttr.fileName);

    ui->subcheckBox->setEnabled(finfo.isDir());


}

ALCDialog::~ALCDialog()
{
    delete ui;
}

void ALCDialog::on_cancelButton_clicked()
{
    reject();
}

void ALCDialog::on_okButton_clicked()
{
    accept();
}

void ALCDialog::on_getalcButton_clicked()
{
    char Message[1024]={};
    char filename[1024]={};
    strcpy(filename, m_fileAttr.fileName.toStdString().c_str());
    if(getFileAcl(filename, Message, sizeof(Message))==0)
        ui->textBrowser->setText(QString(Message));
    else
        ui->textBrowser->setText("查看ACL信息失败:"+QString(Message));
}

void ALCDialog::on_setUAlcButton_clicked()
{

    QString user ="";
    if(ui->userComboBox->currentText()==STR_WU)
    {
        user = "";
    }else
            user = ui->userComboBox->currentText();

    char Message[512]={};
    QString cmd = QString("setfacl ") + (ui->subcheckBox->isChecked()? "-R":"")+ "  -m u:"+user + ":"+(ui->rcheckBox->isChecked()?"r":"-")
            + (ui->wcheckBox->isChecked()?"w":"-")+(ui->execheckBox->isChecked()?"x":"-") + " "+ m_fileAttr.fileName;

   if(setFileAcl((char*)cmd.toStdString().c_str(), Message)==-1)
   {
       qDebug()<<Message;
   }else
       QMessageBox::information(this,tr("用户ACL设置"), tr("用户:")+user+tr("ACL权限设置成功!"));
}

void ALCDialog::on_delUAlcButton_clicked()
{
    QString user = "";
    if(ui->userComboBox->currentText()==STR_WU)
    {
        QMessageBox::information(this,tr("用户ACL设置"), tr("用户名为空!"));
        return ;
    }else
            user = ui->userComboBox->currentText();

    char Message[512]={};
    QString cmd = QString("setfacl ") + (ui->subcheckBox->isChecked()? "-R":"")+"  -x u:"+user + " " +m_fileAttr.fileName;

   if(setFileAcl((char*)cmd.toStdString().c_str(), Message)==-1)
   {
       qDebug()<<Message;
   }else
       QMessageBox::information(this,tr("用户ACL设置"), tr("用户:")+user+tr("ACL权限删除成功!"));
}

void ALCDialog::on_setGAlcButton_clicked()
{
    QString group ="";
    if(ui->groupComboBox->currentText()==STR_WU)
    {
        group = "";
    }else
            group = ui->groupComboBox->currentText();

    char Message[512]={};
    QString cmd = QString("setfacl ") + (ui->subcheckBox->isChecked()? "-R":"")+ "  -m g:"+group + ":"+(ui->rcheckBox_2->isChecked()?"r":"-")
            + (ui->wcheckBox_2->isChecked()?"w":"-")+(ui->execheckBox_2->isChecked()?"x":"-") + " "+ m_fileAttr.fileName;

   if(setFileAcl((char*)cmd.toStdString().c_str(), Message)==-1)
   {
       qDebug()<<Message;
   }else
       QMessageBox::information(this,tr("用户ACL设置"), tr("用户组:")+group+tr("ACL权限设置成功!"));
}

void ALCDialog::on_delGAlcButton_clicked()
{
    QString group ="";
    if(ui->groupComboBox->currentText()==STR_WU)
    {
        QMessageBox::information(this,tr("用户ACL设置"), tr("组户名为空!"));
        return ;
    }else
            group = ui->groupComboBox->currentText();

    char Message[512]={};
    QString cmd = QString("setfacl ") + (ui->subcheckBox->isChecked()? "-R":"")+"  -x g:"+group +" "+ m_fileAttr.fileName;

   if(setFileAcl((char*)cmd.toStdString().c_str(), Message)==-1)
   {
       qDebug()<<Message;
   }else
       QMessageBox::information(this,tr("用户ACL设置"), tr("用户组:")+group+tr("ACL权限设置成功!"));
}
