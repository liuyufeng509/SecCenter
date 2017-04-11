#include "userinfodialog.h"
#include "ui_userinfodialog.h"

UserInfoDialog::UserInfoDialog(UserInfo &usinfo,int type,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserInfoDialog),
     userinfo(usinfo),
    m_type(type)

{
    ui->setupUi(this);

    ui->uidLineEdit->setText(userinfo.uid);
    ui->uidLineEdit->setEnabled(m_type==Add);
    ui->unameLineEdit->setText(userinfo.uname);
    type==Add? ui->unameLineEdit->setEnabled(true):ui->unameLineEdit->setEnabled(false);
    ui->grpLineEdit->setText(userinfo.group);
    if(userinfo.othgroups.length()>0)
    {
        QString othgrps="";
        foreach (QString grp, userinfo.othgroups) {
            othgrps +=grp+",";
        }
        othgrps = othgrps.left(othgrps.length()-1);
        ui->grpsLineEdit->setText(othgrps);
    }else
        ui->grpsLineEdit->setText("");

    this->setWindowTitle((m_type==Add? tr("添加用户信息"):tr("修改用户信息")));
}

UserInfoDialog::~UserInfoDialog()
{
    delete ui;
}

void UserInfoDialog::accept()
{

}

void UserInfoDialog::on_buttonBox_clicked(QAbstractButton *button)
{

    if(ui->buttonBox->buttonRole(button)  == QDialogButtonBox::AcceptRole)
    {
        UserInfo usrTmp;
        usrTmp.uname = ui->unameLineEdit->text();
        usrTmp.group = ui->grpLineEdit->text();
        usrTmp.othgroups = ui->grpsLineEdit->text().trimmed().split(',');
        usrTmp.uid = ui->uidLineEdit->text();
        for(int i=0; i<usrTmp.othgroups.length(); i++)
        {
            if(usrTmp.othgroups[i].length()==0)
                usrTmp.othgroups.removeAt(i);
        }

        if(!is_group_exist(usrTmp.group) && usrTmp.group!=usrTmp.uname)
        {
            QMessageBox::information(this, tr("提示"), tr("用户组:")+ usrTmp.group+tr("不存在!"));
            return;
        }

        if(usrTmp.othgroups.length()>0)
        {
            foreach (QString grp, usrTmp.othgroups) {
                if(!is_group_exist(grp))
                {
                    QMessageBox::information(this, tr("提示"), tr("用户组:")+ grp+tr("不存在!"));
                    return;
                }
            }
        }

        QString res = m_type==Add?add_user(usrTmp):change_groups(usrTmp);
        if(res=="")
        {
            QMessageBox::information(this, tr("提示"), tr("用户") +( m_type==Add? tr("添加"):tr("修改"))+tr("成功"));
            usrTmp.uid = get_usr_id_by_name(usrTmp.uname);
            userinfo = usrTmp;
            QDialog::accept();
        }
        else
        {
            QMessageBox::information(this, tr("提示"),  tr("用户") +( m_type==Add? tr("添加"):tr("修改"))+tr("失败"));
        }
    }else
    {
        reject();
    }
}
