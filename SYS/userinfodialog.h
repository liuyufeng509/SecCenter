#ifndef USERINFODIALOG_H
#define USERINFODIALOG_H

#include <QDialog>
#include"common.h"
#include "sysfunclass.h"
namespace Ui {
class UserInfoDialog;
}

class UserInfoDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserInfoDialog(UserInfo &usinfo, int type = Add, QWidget *parent = 0);
    ~UserInfoDialog();

private slots:

    void on_buttonBox_clicked(QAbstractButton *button);
    void accept();

private:
    Ui::UserInfoDialog *ui;
    UserInfo &userinfo;
    int m_type;
    SysFunClass m_sysFunModel;
    QStringList grouplist;
};

#endif // USERINFODIALOG_H
