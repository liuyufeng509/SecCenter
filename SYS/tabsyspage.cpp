#include "tabsyspage.h"
#include "ui_tabsyspage.h"
#include"userinfodialog.h"
#include"time.h"
#include"logmainwindow.h"
TabSysPage::TabSysPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabSysPage)
{
    ui->setupUi(this);

    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
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

    UpdateToUI();

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

    get_services(sevrs);
    UpdateToSvrUI();
    //time_t t2 = time(NULL);
   // qDebug()<<t2-t1;



    //cpu
    cpu_num = get_n_cpu();
    QVBoxLayout * vlayout = new QVBoxLayout(ui->groupBox_4);
    for(int i=0; i<cpu_num; i++)
    {
        CPUGrap cpuGrap;
        cpuGrap.label = new QLabel(QString("CPU%1:0.0%").arg(i+1));
        cpuGrap.label->setMaximumSize(65,25);
        cpuGrap.label->setMinimumSize(65, 25);
        cpuGrap.progressBar = new QProgressBar;
        cpuGrap.progressBar->setRange(0, 100);
        cpuGrap.progressBar->setTextVisible(false);
        cpuGrap.progressBar->setValue(0);
        QHBoxLayout *hlayout = new QHBoxLayout;
        hlayout->addWidget(cpuGrap.progressBar);
        hlayout->addWidget(cpuGrap.label);
        cpuGraps.append(cpuGrap);
        vlayout->addLayout(hlayout);
    }
    browser = new QTextBrowser;
    vlayout->addWidget(browser);
     QHBoxLayout *hlayout = new QHBoxLayout;
    openCpuButton = new QPushButton(tr("开始监视"));
    clearLogButton = new QPushButton(tr("清空报警信息"));
    setWarnButton = new QPushButton(tr("设置报警边界"));
    hlayout->addWidget(openCpuButton);
    hlayout->addWidget(clearLogButton);
    hlayout->addWidget(setWarnButton);
    vlayout->addLayout(hlayout);
    QSpacerItem *verticalSpacer = new QSpacerItem(20, 189, QSizePolicy::Minimum, QSizePolicy::Expanding);
    vlayout->addItem(verticalSpacer);

    connect(openCpuButton, SIGNAL(clicked()), this, SLOT(openCputButtonClicked()));
    connect(clearLogButton, SIGNAL(clicked()), this, SLOT(clearLogButtonClicked()));
    connect(setWarnButton, SIGNAL(clicked()), this, SLOT(setWarnButtonClicked()));

    cpuTimer = new QTimer(this);
    connect(cpuTimer, SIGNAL(timeout()), this, SLOT(updateToCPU()));
    //cpuTimer->start(1000);

    QString path = getCwdPath()+"config.ini";
    configIniRead = new QSettings(path, QSettings::IniFormat);
    cpuWarning = configIniRead->value("CPU/warning").toInt();
    //hidden cpu function


    //mem and swap
    {
    memWarning = configIniRead->value("MEM/warning").toInt();
    swapWarning = configIniRead->value("SWAP/warning").toInt();

    QVBoxLayout * vmemAndSwaplayout = new QVBoxLayout(ui->groupBox_5);
    memLabel = new QLabel(QString("内存：0M(0.0%),共0G"));
    memLabel->setMaximumSize(160,25);
    memLabel->setMinimumSize(160, 25);
    memProgressBar = new QProgressBar;
    memProgressBar->setRange(0, 100);
    memProgressBar->setTextVisible(false);
    memProgressBar->setValue(0);
    {
        QHBoxLayout *hlayout = new QHBoxLayout;
        hlayout->addWidget(memProgressBar);
        hlayout->addWidget(memLabel);
        vmemAndSwaplayout->addLayout(hlayout);
    }

    swapLabel = new QLabel(QString("交换：0M(0.0%),共0G"));
    swapLabel->setMaximumSize(160,25);
    swapLabel->setMinimumSize(160, 25);
    swapProgressBar = new QProgressBar;
    swapProgressBar->setRange(0, 100);
    swapProgressBar->setTextVisible(false);
    swapProgressBar->setValue(0);
    {
        QHBoxLayout *hlayout = new QHBoxLayout;
        hlayout->addWidget(swapProgressBar);
        hlayout->addWidget(swapLabel);
        vmemAndSwaplayout->addLayout(hlayout);
    }


    memAndSwapBrowser = new QTextBrowser;
    vmemAndSwaplayout->addWidget(memAndSwapBrowser);

    {
        QHBoxLayout *hlayout = new QHBoxLayout;
        openMemSwapButton = new QPushButton(tr("开始监视"));
        clearMemSwapButton = new QPushButton(tr("清空报警信息"));
        setWarnMemSwapButton = new QPushButton(tr("设置报警边界"));
        hlayout->addWidget(openMemSwapButton);
        hlayout->addWidget(clearMemSwapButton);
        hlayout->addWidget(setWarnMemSwapButton);
        vmemAndSwaplayout->addLayout(hlayout);
    }
//    QSpacerItem *verticalSpacer1 = new QSpacerItem(20, 189, QSizePolicy::Minimum, QSizePolicy::Expanding);
//    vmemAndSwaplayout->addItem(verticalSpacer1);

    connect(openMemSwapButton, SIGNAL(clicked()), this, SLOT(openMemSwapButtonClicked()));
    connect(clearMemSwapButton, SIGNAL(clicked()), this, SLOT(clearMemSwapButtonClicked()));
    connect(setWarnMemSwapButton, SIGNAL(clicked()), this, SLOT(setWarnMemSwapButtonClicked()));

    memTimer = new QTimer(this);
    connect(memTimer, SIGNAL(timeout()), this, SLOT(updateToMem()));
    }

    //file system

    ui->fsTableWidget->horizontalHeader()->setStretchLastSection(true);
    QHeaderView* headerView2 = ui->fsTableWidget->verticalHeader();
    headerView2->setHidden(true);
    ui->fsTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->fsTableWidget->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    ui->fsTableWidget->setEditTriggers ( QAbstractItemView::NoEditTriggers );
    diskwarning = configIniRead->value("DISK/warning").toInt();
    UpdateToFS();

    int isHid = configIniRead->value("Hid/hidden").toInt();
    if(isHid)
    {
        ui->listWidget->item(4)->setHidden(true);
        ui->listWidget->item(3)->setHidden(true);
        ui->listWidget->item(2)->setHidden(true);
    }

    logw = new LogMainWindow(this);
}

void TabSysPage::openMemSwapButtonClicked()
{
    if(openMemSwapButton->text() == tr("开始监视"))
    {
        memTimer->start(1000);
        openMemSwapButton->setText(tr("关闭监视"));
    }else
    {
        memTimer->stop();
        openMemSwapButton->setText(tr("开始监视"));
        memProgressBar->setValue(0);
        memProgressBar->setStyleSheet("");
        memLabel->setText(QString("内存：0M(0.0%),共0G"));
        swapProgressBar->setValue(0);
        swapProgressBar->setStyleSheet("");
        swapLabel->setText(tr("交换：0M(0.0%),共0G"));
    }

}

void TabSysPage::clearMemSwapButtonClicked()
{
    memAndSwapBrowser->setText("");
}

void TabSysPage::setWarnMemSwapButtonClicked()
{
    QDialog *dialog = new QDialog(this);
    dialog->resize(200,100);
    QVBoxLayout *vlayout = new QVBoxLayout(dialog);
    QLineEdit *lineEdit = new QLineEdit(dialog);
    QLineEdit *lineEdit2 = new QLineEdit(dialog);
    lineEdit->setText(QString::number(memWarning));
    lineEdit2->setText(QString::number(swapWarning));
    QLabel *label = new QLabel(tr("内存:"), dialog);
    QLabel *label2 = new QLabel(tr("交换:"), dialog);
    {
        QHBoxLayout * hlayout = new QHBoxLayout;
        hlayout->addWidget(label);
        hlayout->addWidget(lineEdit);
        vlayout->addLayout(hlayout);
    }
    {
        QHBoxLayout * hlayout = new QHBoxLayout;
        hlayout->addWidget(label2);
        hlayout->addWidget(lineEdit2);
        vlayout->addLayout(hlayout);
    }
    QPushButton *ok = new QPushButton(tr("确定"),dialog);
    QPushButton *cancel = new QPushButton(tr("取消"),dialog);

    QHBoxLayout *hlayout = new QHBoxLayout(dialog);
    hlayout->addWidget(cancel);
    hlayout->addWidget(ok);
    vlayout->addLayout(hlayout);
    connect(ok, SIGNAL(clicked()), dialog, SLOT(accept()));
    connect(cancel, SIGNAL(clicked()), dialog, SLOT(reject()));

    if(dialog->exec()==QDialog::Accepted)
    {
        if(lineEdit->text().toInt()<=0|| lineEdit->text().toInt()>100 || lineEdit2->text().toInt()<=0|| lineEdit2->text().toInt()>100)
            QMessageBox::information(this, tr("提示"), tr("报警界限设置失败，必须为1~100之间"));
        else
        {
            QMessageBox::information(this, tr("提示"), tr("报警界限设置成功"));
            configIniRead->setValue("MEM/warning",lineEdit->text().toInt());
            configIniRead->setValue("SWAP/warning",lineEdit2->text().toInt());
             memWarning = lineEdit->text().toInt();
             swapWarning = lineEdit2->text().toInt();
        }
    }
    delete dialog;
}

void TabSysPage::openCputButtonClicked()
{
    if(openCpuButton->text() == tr("开始监视"))
    {
        cpuTimer->start(1000);
        openCpuButton->setText(tr("关闭监视"));
    }else
    {
        cpuTimer->stop();
        openCpuButton->setText(tr("开始监视"));
        for(int i=0; i<cpu_num;i++)
        {
            cpuGraps[i].progressBar->setValue(0);
            cpuGraps[i].progressBar->setStyleSheet("");
            cpuGraps[i].label->setText(QString("CPU%1:0.0%").arg(i+1));
        }
    }
}

void TabSysPage::clearLogButtonClicked()
{
    browser->setText("");
}

void TabSysPage::setWarnButtonClicked()
{
    QDialog *dialog = new QDialog(this);
    dialog->resize(200,100);
    QLineEdit *lineEdit = new QLineEdit(dialog);
    lineEdit->setText(QString::number(cpuWarning));
    QPushButton *ok = new QPushButton(tr("确定"),dialog);
    QPushButton *cancel = new QPushButton(tr("取消"),dialog);
    QVBoxLayout *vlayout = new QVBoxLayout(dialog);
    vlayout->addWidget(lineEdit);
    QHBoxLayout *hlayout = new QHBoxLayout(dialog);
    hlayout->addWidget(cancel);
    hlayout->addWidget(ok);
    vlayout->addLayout(hlayout);
    connect(ok, SIGNAL(clicked()), dialog, SLOT(accept()));
    connect(cancel, SIGNAL(clicked()), dialog, SLOT(reject()));

    if(dialog->exec()==QDialog::Accepted)
    {
        if(lineEdit->text().toInt()<=0|| lineEdit->text().toInt()>100)
            QMessageBox::information(this, tr("提示"), tr("报警界限设置失败，必须为1~100之间"));
        else
        {
            QMessageBox::information(this, tr("提示"), tr("报警界限设置成功"));
            configIniRead->setValue("CPU/warning",lineEdit->text().toInt());
            cpuWarning = lineEdit->text().toInt();
        }
    }
    delete dialog;
}

void TabSysPage::updateToMem()
{
    MeM mem;
    SWAP swap;
    getmem(mem);
    get_swap(swap);

    memLabel->setText(QString("内存：%1(%2%),共%3").arg(getHintNum(mem.used)).arg(mem.percent*100, 0, 'f', 1).arg(getHintNum(mem.total)));
    swapLabel->setText(QString("交换：%1(%2%),共%3").arg(getHintNum(swap.used)).arg(swap.percent*100, 0, 'f', 1).arg(getHintNum(swap.total)));
    memProgressBar->setValue(mem.percent*100);
    swapProgressBar->setValue(swap.percent*100);
    if(mem.percent*100>memWarning)
    {
        memProgressBar->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 0, 0) }");
        QApplication::beep();
        QString str = QString("<span style=\" color:red; \">[Warning:%1]内存利用率为%2%超过警戒线！！！</span>").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(mem.percent*100,0,'f',1);
        memAndSwapBrowser->append(str);
    }else
        memProgressBar->setStyleSheet("");

    if(swap.percent*100>swapWarning)
    {
        swapProgressBar->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 0, 0) }");
        QApplication::beep();
        QString str = QString("<span style=\" color:red; \">[Warning:%1]交换内存利用率为%2%超过警戒线！！！</span>").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(swap.percent*100,0,'f',1);
        memAndSwapBrowser->append(str);
    }else
        swapProgressBar->setStyleSheet("");
}

void TabSysPage::updateToCPU()
{
    std::vector<CPU> cpus;
    get_load(cpus);
    for(int i=0; i<cpu_num; i++)
    {
        cpuGraps[i].label->setText(QString("CPU%1: %2%").arg(i+1).arg(cpus[i].useage,0,'f',1));
        cpuGraps[i].progressBar->setValue(cpus[i].useage);
        if(cpus[i].useage>cpuWarning)
        {
            cpuGraps[i].progressBar->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 0, 0) }");
            QApplication::beep();
            QString str = QString("<span style=\" color:red; \">[Warning:%1]cpu%2利用率为%3%超过警戒线！！！</span>").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(i+1).arg(cpus[i].useage,0,'f',1);
            browser->append(str);
        }
        else
            cpuGraps[i].progressBar->setStyleSheet("");
    }
}

void TabSysPage::set_up_down_when_start()
{
    int row = ui->svrTableWidget->currentRow();
    if(row<0)
    {
        QMessageBox::information(this, tr("提示"), tr("请选择要操作的服务!"));
        return;
    }

    if(upAction->text()==tr("设置开机启动"))
    {
       if( up_service_when_start(sevrs[row].sName))
       {
           QMessageBox::information(this,  tr("提示"), tr("设置成功"));
           sevrs[row].cfgStatus = ENABLE;
       }else
           QMessageBox::information(this,  tr("提示"), tr("设置失败"));
    }else
    {
        if(down_service_when_start(sevrs[row].sName))
        {
            QMessageBox::information(this,  tr("提示"), tr("设置成功"));
            sevrs[row].cfgStatus = DISABLE;
        }else
            QMessageBox::information(this,  tr("提示"), tr("设置失败"));
    }

    UpdateToSvrUI();
}

void TabSysPage::start_stop_service()
{
    int row = ui->svrTableWidget->currentRow();
    if(row<0)
    {
        QMessageBox::information(this, tr("提示"), tr("请选择要操作的服务!"));
        return;
    }

    //qDebug()<<svrCtrlAction->text();
    if(svrCtrlAction->text()==tr("关闭运行"))
    {
       if( stop_service(sevrs[row].sName))
       {
           QMessageBox::information(this,  tr("提示"), tr("设置成功"));
           sevrs[row].runStat = DEAD;
       }else
           QMessageBox::information(this,  tr("提示"), tr("设置失败"));
    }else
    {
        if(start_service(sevrs[row].sName))
        {
            QMessageBox::information(this,  tr("提示"), tr("设置成功"));
            sevrs[row].runStat = is_serv_running(sevrs[row].sName);
        }else
            QMessageBox::information(this,  tr("提示"), tr("设置失败"));
    }

    UpdateToSvrUI();
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

void TabSysPage::UpdateToUI()
{
    set_userinfo(users);
    set_userinfos_groups(users);
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setRowCount(users.size());
    for(int i=0; i<users.length(); i++)
    {
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(users[i].uname));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(users[i].uid));
        if(users[i].othgroups.length()<1)
        {
             QString groups=users[i].group;
             ui->tableWidget->setItem(i, 2, new QTableWidgetItem(groups));
        }else
        {
            QString groups=users[i].group+ ":";
            for(int j=0; j<users[i].othgroups.length()-1;j++)
            {
                groups += users[i].othgroups[j]+":";
            }
            groups += users[i].othgroups[users[i].othgroups.length()-1];
            ui->tableWidget->setItem(i, 2, new QTableWidgetItem(groups));
        }
    }
}

void TabSysPage::on_tableWidget_customContextMenuRequested(QPoint pos)
{
    userMenu->addAction(addusrAction);
    userMenu->addAction(edtusrAction);
    userMenu->addAction(delusrAction);
    userMenu->exec(QCursor::pos());
}

void TabSysPage::on_svrTableWidget_customContextMenuRequested(QPoint pos)
{
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
                UpdateToUI();
       }
}

void TabSysPage::edit_user_action()
{
    int row = ui->tableWidget->currentRow();
    if(row<0)
    {
        QMessageBox::information(this, tr("提示"), tr("请选中要编辑的行"));
        return;
    }
    UserInfoDialog usrdialog(users[row], Edt);
    if(usrdialog.exec()==QDialog::Accepted)
    {
             UpdateToUI();
    }

}
void TabSysPage::del_user_action()
{
    int row = ui->tableWidget->currentRow();
    if(row<0)
    {
        QMessageBox::information(this, tr("提示"), tr("请选中要删除的行"));
        return;
    }

    del_user(users[row]);
    UpdateToUI();
}


TabSysPage::~TabSysPage()
{
    delete ui;
}

void TabSysPage::on_freshButton_clicked()
{
    UpdateToFS();
}

void TabSysPage::on_cleanwarnButton_clicked()
{
    ui->textBrowser->setText("");
}

void TabSysPage::on_setWarnButton_clicked()
{
    QDialog *dialog = new QDialog(this);
    dialog->resize(200,100);
    QLineEdit *lineEdit = new QLineEdit(dialog);
    lineEdit->setText(QString::number(diskwarning));
    QPushButton *ok = new QPushButton(tr("确定"),dialog);
    QPushButton *cancel = new QPushButton(tr("取消"),dialog);
    QVBoxLayout *vlayout = new QVBoxLayout(dialog);
    vlayout->addWidget(lineEdit);
    QHBoxLayout *hlayout = new QHBoxLayout(dialog);
    hlayout->addWidget(cancel);
    hlayout->addWidget(ok);
    vlayout->addLayout(hlayout);
    connect(ok, SIGNAL(clicked()), dialog, SLOT(accept()));
    connect(cancel, SIGNAL(clicked()), dialog, SLOT(reject()));

    if(dialog->exec()==QDialog::Accepted)
    {
        if(lineEdit->text().toInt()<=0|| lineEdit->text().toInt()>100)
            QMessageBox::information(this, tr("提示"), tr("报警界限设置失败，必须为1~100之间"));
        else
        {
            QMessageBox::information(this, tr("提示"), tr("报警界限设置成功"));
            configIniRead->setValue("DISK/warning",lineEdit->text().toInt());
            diskwarning = lineEdit->text().toInt();
        }
    }
    delete dialog;
}

void TabSysPage::UpdateToFS()
{
    get_fsInfo(disks);

    ui->fsTableWidget->setRowCount(0);
    ui->fsTableWidget->setRowCount(disks.size());

    for(int i=0;i<disks.size();i++)
    {
        ui->fsTableWidget->setItem(i, 0, new QTableWidgetItem(disks[i].devName));
        ui->fsTableWidget->setItem(i, 1, new QTableWidgetItem(disks[i].mountDir));
        ui->fsTableWidget->setItem(i, 2, new QTableWidgetItem(disks[i].ftype));
        ui->fsTableWidget->setItem(i, 3, new QTableWidgetItem(getHintNum(disks[i].btotal)));
        ui->fsTableWidget->setItem(i, 4, new QTableWidgetItem(getHintNum(disks[i].bavail)));
        ui->fsTableWidget->setItem(i, 5, new QTableWidgetItem(getHintNum(disks[i].used)));
        ui->fsTableWidget->setItem(i, 6, new QTableWidgetItem(QString("")));
        QProgressBar *progress = new QProgressBar;
        progress->setRange(0,100);
        progress->setValue(disks[i].percent);
        if(disks[i].percent>diskwarning)
        {
            progress->setStyleSheet("QProgressBar::chunk { background-color: rgb(255, 0, 0) }");
            ui->textBrowser->append(QString("<span style=\" color:red; \">[Warning:%1]硬盘%2占用率为%3%超过警戒线！！！</span>").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")).arg(disks[i].devName).arg(disks[i].percent));
        }else
            progress->setStyleSheet("");
        ui->fsTableWidget->setCellWidget(i, 6, progress);
    }
}

void TabSysPage::on_shutdowButton_clicked()
{
    QMessageBox::StandardButton rb = QMessageBox::question(this, tr("警告"), tr("将要进行关机操作，请确保数据全部保存"),QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
        shutdown();
}

void TabSysPage::on_logoutButton_clicked()
{
    QMessageBox::StandardButton rb = QMessageBox::question(this, tr("警告"), tr("将要进行注销操作，请确保数据全部保存"),QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
        logout();
}

void TabSysPage::on_rebootButton_clicked()
{
    QMessageBox::StandardButton rb = QMessageBox::question(this, tr("警告"), tr("将要进行重启操作，请确保数据全部保存"),QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if(rb == QMessageBox::Yes)
        reboot();
}

void TabSysPage::on_logButton_clicked()
{
    logw->show();
}
