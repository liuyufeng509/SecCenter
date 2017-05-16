#include "audfunclass.h"

AudFunClass* AudFunClass::m_pInstance = NULL;

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
    resStr.chop(strl.last().length());
    res = resStr;
    if(strl.last().toInt()!=0 && strl.last().toInt()!=1)
    {
        QString errContent=tr("执行操作：")+ optType + tr("失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }

    return true;
}

bool AudFunClass::getCurrentRules(QStringList &ruleList)            //获取所有的审计规则
{
    QString cmdstr = "auditctl -l";

    try
    {
        QString rs;
        AudFunClass::getInstance()->excuteAudCmd(cmdstr, tr("获取当前规则"),rs);
        ruleList = rs.split('\n');
    }catch (Exception exp)
            {
        throw exp;
    }
    return true;
}

bool AudFunClass::startOrStopService(QString svName, int opt)      //开启或关闭服务
{
    QString cmd = "systemctl " ;
    if(opt==1)
        cmd += "stop "+ svName +" 2>&1; echo $?";
    else if(opt==0)
        cmd += "start " + svName + " 2>&1; echo $?";
    else
        cmd += "restart "+svName + " 2>&1; echo $?";

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
