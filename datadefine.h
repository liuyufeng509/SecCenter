#ifndef DATADEFINE_H
#define DATADEFINE_H
#include<QString>
#include <QStringList>
#include <QSet>
//Exception 类，用于调用命令时抛出异常。
class Exception
{
public:
    Exception()
    {

    }

	Exception(QString errNo, QString errWhat)
	{
		m_errNo = errNo;
		m_errWhat=errWhat;		
	}
	QString getErroNo(){return m_errNo;}
	QString getErroWhat(){return m_errWhat;}
private:
	QString m_errNo;
	QString m_errWhat;
};

/*****************************************************
 *主界面逻辑处理所需类型定义
 *
*****************************************************/
enum ROLE{
    ROOT,
	SECADMIN,
	SYSADMIN,
    AUDADMIN,
	ERROR,
};

/*******************************************************
 * 系统管理界面逻辑处理所需类型定义
 * ****************************************************/
//用户信息
struct UserInfo
{
	QString uid;
	QString uname;
	QString group;
    QStringList ogroups;
	bool isShow;
    QSet<QString> ttys;         //当前登陆的终端
};
//用户操作类型
enum UserOpt
{
    Add,
    Edt,
};

//服务信息
enum SevCfgStatus       //开机启动的状态
{
    ENABLE=1,
    DISABLE,
    STATIC
};
enum RUNSTATE       //运行状态
{
    RUNNING=1,
    EXIT,
    DEAD,
    OTHER
};

struct ServiceInfo      //服务信息结构体
{
    QString sName;
    QString pId;
    QString desc;
    SevCfgStatus cfgStatus;
    RUNSTATE runStat;
};
typedef QList<ServiceInfo>   SEVLIST;//服务列表类型

/*******************************************************
 * 安全管理界面逻辑处理所需类型定义
 * ****************************************************/
struct SecUserInfo          //安全管理的用户信息
 {
    QString uId;
    QString uName;
    bool     bBindKey;
    bool     bLocked;
    bool     bShow;
};

struct UkeyInfo                 //Ukey信息
{
    QString cur_pin;
    QString new_pin;
    QString user;
    int type;
};

struct TryLockInfo      //锁定规则信息
{
    QString dParam;
    QString uParam;
    QString sParam;
};

struct PwdInfo      //密码规则信息
{
    QString minLen;
    QString dcredit;
    QString ucredit;
    QString lcredit;
    QString ocredit;
};

struct SecStatus                    //安全状态信息
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
        curr_mode=QObject::tr("未知");
        mode_frm_cfg="";
        mls_status="";
        policy_deny_stat="";
        max_kern_policy_version="";
    }
};

//文件安全标签
struct FileTag
{
    QString filename;
    QString safeTag;
    QString wholeTag;
    bool isDir;
};

//用户安全标签
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

//TE 策略
struct TERule
{
    QString domain_type;
    QString file_type;
    QString class_type;
    QString permmisions;
};
//文件/进程策略
struct FileProConV
{
    QString src_type;
    QString targ_type;
    QString class_type;
    QString default_type;
};
typedef QList<TERule>   TELIST;//TE策略列表类型
typedef QList<FileProConV>   F_PLIST;//文件/进程策略列表类型

//SAK 信息
struct SakInfo
{
    QString current_mode;
    QString default_mode;
};

/*******************************************************
 * 安全管理界面逻辑处理所需类型定义
 * ****************************************************/
enum OP_TYPE
{
    OPEN_CLOSE_AUD,
    QUERY_SET,
    AUD_REPORT,
    KER_AUD_PARM,
    FILE_AUD_PARM,
    SYS_CALL_RULE,
    CUSTOM_RULE,
    AUD_CONFIG,
    AUD_DISPLAY
};

//file aud rules
struct FileAudRule
{
    QString file_name;
    QString auth;
    QString ts_time;
    QString te_time;
    QString key_word;
};

//系统调用结构体
struct SysCallRule
{
    QString list;
    QString sparam;
    QString ts;
    QString te;
};

#endif
