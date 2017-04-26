#ifndef TABSYSPAGE_H
#define TABSYSPAGE_H

#include <QWidget>
#include"common.h"
#include"logmainwindow.h"
#include"kernalparammanager.h"
#include "sysfunclass.h"
#include<QThread>
#include "waitdialog.h"

namespace Ui {
class TabSysPage;
}

class TabSysPage : public QWidget
{
    Q_OBJECT

public:
    explicit TabSysPage(QWidget *parent = 0);
    ~TabSysPage();

    void UpdateToUsersUI();     //更新用户管理列表的界面
    void UpdateToSvrUI();

    void waitDiaogAppear();
public slots:
    void on_tableWidget_customContextMenuRequested(QPoint pos);             //用户管理右键菜单
    void on_svrTableWidget_customContextMenuRequested(QPoint pos);

    void add_user_action();             //添加用户
    void edit_user_action();             //编辑用户
    void del_user_action();              //删除用户

    void set_up_down_when_start();
    void start_stop_service();
    void restart_service();

signals:
    void emitGetServsSignal(SEVLIST servs);
    void emitSetUpDownWhenBoot(QString svName, int opt);
    void emitStartOrStopService(QString svName, int opt);
private slots:

   void listRowChangedSlot(int page);
   void getServicesSlot(int res, Exception exp, SEVLIST svs);      //获取用户列表线程结束，处理结果
   void setUpDownWhenBootSlot(int res, Exception exp);
   void startOrStopServiceSlot(int res, Exception exp);
private:
    Ui::TabSysPage *ui;
    SysFunClass m_sysFunModel;  //处理系统管理的后台业务逻辑
    QThread *thread;

    //user manager
    QList<UserInfo> users;
    QMenu *userMenu;
    QAction *addusrAction;
    QAction *edtusrAction;
    QAction *delusrAction;


    //service manager
    QList<ServiceInfo> sevrs;
    QMap<SevCfgStatus, QString> svrcfgMap;
    QMap<RUNSTATE, QString> runstatMap;
    QMenu *svrMenu;
    QAction *upAction;
    QAction *svrCtrlAction;
    QAction *svrRestartAction;
    bool is_first;
    QSet<QString> noShowSvrs;

    //log
    LogMainWindow *logw;

    KernParmMngWidget *kerParamWidget;

    WaitDialog *waitD;
};

#endif // TABSYSPAGE_H
