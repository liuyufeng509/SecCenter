#ifndef TABSECRITYPAGE_H
#define TABSECRITYPAGE_H

#include <QWidget>
#include"common.h"

namespace Ui {
class TabSecrityPage;
}



class TabSecrityPage : public QWidget
{
    Q_OBJECT

public:
    explicit TabSecrityPage(QWidget *parent = 0);
    ~TabSecrityPage();
    void get_services();
    void UpdateToSecStatus();
    void initSecStatusUI();

    void UpdateRuletabel(QList<TERule> &rules);
    void InitRuleTab();

    void InitFPTab();
    void UpdateFPTable(QList<FileProConV> &convs);

    void display_cur_pwd_info();
private slots:
    void on_setusButton_clicked();
    
    void on_unlockButton_clicked();

    void on_open_closeButton_clicked();

    void on_setPwButton_clicked();

    void on_fresh_staButton_clicked();

    void on_freshButton_clicked();
    void on_setButton_clicked();

    void on_f_freshButton_clicked();

    void on_browserButton_clicked();

    void on_f_setButton_clicked();

    void init_data_of_page(int page);

    void on_findButton_clicked();

    void on_findButton2_clicked();

private:
    Ui::TabSecrityPage *ui;
    PwdInfo pwdInfo;

    QStringList services;
    SecStatus secStatus;

    QList<UserTag> user_list;

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

    //
};

#endif // TABSECRITYPAGE_H
