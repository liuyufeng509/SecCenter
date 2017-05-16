#ifndef AUDSVRCTLPAGE_H
#define AUDSVRCTLPAGE_H

#include <QWidget>
#include "secfunclass.h"
namespace Ui {
class AudSvrCtlPage;
}

class AudSvrCtlPage : public QWidget
{
    Q_OBJECT

public:
    explicit AudSvrCtlPage(QWidget *parent = 0);
    ~AudSvrCtlPage();

private slots:
    void on_closeAduButton_clicked();

private:
    Ui::AudSvrCtlPage *ui;
};

#endif // AUDSVRCTLPAGE_H
