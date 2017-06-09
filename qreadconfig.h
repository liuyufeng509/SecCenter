#ifndef QREADCONFIG_H
#define QREADCONFIG_H
#include<QSettings>
#include <QMap>

#include"common.h"
struct SysCfgInfo
{
    int cpu_warning;
    int mem_warning;
    int disk_warning;
    int swap_warning;
    int user_hidden;
    int serv_hidden;
    int cpu_hidden;
    int mem_hidden;
    int disk_hidden;
    int other_hidden;
};

struct AudCfgInfo
{
    int warn;   //审计配置的联系人是否显示
    int audctl; //是否启用审计服务控制功能。
};

struct CommCfgInfo
{
    int dac_hidden;
    int other_hidden;
};

struct KernCfgInfo
{
    QString name;
    QString desc;
    int type;       //类型，显示的控件类型
    QString values;
};

struct KernCfgInfoList
{
    int size;
    QList<KernCfgInfo> list;
};

#define GetFileLevelErrBase   100
#define SetFileLevelErrBase   200
#define GetUserLevelErrBase 300
#define SetUserLevelErrBase 400

enum ErroNo
{
    GetFileLevelSuc = GetFileLevelErrBase,
    GetFileLevelFail = GetFileLevelErrBase-1,

    SetFileLevelSuc = SetFileLevelErrBase,


    GetUserLevelSuc = GetUserLevelErrBase,
    GetUserByNameFail=GetUserLevelErrBase-1,
    QueryUserFail = GetUserLevelErrBase-2,
    CreateKeyFail = GetUserLevelErrBase-3,

    SetUserLevelSuc = SetUserLevelErrBase,
};

class QGlobalClass
{
public:
    static QGlobalClass*  getInstance()
       {
           if(m_pInstance == NULL)
           {
               m_pInstance = new QGlobalClass();
               //QString path = getCwdPath()+"config.ini";
               QString path = "/etc/sysctl.d/Security.conf";
               m_pInstance->configIniRead = new QSettings(path, QSettings::IniFormat);
               m_pInstance->configIniRead->setIniCodec("UTF-8");
           }
           return m_pInstance;
       }

      void readConfigFile(QString filePath="");  //读取配置文件
      void setSysCfgInfoToFile();
      void setCommCfgInfoToFile();

       void printInfo();       //打印配置信息

       ~QGlobalClass();

   private:
       QGlobalClass()
       {
           errMap.insert(GetFileLevelSuc, QObject::tr("获取客体安全性标签成功"));
           errMap.insert(GetFileLevelFail, QObject::tr("获取客体安全性标签失败"));
           errMap.insert(GetUserLevelSuc, QObject::tr("获取主体安全性标签成功"));
           errMap.insert(GetUserByNameFail, QObject::tr("获取主体安全性标签失败，根据用户名获取用户失败"));
           errMap.insert(QueryUserFail, QObject::tr("获取主体安全性标签失败，查询用户失败"));
           errMap.insert(CreateKeyFail, QObject::tr("获取主体安全性标签失败，创建key失败"));
       }

       static QGlobalClass *m_pInstance;        //单例模式

       QSettings *configIniRead;//负责ini文件的读取

 public:
       SysCfgInfo sysCfgInfo;
        CommCfgInfo comInfo;
        AudCfgInfo audCfgInfo;
        KernCfgInfoList kernCfgInfoList;
        QMap<int,QString> errMap;
};

#endif // QREADCONFIG_H
