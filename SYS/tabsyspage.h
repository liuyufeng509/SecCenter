#ifndef TABSYSPAGE_H
#define TABSYSPAGE_H

#include <QWidget>
#include"common.h"
namespace Ui {
class TabSysPage;
}

class TabSysPage : public QWidget
{
    Q_OBJECT

public:
    explicit TabSysPage(QWidget *parent = 0);
    ~TabSysPage();
    void UpdateToUI();
    void UpdateToSvrUI();
public slots:
    void on_tableWidget_customContextMenuRequested(QPoint pos);
    void on_svrTableWidget_customContextMenuRequested(QPoint pos);

    void add_user_action();
    void edit_user_action();
    void del_user_action();

    void set_up_down_when_start();
    void start_stop_service();
private:
    Ui::TabSysPage *ui;

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

};

#endif // TABSYSPAGE_H
