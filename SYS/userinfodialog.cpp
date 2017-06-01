#include "userinfodialog.h"
#include "ui_userinfodialog.h"

UserInfoDialog::UserInfoDialog(UserInfo &usinfo,int type,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserInfoDialog),
     userinfo(usinfo),
    m_type(type)

{
    ui->setupUi(this);
    //获取所有用户组
    try
    {
        SysFunClass::getInstance()->getGroupList(grouplist);
    }catch(Exception exp)
            {
        errMsgBox(exp.getErroWhat(), this);
    }

    //处理附属组下拉框
    pListWidget = new QListWidget(this);
    pLineEdit = new QLineEdit(this);
    for (int i = 0; i < grouplist.count(); ++i)
    {
        QListWidgetItem *pItem = new QListWidgetItem(pListWidget);
        pListWidget->addItem(pItem);
        pItem->setData(Qt::UserRole, i);
        QCheckBox *pCheckBox = new QCheckBox(this);
        pCheckBox->setText(grouplist[i]);
        pListWidget->addItem(pItem);
        pListWidget->setItemWidget(pItem, pCheckBox);
        connect(pCheckBox, SIGNAL(stateChanged(int)), this, SLOT(stateChanged(int)));
        checkList.append(pCheckBox);
    }
    ui->comboBox->setModel(pListWidget->model());
    ui->comboBox->setView(pListWidget);
    ui->comboBox->setLineEdit(pLineEdit);
    pLineEdit->setReadOnly(true);                                                                                                                    //ui.comboBox->setEditable(true);
    connect(pLineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(textChanged(const QString &)));


    //更新数据到界面
//    ui->uidLineEdit->setText(userinfo.uid);
//    ui->uidLineEdit->setEnabled(m_type==Add);
    ui->unameLineEdit->setText(userinfo.uname);
    type==Add? ui->unameLineEdit->setEnabled(true):ui->unameLineEdit->setEnabled(false);
  //  pLineEdit->setText(userinfo.group);
    ui->grpLineEdit->setText(userinfo.group);
    for(int i=0; i<checkList.count(); i++)
        {
        if(userinfo.ogroups.contains(checkList[i]->text()))
            checkList[i]->setChecked(true);
    }


    this->setWindowTitle((m_type==Add? tr("添加用户信息"):tr("修改用户信息")));

    ui->buttonBox->addButton(QDialogButtonBox::Cancel)->setText(tr("取消"));
    ui->buttonBox->addButton(QDialogButtonBox::Ok)->setText(tr("确定"));
}

void UserInfoDialog::stateChanged(int state)
{

    bSelected = true;
    QString strSelectedData("");
    strSelectedText.clear();
    QObject *object = QObject::sender();
    QCheckBox *pSenderCheckBox = static_cast<QCheckBox*>(object);
    int nCount = pListWidget->count();
    for (int i = 0; i < nCount; ++i)
    {
        QListWidgetItem *pItem = pListWidget->item(i);
        QWidget *pWidget = pListWidget->itemWidget(pItem);
                QCheckBox *pCheckBox = (QCheckBox *)pWidget;
                if (pCheckBox->isChecked())
                {
                        QString strText = pCheckBox->text();
                        strSelectedData.append(strText).append(",");
                }
                //所点击的复选框
        if (pSenderCheckBox == pCheckBox)
        {
                        int nData = pItem->data(Qt::UserRole).toInt();
            qDebug() << QString("I am sender...id : %1").arg(nData)<<state;
        }
    }
    if (strSelectedData.endsWith(","))
        strSelectedData.remove(strSelectedData.count() - 1, 1);
    if (!strSelectedData.isEmpty())
    {
                //ui.comboBox->setEditText(strSelectedData);
                strSelectedText = strSelectedData;
        pLineEdit->setText(strSelectedData);
        pLineEdit->setToolTip(strSelectedData);
    }
        else
        {
                pLineEdit->clear();
                //ui.comboBox->setEditText("");
        }
        bSelected = false;
}

void UserInfoDialog::textChanged(const QString &text)
{
    qDebug()<<text;
        if (!bSelected)
                pLineEdit->setText(strSelectedText);
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
    if(ui->unameLineEdit->text().isEmpty())
    {
        errMsgBox(tr("请输入用户名"),this);
        return;
    }

    if(ui->grpLineEdit->text().isEmpty())
    {
        errMsgBox(tr("请输入所属组"),this);
        return;
    }
    if(ui->buttonBox->buttonRole(button)  == QDialogButtonBox::AcceptRole)
    {
        UserInfo usrTmp;
        usrTmp.uname = ui->unameLineEdit->text();
        usrTmp.group = ui->grpLineEdit->text();
        usrTmp.ogroups = pLineEdit->text().trimmed().split(',');//ui->grpsLineEdit->text().trimmed().split(',');
        //usrTmp.uid = ui->uidLineEdit->text();
        for(int i=0; i<usrTmp.ogroups.length(); i++)
        {
            if(usrTmp.ogroups[i].length()==0)
                usrTmp.ogroups.removeAt(i);
        }
        //判断用户组是否存在， 需要捕获异常。
        try
        {
            if(!SysFunClass::getInstance()->isGroupExist(usrTmp.group) && usrTmp.group!=usrTmp.uname)
            {
                errMsgBox(tr("用户组:")+ usrTmp.group+tr("不存在!"), this);
                return;
            }
            if(usrTmp.ogroups.length()>0)
            {
                foreach (QString grp, usrTmp.ogroups) {
                    if(!SysFunClass::getInstance()->isGroupExist(grp))
                    {
                        errMsgBox(tr("用户组:")+ grp+tr("不存在!"), this);
                        return;
                    }
                }
            }
        }catch(Exception exp)
        {
            errMsgBox(exp.getErroWhat(), this);
            return;
        }
        //开始添加或者编辑用户
        try
        {
            if(m_type==Add)
            {
                SysFunClass::getInstance()->addUser(usrTmp);
                infoMsgBox(tr("添加用户成功"), this);
            }else
            {
                SysFunClass::getInstance()->modifyUser(usrTmp);
                infoMsgBox(tr("修改用户成功"), this);
            }
            QDialog::accept();
        }catch(Exception exp)
         {
            errMsgBox(exp.getErroWhat(), this);
        }
    }else
    {
        reject();
    }
}
