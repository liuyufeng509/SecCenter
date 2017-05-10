#ifndef SYSCALLDIALOG_H
#define SYSCALLDIALOG_H

#include <QDialog>

namespace Ui {
class SysCallDialog;
}

class SysCallDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SysCallDialog(QWidget *parent = 0);
    ~SysCallDialog();
protected:
  //  void mouseReleaseEvent(QMouseEvent *);
private:
    Ui::SysCallDialog *ui;
};

#endif // SYSCALLDIALOG_H
