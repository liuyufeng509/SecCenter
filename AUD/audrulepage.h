#ifndef AUDRULEPAGE_H
#define AUDRULEPAGE_H

#include <QWidget>
#include "common.h"
#include "audfunclass.h"
namespace Ui {
class AudRulePage;
}

class AudRulePage : public QWidget
{
    Q_OBJECT

public:
    explicit AudRulePage(QWidget *parent = 0);
    ~AudRulePage();
    void updateRuleList();
    void updateUI();
    bool isRuleExist(QString rule, QStringList ruleList);
private slots:
    void on_addButton_clicked();

    void on_cancelButton_clicked();

    void on_upButton_clicked();

    void on_downButton_clicked();

    void on_modButton_clicked();
    void enableItemEditable();
    void on_delButton_clicked();

    void on_nowButton_clicked();

    void on_freshButton_clicked();

    void on_forEverButton_clicked();

private:
    Ui::AudRulePage *ui;
    QStringList ruleList;
    FileAudRule fileRule;
    SysCallRule sysCallRule;
    bool isModify;
};

#endif // AUDRULEPAGE_H
