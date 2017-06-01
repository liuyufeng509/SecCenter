#ifndef SECSTATUSPAGE_H
#define SECSTATUSPAGE_H

#include <QWidget>
#include "secfunclass.h"
namespace Ui {
class SecStatusPage;
}

class SecStatusPage : public QWidget
{
    Q_OBJECT

public:
    explicit SecStatusPage(QWidget *parent = 0);
    ~SecStatusPage();


private slots:
    void on_freshSafeStatusButton_clicked();

signals:
    //void freshClicked(SecStatus);

private:
    void UpdateToSecStatus();
private:
    Ui::SecStatusPage *ui;
    SecStatus secStatus;
};

#endif // SECSTATUSPAGE_H
