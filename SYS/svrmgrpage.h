#ifndef SVRMGRPAGE_H
#define SVRMGRPAGE_H
#include "common.h"
#include <QWidget>
#include "waitdialog.h"
#include <QThread>
#include <QModelIndex>
#include "sysfunclass.h"
namespace Ui {
class SvrMgrPage;
}

class SvrMgrPage : public QWidget
{
    Q_OBJECT

public:
    explicit SvrMgrPage(QWidget *parent = 0);
    ~SvrMgrPage();
    void UpdateToSvrUI();
    void waitDialogAccept();
    void waitDiaogAppear();

    void getAllServices();
signals:
    void emitGetServsSignal(SEVLIST servs);
    void emitSetUpDownWhenBoot(QString svName, int opt);
    void emitStartOrStopService(QString svName, int opt);

public slots:
    void set_up_down_when_start();
    void start_stop_service();
    void restart_service();
    void fresh_services();
    void on_svrTableWidget_customContextMenuRequested(QPoint pos);
private slots:
    void getServicesSlot(int res, Exception exp, SEVLIST svs);      //获取用户列表线程结束，处理结果
    void setUpDownWhenBootSlot(int res, Exception exp);
    void startOrStopServiceSlot(int res, Exception exp);
    void on_svrTableWidget_clicked(const QModelIndex &index);

private:
    Ui::SvrMgrPage *ui;
    QThread *thread;
    QList<ServiceInfo> sevrs;
    QMap<SevCfgStatus, QString> svrcfgMap;
    QMap<RUNSTATE, QString> runstatMap;
    QMenu *svrMenu;
    QAction *upAction;
    QAction *svrCtrlAction;
    QAction *svrRestartAction;
    QAction *freshListAction;
    bool is_first;
    QSet<QString> noShowSvrs;       //不显示的服务

    WaitDialog *waitD;
};

#endif // SVRMGRPAGE_H
