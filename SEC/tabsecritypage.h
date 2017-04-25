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

    void on_unlockButton_clicked();

    void on_open_closeButton_clicked();

    void on_setPwButton_clicked();

    void on_freshFileTagButton_clicked();

    void on_browserButton_clicked();
    void on_setFileTagButton_clicked();

    void listItemChangedSlot(int page);

    void on_findButton_clicked();

    void on_findButton2_clicked();

    void on_getlockusrsButton_clicked();

    void on_closeAduButton_clicked();

    void on_close_client_reuse_Button_clicked();

    void on_open_close_sak_Button_clicked();

    void on_open_close_def_sak_Button_clicked();

    void on_ukey_pinButton_clicked();

    void on_ukey_userButton_clicked();

    void on_setTryLockButton_clicked();

    void on_freshSafeStatusButton_clicked();

    void on_freshUserSafeTagButton_clicked();

    void on_setUserTagButton_clicked();

    //多线程结果处理
    void setUserTagInfoSlot(int res, Exception exp);

signals:
    void emitSetUserTagInfo(UserTag, int);
private:
    Ui::TabSecrityPage *ui;

    SecFunClass m_secFunModel;

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
};

#endif // TABSECRITYPAGE_H
