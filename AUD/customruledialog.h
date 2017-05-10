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

private:
    Ui::CustomRuleDialog *ui;
};

#endif // CUSTOMRULEDIALOG_H
