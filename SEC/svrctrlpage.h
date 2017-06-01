#ifndef SVRCTRLPAGE_H
#define SVRCTRLPAGE_H

#include <QWidget>
#include "common.h"
#include"secfunclass.h"
namespace Ui {
class SvrCtrlPage;
}

class SvrCtrlPage : public QWidget
{
    Q_OBJECT

public:
    explicit SvrCtrlPage(QWidget *parent = 0);
    ~SvrCtrlPage();

    void getHtmlStr(QString labelStr, QString color, QString status);
      void updateUI();
private slots:

    void on_openAllButton_clicked();

    void on_open_closeSecPolButton_clicked();

   // void freshPolCtl(SecStatus);
    void on_open_close_sak_Button_clicked();

    void on_open_close_def_sak_Button_clicked();

    void on_close_client_reuse_Button_clicked();

private:
    Ui::SvrCtrlPage *ui;
    SecStatus secStatus;
    SakInfo sakinfo;
    QString htmlStr;
    bool isRmOpen;
};

#endif // SVRCTRLPAGE_H
