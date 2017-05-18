#ifndef SYSCALLDIALOG_H
#define SYSCALLDIALOG_H

#include <QDialog>
#include "common.h"

namespace Ui {
class SysCallDialog;
}

class SysCallDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SysCallDialog(SysCallRule &sysCallRule,QWidget *parent = 0);
    ~SysCallDialog();
    public slots:
    void on_list_group_radioButton_clicked();
protected:
  //  void mouseReleaseEvent(QMouseEvent *);
private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();

private:
    Ui::SysCallDialog *ui;
    SysCallRule &sysCallRule;
};

#endif // SYSCALLDIALOG_H
