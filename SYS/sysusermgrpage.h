#ifndef SYSUSERMGRPAGE_H
#define SYSUSERMGRPAGE_H

#include <QWidget>
#include"common.h"
#include "sysfunclass.h"

namespace Ui {
class SysUserMgrPage;
}

class SysUserMgrPage : public QWidget
{
    Q_OBJECT

public:
    explicit SysUserMgrPage(QWidget *parent = 0);
    ~SysUserMgrPage();

    void UpdateUI();     //更新用户管理列表的界面
    bool checkUser();           //检测用户是否可以编辑
public slots:
    void on_tableWidget_customContextMenuRequested(QPoint pos);
    void add_user_action();             //添加用户
    void edit_user_action();             //编辑用户
    void del_user_action();              //删除用户
    void logout_user_action();          //登出用户

private slots:
    void on_freshButton_clicked();

private:
    Ui::SysUserMgrPage *ui;

    QList<UserInfo> users;
    QMenu *userMenu;
    QAction *addusrAction;
    QAction *edtusrAction;
    QAction *delusrAction;
    QAction *logoutUsrAction;
};

#endif // SYSUSERMGRPAGE_H
