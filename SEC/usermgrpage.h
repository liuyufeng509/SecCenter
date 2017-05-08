#ifndef USERMGRPAGE_H
#define USERMGRPAGE_H

#include <QWidget>

namespace Ui {
class UserMgrPage;
}

class UserMgrPage : public QWidget
{
    Q_OBJECT

public:
    explicit UserMgrPage(QWidget *parent = 0);
    ~UserMgrPage();

private:
    Ui::UserMgrPage *ui;
};

#endif // USERMGRPAGE_H
