#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<stdio.h>
#include<QString>
#include<QStringList>
#include<QDesktopWidget>
#include"commtab.h"
#include"qreadconfig.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent ),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    initTitleBar();
    //ui->label->setHidden(true);
    QReadConfig::getInstance()->readConfigFile();
    this->setWindowFlags(Qt::FramelessWindowHint);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);    // 设置尺寸属性
    setMouseTracking(true);    // 界面拉伸需要这个属性
	//获取当前用户角色，如果失败，捕获异常，提示错误信息
    try
    {
        m_curRole = m_MainModel.getUserRole();
        if(m_curRole == ERROR)
        {
            m_curRole = ROOT;
        }
        switch(m_curRole)
        {
        case ROOT:
            tabSecrityPage = new TabSecrityPage(ui->tabWidget);
            tabSysPage = new TabSysPage(ui->tabWidget);
            tabAuditPage = new TabAuditPage(ui->tabWidget);
            indexWidget = new IndexWidget(m_curRole,ui->tabWidget);

            ui->tabWidget->addTab(indexWidget, tr("首页"));
            ui->tabWidget->addTab(tabSecrityPage, tr("安全管理员"));
            ui->tabWidget->addTab(tabSysPage, tr("系统管理员"));
            ui->tabWidget->addTab(tabAuditPage, tr("审计管理员"));
            break;
        case SECADMIN:
            tabSecrityPage = new TabSecrityPage(ui->tabWidget);
            indexWidget = new IndexWidget(m_curRole,ui->tabWidget);

            ui->tabWidget->addTab(indexWidget, tr("首页"));
            ui->tabWidget->addTab(tabSecrityPage, tr("安全管理员"));
            break;
        case SYSADMIN:
            tabSysPage = new TabSysPage(ui->tabWidget);
            indexWidget = new IndexWidget(m_curRole,ui->tabWidget);

            ui->tabWidget->addTab(indexWidget, tr("首页"));
            ui->tabWidget->addTab(tabSysPage, tr("系统管理员"));
            break;
        case AUDIADMIN:
            tabAuditPage = new TabAuditPage(ui->tabWidget);
            indexWidget = new IndexWidget(m_curRole,ui->tabWidget);

            ui->tabWidget->addTab(indexWidget, tr("首页"));
            ui->tabWidget->addTab(tabAuditPage, tr("审计管理员"));
            break;
        case ERROR:
            indexWidget = new IndexWidget(m_curRole,ui->tabWidget);
            ui->tabWidget->addTab(indexWidget, tr("首页"));
            break;
        default:
            break;
        }
        ui->tabWidget->setCurrentIndex(0);
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat());
    }

    connect(ui->tabWidget, SIGNAL(currentChanged(int )), this, SLOT(tabChanged(int )));

}

void MainWindow::tabChanged(int index)
{
    if(index == 0)
    {
        ui->label->setVisible(true);
        indexWidget->initUi();
    }
    else
        ui->label->setVisible(false);
}

void MainWindow::initTitleBar()
{
    m_titleBar = new MyTitleBar(this);
    m_titleBar->move(0, 0);
    m_titleBar->setTitleIcon(":/new/index/title");
    m_titleBar->setTitleContent(tr("安全管理中心"), 11);
    m_titleBar->setButtonType(MIN_MAX_BUTTON);
    connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
    connect(m_titleBar, SIGNAL(signalButtonRestoreClicked()), this, SLOT(onButtonRestoreClicked()));
    connect(m_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
    connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));

}

void MainWindow::paintEvent(QPaintEvent* event)
{
    //设置背景色;
//    QPainter painter(this);
//    QPainterPath pathBack;
//    pathBack.setFillRule(Qt::WindingFill);
//    pathBack.addRoundedRect(QRect(0, 0, this->width(), this->height()), 3, 3);
//    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
//    painter.fillPath(pathBack, QBrush(QColor(238, 223, 204)));

    return QWidget::paintEvent(event);
}

//void MainWindow::loadStyleSheet(const QString &sheetName)
//{
//    QFile file(":/Resources/" + sheetName + ".css");
//    file.open(QFile::ReadOnly);
//    if (file.isOpen())
//    {
//        QString styleSheet = this->styleSheet();
//        styleSheet += QLatin1String(file.readAll());
//        this->setStyleSheet(styleSheet);
//    }
//}

void MainWindow::onButtonMinClicked()
{
    showMinimized();
}

void MainWindow::onButtonRestoreClicked()
{
    QPoint windowPos;
    QSize windowSize;
    m_titleBar->getRestoreInfo(windowPos, windowSize);
    this->setGeometry(QRect(windowPos, windowSize));
}

void MainWindow::onButtonMaxClicked()
{
    m_titleBar->saveRestoreInfo(this->pos(), QSize(this->width(), this->height()));
    QRect desktopRect = QApplication::desktop()->availableGeometry();
    QRect FactRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
    setGeometry(FactRect);
}

void MainWindow::onButtonCloseClicked()
{
    close();
}

MainWindow::~MainWindow()
{
    delete ui;
}
