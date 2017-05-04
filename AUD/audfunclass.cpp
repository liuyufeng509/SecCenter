#include "audfunclass.h"

AudFunClass::AudFunClass(QObject *parent) : QObject(parent)
{

}

bool AudFunClass::getKernAudParam(KernAudParam &param)     //内核审计参数
{
    QString cmd = "auditctl -s 2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：获取内核审计参数三失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    strl.removeLast();      //remove  echo $?
    if(strl.count()!=8)
        {
        QString errContent=tr("执行操作：获取内核审计参数三失败")+ tr("\n执行命令：")+cmd+tr("\n错误内容：结果列表长度不为8");
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }

    for(int i=0; i<strl.count(); i++)
    {
        strl[i] = strl[i].simplified();
        QStringList tmpl = strl[i].split(' ');
        switch(i)
        {
            case 4:
                param.backlog_limit = tmpl[1];
                break;
            case 0:
                param.enable = tmpl[1];
                break;
            case 1:
                param.fail_flag = tmpl[1];
                break;
            case 3:
                param.rate_limit = tmpl[1];
                break;
        }
    }
    return true;
}

bool AudFunClass::excuteAudCmd(QString cmd, QString optType, QString &res)
{
    cmd = cmd + " 2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：")+ optType + tr("失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    resStr.chop(strl.last().length());
    res = resStr;
    return true;
}


bool AudFunClass::startOrStopService(QString svName, int opt)      //开启或关闭服务
{
    QString cmd = "service " ;
    if(opt==1)
        cmd += svName + " stop 2>&1; echo $?";
    else if(opt==0)
        cmd += svName + " start 2>&1; echo $?";
    else
        cmd += svName + " restart 2>&1; echo $?";

    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：开启或关闭服务失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    return true;
}
