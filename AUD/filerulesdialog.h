#ifndef FILERULESDIALOG_H
#define FILERULESDIALOG_H

#include <QDialog>

namespace Ui {
class FileRulesDialog;
}

class FileRulesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FileRulesDialog(QWidget *parent = 0);
    ~FileRulesDialog();

private:
    Ui::FileRulesDialog *ui;
};

#endif // FILERULESDIALOG_H
