#ifndef ALCDIALOG_H
#define ALCDIALOG_H

#include <QDialog>
#include"common.h"
namespace Ui {
class ALCDialog;
}

class ALCDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ALCDialog(FileAttr &fileAttr, QWidget *parent = 0);
    ~ALCDialog();

private slots:
    void on_cancelButton_clicked();

    void on_okButton_clicked();

    void on_getalcButton_clicked();

    void on_setUAlcButton_clicked();

    void on_delUAlcButton_clicked();

    void on_setGAlcButton_clicked();

    void on_delGAlcButton_clicked();

private:
    Ui::ALCDialog *ui;
    FileAttr & fileAttr;

};

#endif // ALCDIALOG_H
