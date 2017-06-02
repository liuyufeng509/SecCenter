#include "usermgrpage.h"
#include "ui_usermgrpage.h"
#include"ukeydialog.h"
UserMgrPage::UserMgrPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserMgrPage)
{
    ui->setupUi(this);
    ui->userTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->userTableWidget->setEditTriggers ( QAbstractItemView::NoEditTriggers );
    QHeaderView* headerView = ui->userTableWidget->verticalHeader();
    headerView->setHidden(true);

    ui->userTableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    secUserMenu = new QMenu(ui->userTableWidget);
    unLockAction = new QAction(tr("解锁用户"), this);
    //ukeyPINAction = new QAction(tr("更改UkeyPIN"), this);
    ukeyBindAction = new QAction(tr("解绑UKey"), this);
    freshAction = new QAction(tr("刷新"),this);
    connect(unLockAction, SIGNAL(triggered()), this, SLOT(unLockActionSlot()));
   // connect(ukeyPINAction, SIGNAL(triggered()), this, SLOT(ukeyPINActionSlot()));
    connect(ukeyBindAction, SIGNAL(triggered()), this, SLOT(ukeyBindActionSlot()));
    connect(freshAction, SIGNAL(triggered()), this, SLOT(freshActionSlot()));

    connect(ui->unlockButton, SIGNAL(clicked()), this, SLOT(unLockActionSlot()));
    connect(ui->modPinButton, SIGNAL(clicked()), this, SLOT(ukeyPINActionSlot()));
    connect(ui->bindUkButton, SIGNAL(clicked()), this, SLOT(ukeyBindActionSlot()));
    connect(ui->freshButton, SIGNAL(clicked()), this, SLOT(freshActionSlot()));
    updateSecUserUI();

    QRegExp regExp("[1-9][0-9][0-9][0-9][0-9]{0,5}");   //^[1-9][0-9]*$ 和 ^[1-9]{1}[/d]*$ 前面的正则表达式表示只能输入大于0的正整数
    //用户锁定规则设置
    getLockServices();
    ui->lockSvrComboBox->addItems(services);
    ui->tmsLineEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->secLineEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->lockSvrComboBox->setCurrentIndex(0);

    //密码规则设置
    ui->minlenEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->dlenEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->uplenEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->lowlenEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->othlenEdit->setValidator(new QRegExpValidator(regExp, this));
    display_cur_pwd_info();
}

void UserMgrPage::display_cur_pwd_info()
{
    try
    {
        SecFunClass::getInstance()->getCurPwdInfo(pwdInfo);
        ui->minlenEdit->setText(pwdInfo.minLen.isEmpty()?tr("无限制"):pwdInfo.minLen);
        ui->dlenEdit->setText(pwdInfo.dcredit.isEmpty()?tr("无限制"):pwdInfo.dcredit);
        ui->uplenEdit->setText(pwdInfo.ucredit.isEmpty()?tr("无限制"):pwdInfo.ucredit);
        ui->lowlenEdit->setText(pwdInfo.lcredit.isEmpty()?tr("无限制"):pwdInfo.lcredit);
        ui->othlenEdit->setText(pwdInfo.ocredit.isEmpty()?tr("无限制"):pwdInfo.ocredit);
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat(), this);
    }
}

void UserMgrPage::getLockServices()
{
    //enhanced-trylock  -l
    try
    {
        SecFunClass::getInstance()->getLockServices(services);
    }catch(Exception exp)
            {
        exp.getErroWhat();
        services.clear();
        services<<"login"<<"gdm"<<"ssh";
    }
}

void UserMgrPage::on_lockSvrComboBox_currentIndexChanged(const QString &arg1)
{
    TryLockInfo info;
    info.sParam = arg1;

    try
    {
        SecFunClass::getInstance()->getCurLockInfo(info);
        ui->tmsLineEdit->setText(info.dParam);
        ui->secLineEdit->setText(info.uParam);
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat(), this);
    }
}

void UserMgrPage::on_userTableWidget_customContextMenuRequested(const QPoint &pos)
{
    pos.isNull();
    int row = ui->userTableWidget->currentRow();
    if(row<0)
        return;
    unLockAction->setEnabled(secUserList[row].bLocked); //被锁用户才可以使用
    secUserMenu->addAction(unLockAction);
   // secUserMenu->addAction(ukeyPINAction);
    secUserList[row].bBindKey?ukeyBindAction->setText(tr("解绑Ukey")):ukeyBindAction->setText(tr("绑定Ukey"));
    secUserMenu->addAction(ukeyBindAction);
    secUserMenu->addAction(freshAction);
    secUserMenu->exec(QCursor::pos());
}

void UserMgrPage::updateSecUserUI()
{
    try
    {
        SecFunClass::getInstance()->getSecUserList(secUserList);
        ui->userTableWidget->setRowCount(0);
        ui->userTableWidget->setRowCount(secUserList.size());
        ui->userTableWidget->setToolTip(tr("右键可对用户进行操作"));
        for(int i=0; i<secUserList.length(); i++)
        {
            ui->userTableWidget->setItem(i, 0, new QTableWidgetItem(secUserList[i].uName));
            ui->userTableWidget->setItem(i, 1, new QTableWidgetItem(secUserList[i].bLocked?tr("是"):tr("否")));
            ui->userTableWidget->setItem(i, 2, new QTableWidgetItem(secUserList[i].bBindKey?tr("是"):tr("否")));
            if(!secUserList[i].bShow)
                ui->userTableWidget->setRowHidden(i, true);
        }

    }catch(Exception exp)
     {
        errMsgBox(exp.getErroWhat(), this);
    }
}

void UserMgrPage::unLockActionSlot()
{
    int row = ui->userTableWidget->currentRow();
    if(row<0)
    {
        errMsgBox(tr("未选中要操作的用户!"), this);
        return;
    }
    if(!secUserList[row].bLocked)
        {
        infoMsgBox(tr("选中的用户未被锁定"), this);
        return;
    }
    try
    {
        SecFunClass::getInstance()->unLockUser(secUserList[row].uName);
        infoMsgBox(tr("解锁成功"), this);
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat(), this);
    }
    updateSecUserUI();
}

void UserMgrPage::ukeyPINActionSlot()
{
    UkeyDialog ukdialog(Reset_PIN, "", this);
    ukdialog.exec();
}

void UserMgrPage::ukeyBindActionSlot()
{
    int row = ui->userTableWidget->currentRow();
    if(row<0)
    {
        errMsgBox(tr("未选中要操作的用户!"), this);
        return;
    }

    if(ukeyBindAction->text()==tr("解绑Ukey"))
        {
        UkeyDialog ukdialog(UnBUND_User, secUserList[row].uName,this);
        if(ukdialog.exec()==QDialog::Accepted)
            updateSecUserUI();
    }else
        {
        UkeyDialog ukdialog(BUND_User, secUserList[row].uName,this);
        if(ukdialog.exec()==QDialog::Accepted)
            updateSecUserUI();
    }

}

void UserMgrPage::on_cleanUkButton_clicked()
{
    UkeyDialog ukdialog(Clean_Ukey, this);
    ukdialog.exec();
}

void UserMgrPage::freshActionSlot()
{
    updateSecUserUI();
}


UserMgrPage::~UserMgrPage()
{
    delete ui;
}

void UserMgrPage::on_userTableWidget_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
{
    int row=0;
    if(previous == NULL)
        row = 0;
    if(current==NULL)
        return;
    row = current->row();
    if(row<0)
        return ;
    if(secUserList[row].bBindKey)
        {
        ui->bindUkButton->setText(tr("解绑UKey"));
    }else
        ui->bindUkButton->setText(tr("绑定UKey"));
}

void UserMgrPage::on_setTryLockButton_clicked()
{
    TryLockInfo info;
    info.dParam =ui->tmsLineEdit->text();
    info.uParam = ui->secLineEdit->text();
    info.sParam = ui->lockSvrComboBox->currentText();
    if(info.dParam.isEmpty() || info.uParam.isEmpty())
        {
        errMsgBox(tr("用户锁定规则的参数不能为空"),this);
        return;
    }
    try
    {
        SecFunClass::getInstance()->tryLockOption(info);
        infoMsgBox(tr("用户锁定规则设置成功"), this);
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat(), this);
    }
}

void UserMgrPage::on_setPwButton_clicked()
{
    if(/*!ui->minlenEdit->text().isEmpty() &&*/ ui->minlenEdit->text().toInt()<8 /*&& ui->minlenEdit->text()!=tr("无限制")*/)
    {
        errMsgBox(tr("最小长度不能小于8，请重新设置最小长度"), this);
        return;
    }
    if(ui->dlenEdit->text().toInt()<1)
    {
        errMsgBox(tr("数字长度至少为1"));
        ui->dlenEdit->setText("1");
        return;
    }
    if(ui->uplenEdit->text().toInt()<1)
    {
        errMsgBox(tr("大写字母长度至少为1"));
        ui->uplenEdit->setText("1");
        return;
    }
    if(ui->lowlenEdit->text().toInt()<1)
    {
        errMsgBox(tr("小写字母长度至少为1"));
        ui->lowlenEdit->setText("1");
        return;
    }
    if(ui->othlenEdit->text().toInt()<1)
    {
        errMsgBox(tr("特殊字符长度至少为1"));
        ui->othlenEdit->setText("1");
        return;
    }
    pwdInfo.minLen =ui->minlenEdit->text();
    pwdInfo.dcredit =ui->dlenEdit->text();
    pwdInfo.ucredit =ui->uplenEdit->text();
    pwdInfo.lcredit =ui->lowlenEdit->text();
    pwdInfo.ocredit =ui->othlenEdit->text();

    try
    {
        SecFunClass::getInstance()->setCurPwdInfo(pwdInfo);
        infoMsgBox(tr("密码规则设置成功"), this);
        display_cur_pwd_info();
    }catch(Exception exp)
            {
        errMsgBox(exp.getErroWhat(), this);
    }

}


