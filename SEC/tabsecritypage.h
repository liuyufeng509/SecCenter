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
    void UpdateToSecStatus();
    void initSecStatusUI();
private slots:
    void on_setusButton_clicked();
    
    void on_unlockButton_clicked();

    void on_open_closeButton_clicked();

    void on_setPwButton_clicked();

    void on_fresh_staButton_clicked();

private:
    Ui::TabSecrityPage *ui;
    QStringList services;
    SecStatus secStatus;

    QList<UserTag> user_list;
};

#endif // TABSECRITYPAGE_H
