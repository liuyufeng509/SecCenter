#ifndef QREADCONFIG_H
#define QREADCONFIG_H
#include<QSettings>
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

struct CommCfgInfo
{
    int dac_hidden;
    int other_hidden;
};



class QReadConfig
{
public:
    static QReadConfig*  getInstance()
       {
           if(m_pInstance == NULL)
           {
               m_pInstance = new QReadConfig();
               QString path = getCwdPath()+"config.ini";
               m_pInstance->configIniRead = new QSettings(path, QSettings::IniFormat);
           }
           return m_pInstance;
       }

      void readConfigFile(QString filePath="");  //读取配置文件
      void setSysCfgInfoToFile();
      void setCommCfgInfoToFile();

       void printInfo();       //打印配置信息

       ~QReadConfig();

   private:
       QReadConfig()
       {

       }

       static QReadConfig *m_pInstance;        //单例模式

       QSettings *configIniRead;//负责ini文件的读取

 public:
       SysCfgInfo sysCfgInfo;
        CommCfgInfo comInfo;
};

#endif // QREADCONFIG_H
