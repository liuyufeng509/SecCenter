#include "mainfunclass.h"

MainFunClass::MainFunClass(QObject *parent) : QObject(parent)
{

}

ROLE MainFunClass::getUserRole() /* throw()*/
{
    QString cmd = "id -Z 2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
	//执行命令，如果返回0，失败，抛出异常
    if(strl.last().toInt()!=0)
    {
		resStr.chop(strl.last().length());
        QString errContent =tr("执行操作：获取当前用户角色失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
		qDebug()<<errContent;
        throw Exception(strl.last(), errContent); 
    }
    QStringList rl = strl[0].split(':');
	//解析结果，获取第二个字段
	if(rl.length()<2)
	{
        QString errContent  = tr("执行命令：")+cmd+tr("失败\n错误内容：解析命令结果失败");
		qDebug()<<errContent;
		throw Exception("", errContent);
	}	
    ROLE res = ERROR;
	
    if(rl[1] == "secadm_r")
        res = SECADMIN;
    else if(rl[1] == "sysadm_r")
        res = SYSADMIN;
    else if(rl[1] == "auditadm_r")
        res = AUDADMIN;
	else
		res = ERROR;
    // qDebug()<<"res="<<res<<" rl[0]="<<rl[0];
	 return res;
}

