#ifndef UKEYDIALOG_H
#define UKEYDIALOG_H

#include <QDialog>
#include"common.h"
#include "secfunclass.h"
#include "waitdialog.h"
#include<QThread>
namespace Ui {
class UkeyDialog;
}


class UkeyDialog : public QDialog
{
    Q_OBJECT

public:

public:
    explicit UkeyDialog(int type, QString username="",QWidget *parent = 0);
    explicit UkeyDialog(int type, QWidget *parent = 0);
    ~UkeyDialog();
    void setUserOfUkey(int type);
    void waitDialogAccept();
    void waitDiaogAppear();
    void InitUI(int type);
signals:
    void emitSetUserOfUkeySignal(UkeyInfo ukeyInfo);
    void emitResetPINOfUkeySignal(UkeyInfo ukeyInfo);

private slots:
    void setUserOfUkeySlot(int res, Exception exp);
    void resetPINOfUkeySlot(int res, Exception exp);

private slots:
    void on_pinokButton_clicked();

//    void on_bundingButton_clicked();

   // void on_unbundButton_clicked();

    void on_okButton_clicked();

    void on_pin_cancelButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::UkeyDialog *ui;
    UkeyInfo ukif;
    int mType;
    WaitDialog *waitD;
    QThread *thread;
};

#endif // UKEYDIALOG_H
