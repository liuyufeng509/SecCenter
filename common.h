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
#include <glibtop/cpu.h>
#include<glibtop/mem.h>
#include<glibtop/swap.h>
#include <glibtop/sysinfo.h>
#include <unistd.h>
#include <QTimer>
#include<QSettings>
#include<QTextBrowser>
#include<QDateTime>
#include<QPushButton>
#include <QLineEdit>
#include<QScrollArea>
#include<QSet>

#include <glibtop/mountlist.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <glibtop/fsusage.h>
#include <glibtop/mountlist.h>

#include <time.h>

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
    bool isShow;

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

struct CPUGrap
{
    QProgressBar *progressBar;
    QLabel          *label;
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

bool set_userinfo(QList<UserInfo> &users);
void set_userinfos_groups(QList<UserInfo> &users);

QString add_user(UserInfo userinfo);

bool is_group_exist(QString groupname);

QString change_groups(UserInfo userinfo);
QString del_user(UserInfo userinfo);

bool get_trylock_services(QStringList &list);
bool trylock_service(QString cmd);

//bool get_all_locked_users(QStringList &list);
bool unlock_all_users();
bool set_pwd_rule(QString cmd);

//about Security
struct SecStatus
{
    QString selinux_status;
    QString selinux_fs_mount;
    QString selinux_root_dir;
    QString load_policy_name;
    QString curr_mode;
    QString mode_frm_cfg;
    QString mls_status;
    QString policy_deny_stat;
    QString max_kern_policy_version;
    void clear()
    {
        selinux_status="";
        selinux_fs_mount="";
        selinux_root_dir="";
        load_policy_name="";
        curr_mode="";
        mode_frm_cfg="";
        mls_status="";
        policy_deny_stat="";
        max_kern_policy_version="";
    }
};

bool get_sec_status(SecStatus &status);
bool open_close_sec_policy(bool open);

struct UserTag
{
    QString username;
    QString safeTag;
    QString wholeTag;
    bool operator ==(const UserTag& d)
    {
        return username == d.username;
    }
};
bool get_user_taginfos(QList<UserTag> &reslist);
bool set_user_tagInfo(UserTag usrtag, bool add);


//cpu and mem
enum {
    CPU_TOTAL,
    CPU_USED,
    N_CPU_STATES
};

struct cpu_record
{
    int now;
    long times[2][GLIBTOP_MAX_CPU][GLIBTOP_NCPU];
};

struct CPU
{
    float total;
    float used;
    float useage;
};

struct MeM
{
    quint64 total;          //Byte
    quint64 used;          //Byte
    float      percent;
};

struct SWAP
{
    quint64 total;          //Byte
    quint64 used;          //Byte
    float      percent;
};

struct DISK
{
    quint64 devid;
    QString devName;
    QString  mountDir;
    QString ftype;
    guint64 used;
    guint64 bfree;
    guint64 bavail;
    guint64 btotal;
    int         percent;
};

bool is_command_exist(QString cmd);

//service option
bool get_services(QList<ServiceInfo> &sevrs);

RUNSTATE is_serv_running(QString svName);

bool up_service_when_start(QString sname);
bool down_service_when_start(QString sname);


bool stop_service(QString sname);
bool start_service(QString sname);

void shutdown();
void reboot();
void logout();

//cpu and mem
int  get_n_cpu();
void get_load (std::vector<CPU>  &cpus);

void getmem(MeM &memInfo);
void get_swap(SWAP &swapInfo);
QString getHintNum(quint64 bytesNum);

//config.ini
QString getCwdPath();

//file system
void fsusage_stats(const glibtop_fsusage *buf,
              guint64 *bused, guint64 *bfree, guint64 *bavail, guint64 *btotal,
              gint *percentage);
void get_fsInfo(QList<DISK> &disks);


//security file tag
struct FileTag
{
    QString filename;
    QString safeTag;
    QString wholeTag;
    bool isDir;
};

bool get_filetag_info(FileTag &fileinfo);
bool set_filetag_info(FileTag fileinfo);


//security rule
struct TERule
{
    QString domain_type;
    QString file_type;
    QString class_type;
    QString permmisions;
};

bool get_te_rules(QList<TERule> &terulist);

//security file process conversion
struct FileProConV
{
    QString src_type;
    QString targ_type;
    QString class_type;
    QString default_type;
};

bool get_f_p_types(QList<FileProConV> &fpconvs);


//audit manager

//query sentence set
bool excute_aud_cmd(QString cmd, QString &res);

//audit rules
struct KernAudParam
{
    QString backlog_limit;
    QString enable;
    QString fail_flag;
    QString rate_limit;
    bool bignore;
};

bool get_kern_aud_param(KernAudParam &param);

//audit config
struct AudConfigInfo
{
    QString log_file;
    QString max_log_file;
    QString max_log_file_action;
    QString num_logs;
    QString admin_space_left;
    QString admin_space_left_action;
    QString space_left;
    QString space_left_action;
    QString disk_full_action;
    QString disk_error_action;
    QString warning_mail;
    QString warning_tel;
};

bool config_key_exist(QString key);

//file aud rules
struct FileAudRule
{
    QString file_name;
    QString auth;
    QString ts_time;
    QString te_time;
    QString key_word;
};

bool set_file_rule(FileAudRule fileRule);

#endif // COMMON_H
