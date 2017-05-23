#include "qreadconfig.h"
QReadConfig* QReadConfig::m_pInstance = NULL;


void QReadConfig::readConfigFile(QString filePath)
{
    filePath = "";
    sysCfgInfo.cpu_warning =   configIniRead->value("SYSTEM/cpu_warning").toInt();
    sysCfgInfo.mem_warning = configIniRead->value("SYSTEM/mem_warning").toInt();
    sysCfgInfo.disk_warning =  configIniRead->value("SYSTEM/disk_warning").toInt();
    sysCfgInfo.swap_warning =configIniRead->value("SYSTEM/swap_warning").toInt();
    sysCfgInfo.user_hidden =   configIniRead->value("SYSTEM/user_hidden").toInt();
    sysCfgInfo.serv_hidden = configIniRead->value("SYSTEM/serv_hidden").toInt();
    sysCfgInfo.cpu_hidden = configIniRead->value("SYSTEM/cpu_hidden").toInt();
    sysCfgInfo.mem_hidden = configIniRead->value("SYSTEM/mem_hidden").toInt();
    sysCfgInfo.disk_hidden = configIniRead->value("SYSTEM/disk_hidden").toInt();
    sysCfgInfo.other_hidden = configIniRead->value("SYSTEM/other_hidden").toInt();


    comInfo.dac_hidden = configIniRead->value("COMMON/dac_hidden").toInt();
    comInfo.other_hidden = configIniRead->value("COMMON/other_hidden").toInt();

    audCfgInfo.warn = configIniRead->value("AUD/warn").toInt();
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
