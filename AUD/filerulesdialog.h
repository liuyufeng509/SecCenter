#ifndef FILERULESDIALOG_H
#define FILERULESDIALOG_H

#include <QDialog>
#include "common.h"

namespace Ui {
class FileRulesDialog;
}

class FileRulesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileRulesDialog(FileAudRule &fileRule,QWidget *parent = 0);
    ~FileRulesDialog();
    bool save_file_rules_from_ui();
private slots:
    void on_okButton_clicked();
    void on_cancelButton_clicked();
    void on_browFileButton_clicked();

    void on_traceButton_clicked();

private:
    Ui::FileRulesDialog *ui;
    FileAudRule &fileRule;
};

#endif // FILERULESDIALOG_H
