#ifndef SYSADMFORM_H
#define SYSADMFORM_H
#include "indexwidget.h"
#include <QWidget>
#include "common.h"
#include "helppage.h"
#include "sysusermgrpage.h"
#include "svrmgrpage.h"
#include "syslogpage.h"
#include "kernalparammanager.h"
namespace Ui {
class SysAdmForm;
}
enum
        {
    INDEXPAGE,
    USERMGRPAGE,
    SVRMGRPAGE,
    SYSLOGPAGE,
    KERNPARPAGE,
    HELPPAGE
};
class SysAdmForm : public QWidget
{
    Q_OBJECT

public:
    explicit SysAdmForm(QWidget *parent = 0);
    ~SysAdmForm();

private slots:
    void on_svrMgrButton_toggled(bool checked);

    void on_userMgrButton_toggled(bool checked);

    void on_indexButton_toggled(bool checked);

    void on_syslogButton_toggled(bool checked);

    void on_kernParamButton_toggled(bool checked);

    void on_helpButton_toggled(bool checked);

private:
    Ui::SysAdmForm *ui;

    IndexWidget *indexPage;
    HelpPage *helpPage;
    SysUserMgrPage *userMgrPage;
    SvrMgrPage *svrMgrPage;
    SysLogPage *sysLogPage;
    KernParmMngWidget *kernalPamPage;
};

#endif // SYSADMFORM_H
