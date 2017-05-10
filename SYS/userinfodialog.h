#ifndef USERINFODIALOG_H
#define USERINFODIALOG_H

#include <QDialog>
#include"common.h"
#include "sysfunclass.h"

#include <QListWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QCheckBox>
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

    //其他组下拉菜单
    void stateChanged(int state);
    void textChanged(const QString &text);

private:
    Ui::UserInfoDialog *ui;
    UserInfo &userinfo;
    int m_type;
    QStringList grouplist;

    //其他组选择过滤功能
    QListWidget *pListWidget;
    QLineEdit *pLineEdit;
    bool bSelected;
    QString strSelectedText;
    QList<QCheckBox*> checkList;
};

#endif // USERINFODIALOG_H
