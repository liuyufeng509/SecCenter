#ifndef COMMON_H
#define COMMON_H
#include<stdio.h>
#include<QString>
#include<QStringList>
#include<QtDebug>
#include<QFileInfo>
#include<QMessageBox>
#include<QList>
#include<QMenu>
#include<QAction>
#include<QMap>
#include<QProgressBar>
#include<QLabel>
#include<vector>
#include<qglobal.h>
//#include <glibtop/cpu.h>
//#include<glibtop/mem.h>
//#include<glibtop/swap.h>
//#include <glibtop/sysinfo.h>
#include <unistd.h>
#include <QTimer>
#include<QSettings>
#include<QTextBrowser>
#include<QDateTime>
#include<QPushButton>
#include <QLineEdit>
#include<QScrollArea>
#include<QSet>

//#include <glibtop/mountlist.h>
#include<sys/types.h>
#include<sys/stat.h>
//#include <glibtop/fsusage.h>
//#include <glibtop/mountlist.h>

#include <time.h>
#include "datadefine.h"

struct FileAttr
{
    FileAttr():fileName(""),owner(""),ownerauth(0),
        groupName(""),groupauth(0),ohterauth(0)
    {

    }

    QString     fileName;
    QString     owner;
    qint8       ownerauth;
    QString  groupName;
    qint8       groupauth;
    qint8       ohterauth;
    QString     umask;
};

struct CPUGrap
{
    QProgressBar *progressBar;
    QLabel          *label;
};

QString GetCmdRes(QString cmd);
QStringList get_users();
QStringList get_groups();
QString get_cur_user();
int get_cur_user_id();
QString get_cur_group();
int get_cur_group_id();
QString get_usr_name_by_id(int id);
QString get_usr_id_by_name(QString name);
QString get_enforce();
QString get_user_str_role();

bool get_userinfo(QList<UserInfo> &users);
void set_userinfos_groups(QList<UserInfo> &users);

QString add_user(UserInfo userinfo);

bool is_group_exist(QString groupname);

QString change_groups(UserInfo userinfo);
bool del_user(UserInfo userinfo);

bool get_trylock_services(QStringList &list);
bool trylock_service(QString cmd);

bool get_all_locked_users(QStringList &list, int &errno);
bool unlock_all_users();
bool unlock_user(QString user);
bool set_pwd_rule(QString cmd);

//about Security

bool get_cur_pwd_info(PwdInfo &pwd);

bool get_sec_status(SecStatus &status);
bool open_close_sec_policy(bool open);

bool get_user_taginfos(QList<UserTag> &reslist);
bool set_user_tagInfo(UserTag usrtag, bool add);

bool get_sak_info(SakInfo &sakinfo);
bool set_cur_sak(QString sta);
bool set_def_sak(QString sta);

bool is_command_exist(QString cmd);

//service option
bool get_services(QList<ServiceInfo> &sevrs);

RUNSTATE is_serv_running(QString svName);

bool up_service_when_start(QString sname);
bool down_service_when_start(QString sname);


bool stop_service(QString sname);
bool start_service(QString sname);
bool restart_service(QString sname);

void shutdown();
void reboot();
void logout();


QString getHintNum(quint64 bytesNum);


//config.ini
QString getCwdPath();

//security file tag
bool get_filetag_info(FileTag &fileinfo);
bool set_filetag_info(FileTag fileinfo);
bool get_te_rules(QList<TERule> &terulist);

//security file process conversion
bool get_f_p_types(QList<FileProConV> &fpconvs);


//query sentence set
bool excute_aud_cmd(QString cmd, QString &res);
bool get_kern_aud_param(KernAudParam &param);

//audit config
bool config_key_exist(QString key);
bool set_file_rule(FileAudRule fileRule);
void errMsgBox(QString str/*, QWidget *prent=NULL*/);
void infoMsgBox(QString str/*, QWidget *prent=NULL*/);
int warnMsgBox(QString msg/*, QWidget *prent=NULL*/);
extern bool isTab;
#endif // COMMON_H
