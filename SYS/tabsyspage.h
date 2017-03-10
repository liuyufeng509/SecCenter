#ifndef TABSYSPAGE_H
#define TABSYSPAGE_H

#include <QWidget>
#include"common.h"
namespace Ui {
class TabSysPage;
}

class TabSysPage : public QWidget
{
    Q_OBJECT

public:
    explicit TabSysPage(QWidget *parent = 0);
    ~TabSysPage();
    void UpdateToUI();
    void UpdateToSvrUI();
    void UpdateToFS();
public slots:
    void on_tableWidget_customContextMenuRequested(QPoint pos);
    void on_svrTableWidget_customContextMenuRequested(QPoint pos);

    void add_user_action();
    void edit_user_action();
    void del_user_action();

    void set_up_down_when_start();
    void start_stop_service();

   void  updateToCPU();
   void updateToMem();

   void openCputButtonClicked();
   void clearLogButtonClicked();
   void setWarnButtonClicked();

   void openMemSwapButtonClicked();
   void clearMemSwapButtonClicked();
   void setWarnMemSwapButtonClicked();

private slots:
   void on_freshButton_clicked();

   void on_cleanwarnButton_clicked();

   void on_setWarnButton_clicked();

private:
    Ui::TabSysPage *ui;

    //user manager
    QList<UserInfo> users;
    QMenu *userMenu;
    QAction *addusrAction;
    QAction *edtusrAction;
    QAction *delusrAction;

    //service manager
    QList<ServiceInfo> sevrs;
    QMap<SevCfgStatus, QString> svrcfgMap;
    QMap<RUNSTATE, QString> runstatMap;
    QMenu *svrMenu;
    QAction *upAction;
    QAction *svrCtrlAction;


    //cpu
    QList<CPUGrap> cpuGraps;
    int cpu_num;
    QTimer *cpuTimer;
    int cpuWarning;

    QTextBrowser *browser;
    QPushButton *openCpuButton;
    QPushButton *clearLogButton;
    QPushButton *setWarnButton;
    QSettings  *configIniRead;


    //mem and swap
    QProgressBar *memProgressBar;
    QLabel          *memLabel;
    QProgressBar *swapProgressBar;
    QLabel          *swapLabel;
    QTextBrowser *memAndSwapBrowser;
    QPushButton *openMemSwapButton;
    QPushButton *clearMemSwapButton;
    QPushButton *setWarnMemSwapButton;
    int memWarning;
    int swapWarning;
    QTimer *memTimer;


    //file system
    QList<DISK> disks;
    int diskwarning;
};

#endif // TABSYSPAGE_H
