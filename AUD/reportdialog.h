#ifndef REPORTDIALOG_H
#define REPORTDIALOG_H

#include <QDialog>

namespace Ui {
class ReportDialog;
}

class ReportDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReportDialog(QWidget *parent = 0);
    ~ReportDialog();
    QString getCmdStence(){return cmd;}
private slots:
    void on_okButton_clicked();

    void on_res_checkBox_clicked();

    void on_tm_sec_checkBox_clicked();

    void on_cancelButton_clicked();

private:
    Ui::ReportDialog *ui;
    QString cmd;
};

#endif // REPORTDIALOG_H
