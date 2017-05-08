#ifndef SVRCTRLPAGE_H
#define SVRCTRLPAGE_H

#include <QWidget>
#include "common.h"
namespace Ui {
class SvrCtrlPage;
}

class SvrCtrlPage : public QWidget
{
    Q_OBJECT

public:
    explicit SvrCtrlPage(QWidget *parent = 0);
    ~SvrCtrlPage();

private slots:
    void on_openAllButton_clicked();

    void on_open_closeSecPolButton_clicked();

private:
    Ui::SvrCtrlPage *ui;
};

#endif // SVRCTRLPAGE_H
