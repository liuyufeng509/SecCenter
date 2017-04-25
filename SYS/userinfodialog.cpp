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
    if(userinfo.ogroups.length()>0)
    {
        QString othgrps="";
        foreach (QString grp, userinfo.ogroups) {
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
        usrTmp.ogroups = ui->grpsLineEdit->text().trimmed().split(',');
        usrTmp.uid = ui->uidLineEdit->text();
        for(int i=0; i<usrTmp.ogroups.length(); i++)
        {
            if(usrTmp.ogroups[i].length()==0)
                usrTmp.ogroups.removeAt(i);
        }
        //判断用户组是否存在， 需要捕获异常。
        try
        {
            if(!m_sysFunModel.isGroupExist(usrTmp.group) && usrTmp.group!=usrTmp.uname)
            {
                errMsgBox(tr("用户组:")+ usrTmp.group+tr("不存在!"));
                return;
            }
            if(usrTmp.ogroups.length()>0)
            {
                foreach (QString grp, usrTmp.ogroups) {
                    if(!m_sysFunModel.isGroupExist(grp))
                    {
                        errMsgBox(tr("用户组:")+ grp+tr("不存在!"));
                        return;
                    }
                }
            }
        }catch(Exception exp)
        {
            errMsgBox(exp.getErroWhat());
            return;
        }
        //开始添加或者编辑用户
        try
        {
            if(m_type==Add)
            {
                m_sysFunModel.addUser(usrTmp);
                infoMsgBox(tr("添加用户成功"));
            }else
            {
                m_sysFunModel.modifyUser(usrTmp);
                infoMsgBox(tr("修改用户成功"));
            }
            QDialog::accept();
        }catch(Exception exp)
         {
            errMsgBox(exp.getErroWhat());
        }
    }else
    {
        reject();
    }
}
