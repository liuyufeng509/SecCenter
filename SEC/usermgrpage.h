#ifndef USERMGRPAGE_H
#define USERMGRPAGE_H

#include <QWidget>
#include "secfunclass.h"
#include "waitdialog.h"
#include <QTableWidgetItem>
namespace Ui {
class UserMgrPage;
}

class UserMgrPage : public QWidget
{
    Q_OBJECT

public:
    explicit UserMgrPage(QWidget *parent = 0);
    ~UserMgrPage();
    void updateSecUserUI();
    void getLockServices();
    void display_cur_pwd_info();
public slots:
    void on_userTableWidget_customContextMenuRequested(const QPoint &pos);
    void unLockActionSlot();
    void ukeyPINActionSlot();
    void ukeyBindActionSlot();
    void freshActionSlot();
private slots:
    void on_userTableWidget_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);
    void on_lockSvrComboBox_currentIndexChanged(const QString &arg1);
    void on_setTryLockButton_clicked();

    void on_setPwButton_clicked();

    void on_cleanUkButton_clicked();

private:
    Ui::UserMgrPage *ui;

    QList<SecUserInfo> secUserList;     //用户列表
    QMenu *secUserMenu;
    //QAction *ukeyPINAction;
    QAction *ukeyBindAction;
    QAction *unLockAction;
    QAction *freshAction;

    QStringList  users;
    PwdInfo pwdInfo;
    QStringList services;
};

#endif // USERMGRPAGE_H
