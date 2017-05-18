#include "mainwindow.h"
#include "ui_mainwindow.h"

#include<stdio.h>
#include<QString>
#include<QStringList>
#include<QDesktopWidget>
#include"commtab.h"
#include"qreadconfig.h"
#include "sysadmform.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent ),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    initTitleBar();
    QReadConfig::getInstance()->readConfigFile();
    this->setWindowFlags(Qt::FramelessWindowHint);
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);    // 设置尺寸属性
    setMouseTracking(true);    // 界面拉伸需要这个属性
	//获取当前用户角色，如果失败，捕获异常，提示错误信息
    initCentralWidget();
}

void MainWindow::initCentralWidget()
{
    //ui->tabWidget->setHidden(true);
    try
    {
        m_curRole = MainFunClass::getInstance()->getUserRole();
        if(m_curRole == ERROR)
        {
            m_curRole = AUDADMIN;
        }
        //m_curRole = SECADMIN;

        if(isTab)
        {
            ui->stackedWidget->setHidden(true);
            switch(m_curRole)
            {
            case ROOT:
                ui->stackedWidget->setHidden(false);
                tabSecrityPage = new TabSecrityPage(ui->tabWidget);
                tabSysPage = new TabSysPage(ui->tabWidget);
                tabAuditPage = new TabAuditPage(ui->tabWidget);
                indexWidget = new IndexWidget(m_curRole,ui->tabWidget);

                ui->tabWidget->addTab(indexWidget, tr("首页"));
                ui->tabWidget->addTab(tabSecrityPage, tr("安全管理员"));
                ui->tabWidget->addTab(tabSysPage, tr("系统管理员"));
                ui->tabWidget->addTab(tabAuditPage, tr("审计管理员"));
                ui->tabWidget->setCurrentIndex(0);
                connect(ui->tabWidget, SIGNAL(currentChanged(int )), this, SLOT(tabChanged(int )));
                break;
            case SECADMIN:
                secTabWidget = new SecTabWidget(ui->centralwidget);
                secTabWidget->setObjectName("secTabWidget");
                ui->verticalLayout->addWidget(secTabWidget);
                connect(secTabWidget, SIGNAL(currentChanged(int )), this, SLOT(tabChanged(int )));
                break;
            case SYSADMIN:
                sysTabWidget = new SysTabWidget(ui->centralwidget);
                sysTabWidget->setObjectName("sysTabWidget");
                ui->verticalLayout->addWidget(sysTabWidget);
                connect(sysTabWidget, SIGNAL(currentChanged(int )), this, SLOT(tabChanged(int )));
                break;
            case AUDADMIN:
                audTabWidget = new AudTabWidget(ui->centralwidget);
                audTabWidget->setObjectName("audTabWidget");
                ui->verticalLayout->addSpacing(6);
                ui->verticalLayout->addWidget(audTabWidget);
                connect(audTabWidget, SIGNAL(currentChanged(int )), this, SLOT(tabChanged(int )));
                break;
            default:
                QTabWidget *tableWiget = new QTabWidget(ui->centralwidget);
                QLabel *label = new QLabel(tableWiget);
                QFont font;
                font.setPointSize(22);
                font.setFamily("Cantarell");
                label->setFont(font);
                label->setText(tr("无法获取用户权限或该用户无使用安全管理中心的权限!"));
                ui->verticalLayout->addWidget(tableWiget);
                break;
            }
        }else
        {
            switch(m_curRole)
            {
            case ROOT:
                ui->tabWidget->setHidden(false);
                tabSecrityPage = new TabSecrityPage(ui->tabWidget);
                tabSysPage = new TabSysPage(ui->tabWidget);
                tabAuditPage = new TabAuditPage(ui->tabWidget);
                indexWidget = new IndexWidget(m_curRole,ui->tabWidget);

                ui->tabWidget->addTab(indexWidget, tr("首页"));
                ui->tabWidget->addTab(tabSecrityPage, tr("安全管理员"));
                ui->tabWidget->addTab(tabSysPage, tr("系统管理员"));
                ui->tabWidget->addTab(tabAuditPage, tr("审计管理员"));
                ui->tabWidget->setCurrentIndex(0);
                connect(ui->tabWidget, SIGNAL(currentChanged(int )), this, SLOT(tabChanged(int )));
                break;
            case SECADMIN:
                secTabWidget = new SecTabWidget(ui->centralwidget);
                secTabWidget->setObjectName("secTabWidget");
                ui->verticalLayout->addWidget(secTabWidget);
                connect(secTabWidget, SIGNAL(currentChanged(int )), this, SLOT(tabChanged(int )));
                break;
            case SYSADMIN:
               { SysAdmForm *sysadm = new SysAdmForm;
                ui->stackedWidget->addWidget(sysadm);
                ui->stackedWidget->setCurrentWidget(sysadm);
                }
                break;
            case AUDADMIN:
                audTabWidget = new AudTabWidget(ui->centralwidget);
                audTabWidget->setObjectName("audTabWidget");
                ui->verticalLayout->addWidget(audTabWidget);
                connect(audTabWidget, SIGNAL(currentChanged(int )), this, SLOT(tabChanged(int )));
                break;
            default:
                QTabWidget *tableWiget = new QTabWidget(ui->centralwidget);
                QLabel *label = new QLabel(tableWiget);
                QFont font;
                font.setPointSize(22);
                font.setFamily("Cantarell");
                label->setFont(font);
                label->setText(tr("无法获取用户权限或该用户无使用安全管理中心的权限!"));
                ui->verticalLayout->addWidget(tableWiget);
                break;
            }
        }

    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat());
    }
}

void MainWindow::tabChanged(int index)
{
    if(index == 0)
    {
        //ui->label->setVisible(true);
        if(sender()->objectName()=="tabWidget")
        {
            indexWidget->initUi();
        }else if(sender()->objectName()=="secTabWidget")
        {
            secTabWidget->UpdateIndex();
        }else if(sender()->objectName()=="sysTabWidget")
        {
            sysTabWidget->UpdateIndex();
        }else if(sender()->objectName()=="audTabWidget")
        {
            audTabWidget->UpdateIndex();
        }

    }
    else
        ;
        //ui->label->setVisible(false);
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
