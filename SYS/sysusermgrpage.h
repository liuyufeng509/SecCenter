#ifndef SYSUSERMGRPAGE_H
#define SYSUSERMGRPAGE_H

#include <QWidget>

namespace Ui {
class SysUserMgrPage;
}

class SysUserMgrPage : public QWidget
{
    Q_OBJECT

public:
    explicit SysUserMgrPage(QWidget *parent = 0);
    ~SysUserMgrPage();

private:
    Ui::SysUserMgrPage *ui;
};

#endif // SYSUSERMGRPAGE_H
