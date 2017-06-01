#include "sysusermgrpage.h"
#include "ui_sysusermgrpage.h"
#include"userinfodialog.h"
#include "ttychoosedialog.h"
SysUserMgrPage::SysUserMgrPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysUserMgrPage)
{
    ui->setupUi(this);

    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    QHeaderView* headerView = ui->tableWidget->verticalHeader();
    headerView->setHidden(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    ui->tableWidget->setEditTriggers ( QAbstractItemView::NoEditTriggers );

    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);   //右键菜单
    userMenu = new QMenu(ui->tableWidget);
    addusrAction = new QAction(tr("添加用户"),this);
    edtusrAction = new QAction(tr("编辑用户"),this);
    delusrAction = new QAction(tr("删除用户"),this);
    logoutUsrAction = new QAction(tr("断开终端"),this);

    connect(addusrAction, SIGNAL(triggered()), this, SLOT(add_user_action()));
    connect(edtusrAction, SIGNAL(triggered()), this, SLOT(edit_user_action()));
    connect(delusrAction, SIGNAL(triggered()), this, SLOT(del_user_action()));
    connect(logoutUsrAction, SIGNAL(triggered()), this, SLOT(logout_user_action()));

    connect(ui->addButton, SIGNAL(clicked()), this, SLOT(add_user_action()));
    connect(ui->modButton, SIGNAL(clicked()), this, SLOT(edit_user_action()));
    connect(ui->delButton, SIGNAL(clicked()), this, SLOT(del_user_action()));
    connect(ui->logOutButton, SIGNAL(clicked()), this, SLOT(logout_user_action()));

    UpdateUI();
}

bool SysUserMgrPage::checkUser()           //检测用户是否可以编辑
{
    int row = ui->tableWidget->currentRow();
    if(row<0)
        return false;
    if(users[row].uname=="sysadm" || users[row].uname=="secadm"||users[row].uname=="auditadm")
        return false;
    return true;
}

void SysUserMgrPage::UpdateUI()
{
    try
    {
        SysFunClass::getInstance()->getUserList(users);
        SysFunClass::getInstance()->getLoginedUserInfo(users);
        ui->tableWidget->setRowCount(0);
        ui->tableWidget->setRowCount(users.size());
        ui->tableWidget->setToolTip(tr("右键可对用户进行操作"));
        for(int i=0; i<users.length(); i++)
        {
            ui->tableWidget->setItem(i, 0, new QTableWidgetItem(users[i].uname));
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem(users[i].uid));
            if(users[i].ogroups.length()<1)
            {
                 QString groups=users[i].group;
                 ui->tableWidget->setItem(i, 2, new QTableWidgetItem(groups));
            }else
            {
                QString groups=users[i].group+ ":";
                for(int j=0; j<users[i].ogroups.length()-1;j++)
                {
                    groups += users[i].ogroups[j]+":";
                }
                groups += users[i].ogroups[users[i].ogroups.length()-1];
                ui->tableWidget->setItem(i, 2, new QTableWidgetItem(groups));
            }
            if(users[i].ttys.count()==0)
                {
                ui->tableWidget->setItem(i, 3, new QTableWidgetItem(STR_WU));
            }else
                {
                QString ttys = "";
                foreach(const QString &value, users[i].ttys)
                    ttys+= value+",";
                ttys.chop(1);
                ui->tableWidget->setItem(i, 3, new QTableWidgetItem(ttys));
            }

            if(!users[i].isShow)
                ui->tableWidget->setRowHidden(i, true);
        }
    }catch(Exception exp)
            {
        errMsgBox(exp.getErroWhat(), this);
    }
}
void SysUserMgrPage::on_tableWidget_customContextMenuRequested(QPoint pos)
{
    userMenu->addAction(addusrAction);
    userMenu->addAction(edtusrAction);
    userMenu->addAction(delusrAction);
    userMenu->addAction(logoutUsrAction);
    int row = ui->tableWidget->currentRow();
    if(row>0&&(users[row].ttys.count()>0))
        {
        logoutUsrAction->setEnabled(true);
    }else
        logoutUsrAction->setEnabled(false);
    userMenu->exec(QCursor::pos());
    pos.isNull();
}

void SysUserMgrPage::add_user_action()
{
       UserInfo userinfo;
       UserInfoDialog usrdialog(userinfo, Add,this);
       if(usrdialog.exec()==QDialog::Accepted)
       {
                users.append(userinfo);
                UpdateUI();
       }
}

void SysUserMgrPage::edit_user_action()
{
    int row = ui->tableWidget->currentRow();
    if(row<0)
    {
        errMsgBox(tr("请选中要编辑的行"), this);
        return;
    }
    if(!checkUser())
        {
        errMsgBox(tr("该用户无法编辑"), this);
        return ;
    }
    UserInfoDialog usrdialog(users[row], Edt);
    if(usrdialog.exec()==QDialog::Accepted)
    {
             UpdateUI();
    }

}

void SysUserMgrPage::del_user_action()
{
    int row = ui->tableWidget->currentRow();
    if(row<0)
    {
        errMsgBox( tr("请选中要删除的行"), this);
        return;
    }
    if(!checkUser())
        {
        errMsgBox(tr("该用户无法删除"), this);
        return ;
    }
    if(warnMsgBox(tr("确定要删除用户")+users[row].uname+tr("?"), this)==QMessageBox::Cancel)
    {
        return;
    }
    try
    {
        SysFunClass::getInstance()->delUser(users[row]);
        UpdateUI();
    }catch(Exception exp)
            {
        errMsgBox(exp.getErroWhat(), this);
    }
}

void SysUserMgrPage::logout_user_action()
{
    int row = ui->tableWidget->currentRow();
    if(row<0)
    {
        errMsgBox( tr("请选中要断开的用户"), this);
        return;
    }
    if(users[row].ttys.count()==0)
        {
        errMsgBox( tr("该用户无登陆的终端"), this);
        return;
    }

    QString tty = "";
    TTyChooseDialog ttyD(users[row].ttys, tty, this);
    if(ttyD.exec()==QDialog::Accepted)
    {
        try
        {
            qDebug()<<tty;
            if(tty.contains(":"))
            {
                infoMsgBox(tr("选中终端为该用户当前登陆的桌面程序，不能断开，\n请如要断开，请注销该用户"), this);
                return;
            }
            SysFunClass::getInstance()->logOutUser(tty);
            UpdateUI();
        }catch(Exception exp)
                {
            errMsgBox(exp.getErroWhat(), this);
        }
    }

}

SysUserMgrPage::~SysUserMgrPage()
{
    delete ui;
}

void SysUserMgrPage::on_freshButton_clicked()
{
    UpdateUI();
}
