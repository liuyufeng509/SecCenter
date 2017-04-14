#ifndef UKEYDIALOG_H
#define UKEYDIALOG_H

#include <QDialog>
#include"common.h"
namespace Ui {
class UkeyDialog;
}


class UkeyDialog : public QDialog
{
    Q_OBJECT

public:
    enum{
        Reset_PIN,
        BUND_User
    };
public:
    explicit UkeyDialog(int type, QWidget *parent = 0);
    ~UkeyDialog();
    void setUserOfUkey(int type);
private slots:
    void on_pinokButton_clicked();

    void on_bundingButton_clicked();

    void on_unbundButton_clicked();

private:
    Ui::UkeyDialog *ui;

    QList<UserInfo> users;
    UkeyInfo ukif;
    ErrorInfo err;
};

#endif // UKEYDIALOG_H
