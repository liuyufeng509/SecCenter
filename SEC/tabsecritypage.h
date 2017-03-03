#ifndef TABSECRITYPAGE_H
#define TABSECRITYPAGE_H

#include <QWidget>
#include"common.h"
namespace Ui {
class TabSecrityPage;
}

class TabSecrityPage : public QWidget
{
    Q_OBJECT

public:
    explicit TabSecrityPage(QWidget *parent = 0);
    ~TabSecrityPage();
    void get_services();
private slots:
    void on_setusButton_clicked();
    
    void on_unlockButton_clicked();

private:
    Ui::TabSecrityPage *ui;
    QStringList services;
};

#endif // TABSECRITYPAGE_H
