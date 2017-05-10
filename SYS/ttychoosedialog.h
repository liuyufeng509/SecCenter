#ifndef TTYCHOOSEDIALOG_H
#define TTYCHOOSEDIALOG_H

#include <QDialog>
#include <QSet>
#include "common.h"
namespace Ui {
class TTyChooseDialog;
}

class TTyChooseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TTyChooseDialog(QSet<QString>ttys, QString &curTty ,QWidget *parent = 0);
    ~TTyChooseDialog();

private slots:
    void on_comboBox_currentIndexChanged(int index);

    void on_buttonBox_clicked(QAbstractButton *button);

private:
    QString &m_curTty;
    Ui::TTyChooseDialog *ui;

};

#endif // TTYCHOOSEDIALOG_H
