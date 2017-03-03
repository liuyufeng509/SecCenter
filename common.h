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
enum ROLE{
    ROOT,
    SECADMIN,
    SYSADMIN,
    AUDIADMIN,
    NORMALUSER,
};

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

struct UserInfo
{
    QString uid;
    QString uname;
    QString group;
    QStringList othgroups;

};
enum UserOpt
{
    Add,
    Edt,
};

enum SevCfgStatus
{
    ENABLE=1,
    DISABLE,
    STATIC
};

enum RUNSTATE
{
    RUNNING=1,
    EXIT,
    DEAD,
    OTHER
};

struct ServiceInfo
{
    QString sName;
    SevCfgStatus cfgStatus;
    RUNSTATE runStat;
};

ROLE get_user_role();
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

void set_userinfo(QList<UserInfo> &users);
void set_userinfos_groups(QList<UserInfo> &users);

QString add_user(UserInfo userinfo);

bool is_group_exist(QString groupname);

QString change_groups(UserInfo userinfo);
QString del_user(UserInfo userinfo);


//service option
bool get_services(QList<ServiceInfo> &sevrs);

RUNSTATE is_serv_running(QString svName);

bool up_service_when_start(QString sname);
bool down_service_when_start(QString sname);


bool stop_service(QString sname);
bool start_service(QString sname);

#endif // COMMON_H
