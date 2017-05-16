#ifndef QUERYDIALOG_H
#define QUERYDIALOG_H

#include <QDialog>
#include "common.h"
namespace Ui {
class QueryDialog;
}

class QueryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QueryDialog(QWidget *parent = 0);
    ~QueryDialog();
    QString getCmdStence(){return querySt;}
    void connCheckBoxAndLineEdit();
    bool check_param();
private slots:
    void on_query_produceButton_clicked();
    void setAuSearchUI();                   //更新审计查询的各个参数输入框的可用状态
    void on_okButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::QueryDialog *ui;
    QString querySt;
};

#endif // QUERYDIALOG_H
