#ifndef TABSECRITYPAGE_H
#define TABSECRITYPAGE_H

#include <QWidget>
#include"common.h"
#include "secfunclass.h"
#include "waitdialog.h"
#include <QThread>
namespace Ui {
class TabSecrityPage;
}



class TabSecrityPage : public QWidget
{
    Q_OBJECT

public:
    explicit TabSecrityPage(QWidget *parent = 0);
    ~TabSecrityPage();

    void updateSecUserUI();

    void getLockServices();
    void UpdateToSecStatus();

    void UpdateRuletabel(QList<TERule> &rules);
    void InitRuleTab();

    void InitFPTab();
    void UpdateFPTable(QList<FileProConV> &convs);

    void display_cur_pwd_info();

    void init_sak_ui();

    void waitDiaogAppear();
private slots:

    void on_setPwButton_clicked();

    void on_freshFileTagButton_clicked();

    void on_browserButton_clicked();
    void on_setFileTagButton_clicked();

    void listItemChangedSlot(int page);

    void on_findButton_clicked();

    void on_findButton2_clicked();

    void on_closeAduButton_clicked();

    void on_close_client_reuse_Button_clicked();

    void on_open_close_sak_Button_clicked();

    void on_open_close_def_sak_Button_clicked();

    void on_setTryLockButton_clicked();

    void on_freshSafeStatusButton_clicked();

    void on_freshUserSafeTagButton_clicked();

    void on_setUserTagButton_clicked();
    void on_userTableWidget_customContextMenuRequested(const QPoint &pos);
    void unLockActionSlot();
    void ukeyPINActionSlot();
    void ukeyBindActionSlot();
    void freshActionSlot();
    void on_lockSvrComboBox_currentIndexChanged(const QString &arg1);

    //多线程结果处理
    void setUserTagInfoSlot(int res, Exception exp);
    void getSafePolicySlot(int res, Exception exp,TELIST teList, F_PLIST fpList);
    void on_open_closeSecPolButton_clicked();

signals:
    void emitSetUserTagInfo(UserTag, int);      //设置用户安全标签
    void emitGetSafePolicy(TELIST, F_PLIST);    //获取安全策略
private:
    Ui::TabSecrityPage *ui;

    SecFunClass m_secFunModel;

    QList<SecUserInfo> secUserList;
    QMenu *secUserMenu;
    QAction *ukeyPINAction;
    QAction *ukeyBindAction;
    QAction *unLockAction;
    QAction *freshAction;

    QStringList  users;
    PwdInfo pwdInfo;

    QStringList services;
    SecStatus secStatus;

    QList<UserTag> userTagList;

    QString filePath;
    bool isDir;

    //te policy rule
    QList<TERule> terules;
    QSet<QString> domain_types;
    QSet<QString> file_types;
    QSet<QString> class_types;

    //security file process conversion
    QList<FileProConV> fpconvs;
    QSet<QString> src_types;
    QSet<QString> targ_types;
    QSet<QString> class_types_of_fp;

    //open/close manager
    SakInfo sakinfo;

    WaitDialog *waitD;
    QThread *thread;

    bool bFirst;
};

#endif // TABSECRITYPAGE_H
