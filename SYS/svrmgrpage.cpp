#include "svrmgrpage.h"
#include "ui_svrmgrpage.h"

SvrMgrPage::SvrMgrPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SvrMgrPage)
{
    is_first =true;
    ui->setupUi(this);
    ui->svrTableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->svrTableWidget->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    ui->svrTableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    QHeaderView* headerView1 = ui->svrTableWidget->verticalHeader();
    headerView1->setHidden(true);
    ui->svrTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->svrTableWidget->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    ui->svrTableWidget->setEditTriggers ( QAbstractItemView::NoEditTriggers );
    QHeaderView *headerGoods = ui->svrTableWidget->horizontalHeader();
    //SortIndicator为水平标题栏文字旁边的三角指示器
    headerGoods->setSortIndicator(0, Qt::AscendingOrder);
    headerGoods->setSortIndicatorShown(true);
    headerGoods->setClickable(true);
    connect(headerGoods, SIGNAL(sectionClicked(int)), ui->svrTableWidget, SLOT (sortByColumn(int)));

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
    svrRestartAction =  new QAction(tr("重启服务"),this);
    freshListAction = new QAction(tr("刷新列表"),this);
    connect(upAction, SIGNAL(triggered()), this, SLOT(set_up_down_when_start()));
    connect(svrCtrlAction, SIGNAL(triggered()), this, SLOT(start_stop_service()));
    connect(svrRestartAction, SIGNAL(triggered()), this, SLOT(restart_service()));
    connect(freshListAction, SIGNAL(triggered()), this, SLOT(fresh_services()));

    connect(ui->setBootButton, SIGNAL(clicked()), this, SLOT(set_up_down_when_start()));
    connect(ui->openSvrButton, SIGNAL(clicked()), this, SLOT(start_stop_service()));
    connect(ui->restartSvrButton, SIGNAL(clicked()), this, SLOT(restart_service()));
    connect(ui->freshButton, SIGNAL(clicked()), this, SLOT(fresh_services()));

    noShowSvrs<<"auditd";

    thread = new QThread;

    SysFunClass::getInstance()->moveToThread(thread);

    qRegisterMetaType<SEVLIST> ("SEVLIST");
    qRegisterMetaType<Exception>("Exception");
    //获取服务列表
    connect(this, SIGNAL(emitGetServsSignal(SEVLIST )), SysFunClass::getInstance(), SLOT(getServicesSlot(SEVLIST )));
    connect(SysFunClass::getInstance(), SIGNAL(emitGetServicesDone(int ,Exception, SEVLIST)), this, SLOT(getServicesSlot(int ,Exception, SEVLIST)));
    //设置开机启动
    connect(this, SIGNAL(emitSetUpDownWhenBoot(QString, int)), SysFunClass::getInstance(), SLOT(setUpDownWhenBootSlot(QString,int)));
    connect(SysFunClass::getInstance(), SIGNAL(emitSetUpDownWhenBootDone(int,Exception)), this, SLOT(setUpDownWhenBootSlot(int ,Exception)));
    //开启或关闭服务
    connect(this, SIGNAL(emitStartOrStopService(QString, int)), SysFunClass::getInstance(), SLOT(startOrStopServiceSlot(QString,int)));
    connect(SysFunClass::getInstance(), SIGNAL(emitStartOrStopServiceDone(int,Exception)), this, SLOT(startOrStopServiceSlot(int ,Exception)));

    thread->start();
}

void SvrMgrPage::getAllServices()
{
    if(is_first)
    {
        ui->svrTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Interactive);
        ui->svrTableWidget->horizontalHeader()->resizeSection(0, 180);
        ui->svrTableWidget->horizontalHeader()->resizeSection(1, 80);
        ui->svrTableWidget->horizontalHeader()->resizeSection(2, 240);
        ui->svrTableWidget->horizontalHeader()->resizeSection(3, 120);
        ui->svrTableWidget->horizontalHeader()->resizeSection(4, 120);
        emit emitGetServsSignal(sevrs);
         waitDiaogAppear();
   }
}

void SvrMgrPage::on_svrTableWidget_customContextMenuRequested(QPoint pos)
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
    svrRestartAction->setEnabled(true);
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
        svrRestartAction->setEnabled(false);
        svrCtrlAction->setText(tr("开启运行"));
        break;
    default:
        break;
    }
    svrMenu->addAction(upAction);
    svrMenu->addAction(svrCtrlAction);
    svrMenu->addAction(svrRestartAction);
    svrMenu->addAction(freshListAction);
    svrMenu->exec(QCursor::pos());
}

void SvrMgrPage::getServicesSlot(int res, Exception exp, SEVLIST svs)     //获取用户列表线程结束，处理结果
{
    sevrs = svs;
    waitDialogAccept();
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
        errMsgBox(exp.getErroWhat(), this);
        is_first = true;
    }
}
void SvrMgrPage::setUpDownWhenBootSlot(int res, Exception exp)
{
    waitDialogAccept();
    int row = ui->svrTableWidget->currentRow();
    if(row<0)
    {
        errMsgBox(tr("请选择要操作的服务!"), this);
        return;
    }
    if(res == 0)
    {
        if(/*upAction->text()==tr("设置开机启动")*/sevrs[row].cfgStatus==DISABLE)
            sevrs[row].cfgStatus = ENABLE;
        else if(sevrs[row].cfgStatus == ENABLE)
            sevrs[row].cfgStatus = DISABLE;

       infoMsgBox(tr("设置成功"), this);
    }else
        {
        errMsgBox(exp.getErroWhat(), this);
    }
    UpdateToSvrUI();
}

void SvrMgrPage::startOrStopServiceSlot(int res, Exception exp)
{
    waitDialogAccept();
    int row = ui->svrTableWidget->currentRow();
    if(row<0)
    {
        errMsgBox(tr("请选择要操作的服务!"), this);
        return;
    }
    if(res==0)
    {
//        if(svrCtrlAction->text()==tr("关闭运行"))
//        {
//               infoMsgBox(tr("设置成功"));
//               sevrs[row].runStat = DEAD;
//               UpdateToSvrUI();
//        }else
        {
                //sevrs[row].runStat = is_serv_running(sevrs[row].sName);
                //更新整个列表
                emit emitGetServsSignal(sevrs);
                 waitDiaogAppear();
        }
    }else
     {
        errMsgBox(exp.getErroWhat(), this);
    }
}


void SvrMgrPage::UpdateToSvrUI()
{
    ui->svrTableWidget->setRowCount(0);
    ui->svrTableWidget->setRowCount(sevrs.size());
    ui->svrTableWidget->setToolTip(tr("右键可对服务进行操作"));
    for(int i=0;i<sevrs.size();i++)
    {
       // qDebug()<<svrcfgMap[sevrs[i].cfgStatus]<<" "<<runstatMap[sevrs[i].runStat];
        ui->svrTableWidget->setItem(i, 0, new QTableWidgetItem(sevrs[i].sName));
        ui->svrTableWidget->setItem(i, 1, new QTableWidgetItem(sevrs[i].pId));
        ui->svrTableWidget->setItem(i, 2, new QTableWidgetItem(sevrs[i].desc));
        ui->svrTableWidget->setItem(i, 3, new QTableWidgetItem(svrcfgMap[sevrs[i].cfgStatus]));
        switch (sevrs[i].cfgStatus) {
        case ENABLE:
            ui->svrTableWidget->item(i, 3)->setForeground(Green);
            break;
        case DISABLE:
            ui->svrTableWidget->item(i, 3)->setForeground(Red);
            break;
        case STATIC:
            ui->svrTableWidget->item(i, 3)->setForeground(Blue);
            break;
        default:
            break;
        }
        ui->svrTableWidget->setItem(i, 4, new QTableWidgetItem(runstatMap[sevrs[i].runStat]));
        switch (sevrs[i].runStat) {
        case RUNNING:
            ui->svrTableWidget->item(i, 4)->setForeground(Green);
            break;
        case EXIT:
            ui->svrTableWidget->item(i, 4)->setForeground(Green);
            break;
        case DEAD:
            ui->svrTableWidget->item(i, 4)->setForeground(Red);
            break;
        case OTHER:
             ui->svrTableWidget->item(i, 4)->setForeground(Qt::gray);
            break;
        default:
            break;
        }

        if(/*sevrs[i].pId.isEmpty()|| */sevrs[i].desc.isEmpty())
        {
            ui->svrTableWidget->setRowHidden(i, true);
        }
    }
}

void SvrMgrPage::waitDialogAccept()
{
    waitD->accept();
    delete waitD;
}

void SvrMgrPage::waitDiaogAppear()
{
    waitD = new WaitDialog(this);
    waitD->exec();
    //waitD->deleteLater();
}

void SvrMgrPage::set_up_down_when_start()
{
    int row = ui->svrTableWidget->currentRow();
    if(row<0)
    {
        errMsgBox(tr("请选择要操作的服务!"), this);
        return;
    }

    if(/*upAction->text()==tr("设置开机启动")*/sevrs[row].cfgStatus==DISABLE)
    {
        emit emitSetUpDownWhenBoot(sevrs[row].sName, 0);
    }else if(sevrs[row].cfgStatus==ENABLE)
    {
        emit emitSetUpDownWhenBoot(sevrs[row].sName, 1);
    }else
        {
        infoMsgBox(tr("启动项不可更改"), this);
        return;
    }
    waitDiaogAppear();
}

void SvrMgrPage::fresh_services()
{
    is_first = true;
    getAllServices();
}
 void SvrMgrPage::restart_service()
 {
     int row = ui->svrTableWidget->currentRow();
     if(row<0)
     {
         errMsgBox(tr("请选择要操作的服务!"), this);
         return;
     }

     emit emitStartOrStopService(sevrs[row].sName.left(sevrs[row].sName.indexOf('.')),2);
     waitDiaogAppear();
 }

void SvrMgrPage::start_stop_service()
{
    int row = ui->svrTableWidget->currentRow();
    if(row<0)
    {
        errMsgBox(tr("请选择要操作的服务!"), this);
        return;
    }

    if(/*svrCtrlAction->text()==tr("关闭运行")*/sevrs[row].runStat==RUNNING||sevrs[row].runStat==EXIT)
    {
        emit emitStartOrStopService(sevrs[row].sName.left(sevrs[row].sName.indexOf('.')),1);
    }else if(sevrs[row].runStat==DEAD)
    {
        emit emitStartOrStopService(sevrs[row].sName.left(sevrs[row].sName.indexOf('.')),0);
    }
     waitDiaogAppear();
}


SvrMgrPage::~SvrMgrPage()
{
    delete ui;
}

void SvrMgrPage::on_svrTableWidget_clicked(const QModelIndex &index)
{
    int row = index.row();
    if(row<0)
        return;
//    switch (sevrs[row].cfgStatus) {
//    case ENABLE:
//        ui->setBootButton->setText(tr("取消开机启动"));
//        break;
//    case DISABLE:
//        upAction->setText(tr("设置开机启动"));
//        break;
//    case STATIC:
//        upAction->setText(tr("启动项不可更改"));
//        upAction->setEnabled(false);
//    default:
//        break;
}
