#ifndef SECSTATUSPAGE_H
#define SECSTATUSPAGE_H

#include <QWidget>

namespace Ui {
class SecStatusPage;
}

class SecStatusPage : public QWidget
{
    Q_OBJECT

public:
    explicit SecStatusPage(QWidget *parent = 0);
    ~SecStatusPage();

private:
    Ui::SecStatusPage *ui;
};

#endif // SECSTATUSPAGE_H
