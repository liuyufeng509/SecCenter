#ifndef SECRULESPAGE_H
#define SECRULESPAGE_H

#include <QWidget>
#include"secfunclass.h"
#include "waitdialog.h"
#include <QThread>

namespace Ui {
class SecRulesPage;
}

class SecRulesPage : public QWidget
{
    Q_OBJECT

public:
    explicit SecRulesPage(QWidget *parent = 0);
    ~SecRulesPage();

    void getAllRules();
    void waitDiaogAppear();
    void waitDialogAccept();

    void UpdateRuletabel(QList<TERule> &rules);
    void InitRuleTab();

    void InitFPTab();
    void UpdateFPTable(QList<FileProConV> &convs);

signals:
    void emitGetSafePolicy(TELIST, F_PLIST);    //获取安全策略

public slots:
    void getSafePolicySlot(int res, Exception exp,TELIST teList, F_PLIST fpList);
private slots:
    void on_findButton2_clicked();
    void on_findButton_clicked();
private:
    Ui::SecRulesPage *ui;
    WaitDialog *waitD;
    QThread *thread;
    bool bFirst;

    QList<TERule> terules;
    QSet<QString> domain_types;
    QSet<QString> file_types;
    QSet<QString> class_types;

    QList<FileProConV> fpconvs;
    QSet<QString> src_types;
    QSet<QString> targ_types;
    QSet<QString> class_types_of_fp;
};

#endif // SECRULESPAGE_H
