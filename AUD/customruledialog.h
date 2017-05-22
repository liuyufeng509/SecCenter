#ifndef CUSTOMRULEDIALOG_H
#define CUSTOMRULEDIALOG_H

#include <QDialog>

namespace Ui {
class CustomRuleDialog;
}

class CustomRuleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomRuleDialog(QWidget *parent = 0);
    ~CustomRuleDialog();
    QString getCustomRule();
private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();
private:
    Ui::CustomRuleDialog *ui;
    QString customRule;
};

#endif // CUSTOMRULEDIALOG_H
