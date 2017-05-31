#include "qreadconfig.h"
QReadConfig* QReadConfig::m_pInstance = NULL;

void QReadConfig::readConfigFile(QString filePath)
{
    filePath = "";
    sysCfgInfo.cpu_warning =   configIniRead->value("SYSTEM/cpu_warning").toInt();
    sysCfgInfo.mem_warning = configIniRead->value("SYSTEM/mem_warning").toInt();

    comInfo.dac_hidden = configIniRead->value("COMMON/dac_hidden").toInt();
    comInfo.other_hidden = configIniRead->value("COMMON/other_hidden").toInt();

    audCfgInfo.warn = configIniRead->value("AUD/warn").toInt();
    audCfgInfo.audctl = configIniRead->value("AUD/audctl").toInt();
    //内核参数设置
    kernCfgInfoList.size = configIniRead->value("Kern/size").toInt();
    for(int i=0; i<kernCfgInfoList.size; i++)
    {
        KernCfgInfo kernInfo;
        QString key = "Kern/name"+QString::number(i+1);
        kernInfo.name = configIniRead->value(key).toString();
        key = "Kern/type"+QString::number(i+1);
        kernInfo.type = configIniRead->value(key).toInt();
        if(kernInfo.type==0)
        {
            key = "Kern/values"+QString::number(i+1);
            kernInfo.values = configIniRead->value(key).toString();
        }
        key = "Kern/desc"+QString::number(i+1);
        kernInfo.desc = configIniRead->value(key).toString();

        kernCfgInfoList.list.append(kernInfo);
    }
}

void QReadConfig::setSysCfgInfoToFile()
{
    configIniRead->setValue("SYSTEM/cpu_warning", sysCfgInfo.cpu_warning);
    configIniRead->setValue("SYSTEM/mem_warning", sysCfgInfo.mem_warning);
    configIniRead->setValue("SYSTEM/disk_warning", sysCfgInfo.disk_warning);
    configIniRead->setValue("SYSTEM/swap_warning", sysCfgInfo.swap_warning);
    configIniRead->setValue("SYSTEM/user_hidden", sysCfgInfo.user_hidden);
    configIniRead->setValue("SYSTEM/serv_hidden", sysCfgInfo.serv_hidden);
    configIniRead->setValue("SYSTEM/cpu_hidden", sysCfgInfo.cpu_hidden);
    configIniRead->setValue("SYSTEM/mem_hidden", sysCfgInfo.mem_hidden);
    configIniRead->setValue("SYSTEM/disk_hidden", sysCfgInfo.disk_hidden);
    configIniRead->setValue("SYSTEM/other_hidden", sysCfgInfo.other_hidden);
}

void QReadConfig::setCommCfgInfoToFile()
{

}

QReadConfig::~QReadConfig()
{
    delete configIniRead;
}
