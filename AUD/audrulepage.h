#ifndef AUDRULEPAGE_H
#define AUDRULEPAGE_H

#include <QWidget>
#include "common.h"
namespace Ui {
class AudRulePage;
}

class AudRulePage : public QWidget
{
    Q_OBJECT

public:
    explicit AudRulePage(QWidget *parent = 0);
    ~AudRulePage();

private slots:
    void on_addButton_clicked();

    void on_cancelButton_clicked();

private:
    Ui::AudRulePage *ui;
};

#endif // AUDRULEPAGE_H
