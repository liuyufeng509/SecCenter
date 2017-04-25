#include "tabsyspage.h"
#include "ui_tabsyspage.h"
#include"userinfodialog.h"
#include"time.h"
#include"logmainwindow.h"
#include"qreadconfig.h"
#include "waitdialog.h"
TabSysPage::TabSysPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabSysPage)
{
    is_first =true;
    ui->setupUi(this);

    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(listRowChangedSlot(int)));
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    QHeaderView* headerView = ui->tableWidget->verticalHeader();
    headerView->setHidden(true);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    ui->tableWidget->setEditTriggers ( QAbstractItemView::NoEditTriggers );

    ui->tableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    userMenu = new QMenu(ui->tableWidget);
    addusrAction = new QAction(tr("添加用户"),this);
    edtusrAction = new QAction(tr("编辑用户"),this);
    delusrAction = new QAction(tr("删除用户"),this);

   connect(addusrAction, SIGNAL(triggered()), this, SLOT(add_user_action()));
   connect(edtusrAction, SIGNAL(triggered()), this, SLOT(edit_user_action()));
   connect(delusrAction, SIGNAL(triggered()), this, SLOT(del_user_action()));

    UpdateToUsersUI();

    //about service manager
    ui->svrTableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->svrTableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QHeaderView* headerView1 = ui->svrTableWidget->verticalHeader();
    headerView1->setHidden(true);
    ui->svrTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->svrTableWidget->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    ui->svrTableWidget->setEditTriggers ( QAbstractItemView::NoEditTriggers );

    //time_t t1 = time(NULL);
    svrcfgMap.insert(ENABLE, tr("开机启动"));
    svrcfgMap.insert(DISABLE, tr("不启动"));
    svrcfgMap.insert(STATIC, tr("依赖服务"));

    runstatMap.insert(RUNNING, tr("活跃运行态"));
    runstatMap.insert(EXIT, tr("活跃退出态"));
    runstatMap.insert(DEAD, tr("未运行"));
    runstatMap.insert(OTHER, tr("异常不可用"));

    svrMenu = new QMenu(ui->svrTableWidget);
    upAction = new QAction(tr("设置开机启动"),this);
    svrCtrlAction = new QAction(tr("开启服务"),this);
    connect(upAction, SIGNAL(triggered()), this, SLOT(set_up_down_when_start()));
    connect(svrCtrlAction, SIGNAL(triggered()), this, SLOT(start_stop_service()));

    noShowSvrs<<"auditd.service";

    logw = new LogMainWindow(this);
    ui->stackedWidget->addWidget(logw);
    //kernal param set
    kerParamWidget = new KernParmMngWidget;
    ui->stackedWidget->addWidget(kerParamWidget);

    thread = new QThread;

    m_sysFunModel.moveToThread(thread);

    qRegisterMetaType<SEVLIST> ("SEVLIST");
    qRegisterMetaType<Exception>("Exception");
    //获取服务列表
    connect(this, SIGNAL(emitGetServsSignal(SEVLIST )), &m_sysFunModel, SLOT(getServicesSlot(SEVLIST )));
    connect(&m_sysFunModel, SIGNAL(emitGetServicesDone(int ,Exception, SEVLIST)), this, SLOT(getServicesSlot(int ,Exception, SEVLIST)));
    //设置开机启动
    connect(this, SIGNAL(emitSetUpDownWhenBoot(QString, int)), &m_sysFunModel, SLOT(setUpDownWhenBootSlot(QString,int)));
    connect(&m_sysFunModel, SIGNAL(emitSetUpDownWhenBootDone(int,Exception)), this, SLOT(setUpDownWhenBootSlot(int ,Exception)));
    //开启或关闭服务
    connect(this, SIGNAL(emitStartOrStopService(QString, int)), &m_sysFunModel, SLOT(startOrStopServiceSlot(QString,int)));
    connect(&m_sysFunModel, SIGNAL(emitStartOrStopServiceDone(int,Exception)), this, SLOT(startOrStopServiceSlot(int ,Exception)));

    thread->start();
   // waitD = new WaitDialog(this);
}

void TabSysPage::getServicesSlot(int res, Exception exp, SEVLIST svs)     //获取用户列表线程结束，处理结果
{
    sevrs = svs;
    waitD->accept();
    if(res == 0)
    {
        QList<ServiceInfo> sevrstmp;
        for(int i=0;i<sevrs.size();i++)
        {
            if(noShowSvrs.contains(sevrs[i].sName))
                continue;
            sevrstmp.append(sevrs[i]);
        }
        sevrs = sevrstmp;
        UpdateToSvrUI();
        is_first = false;
    }else
        {
        errMsgBox(exp.getErroWhat());
        is_first = true;
    }
}
void TabSysPage::setUpDownWhenBootSlot(int res, Exception exp)
{
    waitD->accept();
    int row = ui->svrTableWidget->currentRow();
    if(row<0)
    {
        errMsgBox(tr("请选择要操作的服务!"));
        return;
    }
    if(res == 0)
    {
        if(upAction->text()==tr("设置开机启动"))
            sevrs[row].cfgStatus = ENABLE;
        else
            sevrs[row].cfgStatus = DISABLE;
       infoMsgBox(tr("设置成功"));
    }else
        {
        errMsgBox(exp.getErroWhat());
    }
    UpdateToSvrUI();
}

void TabSysPage::startOrStopServiceSlot(int res, Exception exp)
{
    waitD->accept();
    int row = ui->svrTableWidget->currentRow();
    if(row<0)
    {
        errMsgBox(tr("请选择要操作的服务!"));
        return;
    }
    if(res==0)
    {
        if(svrCtrlAction->text()==tr("关闭运行"))
        {
               infoMsgBox(tr("设置成功"));
               sevrs[row].runStat = DEAD;
               UpdateToSvrUI();
        }else
        {
                //sevrs[row].runStat = is_serv_running(sevrs[row].sName);
                //更新整个列表
                emit emitGetServsSignal(sevrs);
                 waitDiaogAppear();
        }
    }else
     {
        errMsgBox(exp.getErroWhat());
    }
}

void TabSysPage::listRowChangedSlot(int page)
{
    if(page == 1)       //servies
    {
        if(is_first)
        {
            emit emitGetServsSignal(sevrs);
             waitDiaogAppear();
       }
    }
}

void TabSysPage::waitDiaogAppear()
{
    waitD = new WaitDialog(this);
    waitD->exec();
    waitD->deleteLater();
}

void TabSysPage::set_up_down_when_start()
{
    int row = ui->svrTableWidget->currentRow();
    if(row<0)
    {
        errMsgBox(tr("请选择要操作的服务!"));
        return;
    }

    if(upAction->text()==tr("设置开机启动"))
    {
        emit emitSetUpDownWhenBoot(sevrs[row].sName, 0);
    }else
    {
        emit emitSetUpDownWhenBoot(sevrs[row].sName, 1);
    }
    waitDiaogAppear();
}

void TabSysPage::start_stop_service()
{
    int row = ui->svrTableWidget->currentRow();
    if(row<0)
    {
        errMsgBox(tr("请选择要操作的服务!"));
        return;
    }

    if(svrCtrlAction->text()==tr("关闭运行"))
    {
        emit emitStartOrStopService(sevrs[row].sName.left(sevrs[row].sName.indexOf('.')),1);
    }else
    {
        emit emitStartOrStopService(sevrs[row].sName.left(sevrs[row].sName.indexOf('.')),0);
    }
     waitDiaogAppear();
}


void TabSysPage::UpdateToSvrUI()
{
    ui->svrTableWidget->setRowCount(0);
    ui->svrTableWidget->setRowCount(sevrs.size());
    for(int i=0;i<sevrs.size();i++)
    {
       // qDebug()<<svrcfgMap[sevrs[i].cfgStatus]<<" "<<runstatMap[sevrs[i].runStat];
        ui->svrTableWidget->setItem(i, 0, new QTableWidgetItem(sevrs[i].sName));
        ui->svrTableWidget->setItem(i, 1, new QTableWidgetItem(svrcfgMap[sevrs[i].cfgStatus]));
        switch (sevrs[i].cfgStatus) {
        case ENABLE:
            ui->svrTableWidget->item(i, 1)->setForeground(Qt::green);
            break;
        case DISABLE:
            ui->svrTableWidget->item(i, 1)->setForeground(Qt::red);
            break;
        case STATIC:
            ui->svrTableWidget->item(i, 1)->setForeground(Qt::blue);
            break;
        default:
            break;
        }
        ui->svrTableWidget->setItem(i, 2, new QTableWidgetItem(runstatMap[sevrs[i].runStat]));
        switch (sevrs[i].runStat) {
        case RUNNING:
            ui->svrTableWidget->item(i, 2)->setForeground(Qt::green);
            break;
        case EXIT:
            ui->svrTableWidget->item(i, 2)->setForeground(Qt::green);
            break;
        case DEAD:
            ui->svrTableWidget->item(i, 2)->setForeground(Qt::red);
            break;
        case OTHER:
             ui->svrTableWidget->item(i, 2)->setForeground(Qt::gray);
            break;
        default:
            break;
        }
    }
}

void TabSysPage::UpdateToUsersUI()
{
    try
    {
        m_sysFunModel.getUserList(users);
        ui->tableWidget->setRowCount(0);
        ui->tableWidget->setRowCount(users.size());
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
            if(!users[i].isShow)
                ui->tableWidget->setRowHidden(i, true);
        }
    }catch(Exception exp)
            {
        errMsgBox(exp.getErroWhat());
    }
}

void TabSysPage::on_tableWidget_customContextMenuRequested(QPoint pos)
{
    userMenu->addAction(addusrAction);
    userMenu->addAction(edtusrAction);
    userMenu->addAction(delusrAction);
    userMenu->exec(QCursor::pos());
    pos.isNull();
}

void TabSysPage::on_svrTableWidget_customContextMenuRequested(QPoint pos)
{
    pos.isNull();
    int row = ui->svrTableWidget->currentRow();
    if(row<0)
        return;
    upAction->setEnabled(true);
    switch (sevrs[row].cfgStatus) {
    case ENABLE:
        upAction->setText(tr("取消开机启动"));
        break;
    case DISABLE:
        upAction->setText(tr("设置开机启动"));
        break;
    case STATIC:
        upAction->setText(tr("启动项不可更改"));
        upAction->setEnabled(false);
    default:
        break;
    }


    svrCtrlAction->setEnabled(true);
    switch (sevrs[row].runStat) {
    case RUNNING:
        svrCtrlAction->setText(tr("关闭运行"));
        break;
    case EXIT:
        svrCtrlAction->setText(tr("关闭运行"));
        break;
    case DEAD:
        svrCtrlAction->setText(tr("开启运行"));
        break;
    case OTHER:
        svrCtrlAction->setEnabled(false);
        svrCtrlAction->setText(tr("开启运行"));
        break;
    default:
        break;
    }
    svrMenu->addAction(upAction);
    svrMenu->addAction(svrCtrlAction);
    svrMenu->exec(QCursor::pos());
}


void TabSysPage::add_user_action()
{
       UserInfo userinfo;
       UserInfoDialog usrdialog(userinfo, Add,this);
       if(usrdialog.exec()==QDialog::Accepted)
       {
                users.append(userinfo);
                UpdateToUsersUI();
       }
}

void TabSysPage::edit_user_action()
{
    int row = ui->tableWidget->currentRow();
    if(row<0)
    {
        errMsgBox(tr("请选中要编辑的行"));
        return;
    }
    UserInfoDialog usrdialog(users[row], Edt);
    if(usrdialog.exec()==QDialog::Accepted)
    {
             UpdateToUsersUI();
    }

}
void TabSysPage::del_user_action()
{
    int row = ui->tableWidget->currentRow();
    if(row<0)
    {
        errMsgBox( tr("请选中要删除的行"));
        return;
    }
    try
    {
        m_sysFunModel.delUser(users[row]);
        UpdateToUsersUI();
    }catch(Exception exp)
            {
        errMsgBox(exp.getErroWhat());
    }
}


TabSysPage::~TabSysPage()
{
    delete ui;
}
