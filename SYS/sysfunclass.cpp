#include "sysfunclass.h"
#include <QThread>
SysFunClass::SysFunClass(QObject *parent) : QObject(parent)
{

}

bool SysFunClass::getUserList(QList<UserInfo> &ulist)
{
    QString cmd = "awk -F: \'{print $3,$1}\'  /etc/passwd  2>&1; echo $?";
    QString resStr =GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    ulist.clear();
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
		QString errContent=tr("执行操作：获取当前用户列表失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
		qDebug()<<errContent;
        throw Exception(strl.last(), errContent); 
    }
    strl.removeLast();
    for(int i=0; i<strl.length(); i++)
    {
        UserInfo usrinfo;
        QStringList tmpl = strl[i].split(' ');
        usrinfo.uid = tmpl[0].trimmed();
        usrinfo.uname = tmpl[1].trimmed();
        ulist.append(usrinfo);
    }
    
	//验证在/etc/shadow中的情况，判断是否是曾经存在的用户
    try
    {
        getUserListOfShaddow(ulist);
    }catch(Exception exp)
    {
        ulist.clear();
        throw exp;
    }

    //获取每个用户的用户组
    try
    {
        getGroupsOfUserList(ulist);
    }catch (Exception exp)
    {
        ulist.clear();
        throw exp;
    }

	return true;
}

bool SysFunClass::getUserListOfShaddow(QList<UserInfo> &users)
{
    QString cmd = "awk -F: \'{print $2}\' /etc/shadow  2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList list = resStr.split('\n');
    if(list.last().toInt()!=0)
    {
        resStr.chop(list.last().length());
        QString errContent=tr("执行操作：获取当前用户列表失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+list.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(list.last(), errContent);
        return false;
    }
    list.removeLast();
    if(list.length()!=users.length())
    {
        QString errContent=tr("执行操作：获取当前用户列表失败")+ tr("\n执行命令：")+cmd+tr("\n错误内容：/etc/shaddow中用户与/etc/pawwd不一致");
        qDebug()<<errContent;
        throw Exception(list.last(), errContent);
        return false;
    }
    for(int i=0; i<users.length();i++)
    {
        if(list[i] == "!!!" || users[i].uid.toInt()<1000)
            users[i].isShow = false;
        else
            users[i].isShow = true;
    }

    return true;
}

bool SysFunClass::getGroupsOfUserList(QList<UserInfo> &users)
{
    for(int i=0; i<users.length(); i++)
    {
        QString cmd = "groups "+users[i].uname + " 2>&1; echo $?";
        QString resStr = GetCmdRes(cmd).trimmed();
        QStringList strl = resStr.split('\n');
        if(strl.last().toInt()!=0)
        {
            resStr.chop(strl.last().length());
            QString errContent=tr("执行操作：获取当前用户的用户组失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
            qDebug()<<errContent;
            throw Exception(strl.last(), errContent);
        }
        resStr.chop(strl.last().length());
        QStringList list = resStr.split(':');
        if(list.length()<2)
        {
            QString errContent = tr("执行操作：获取当前用户的用户组失败")+ tr("\n执行命令：")+cmd+tr("\n错误内容：解析用户组失败");
            qDebug()<<errContent;
            throw Exception("", errContent);
        }
        QStringList glist = list[1].trimmed().split(' ');
        if(glist.length()<1)
        {
            QString errContent = tr("执行操作：获取当前用户的附属组失败")+ tr("\n执行命令：")+cmd+tr("\n错误内容：解析附属组失败");
            qDebug()<<errContent;
            throw Exception("", errContent);
            return false;
        }
        users[i].group =glist[0].trimmed();
        for(int j=1; j<glist.length(); j++)
        {
            users[i].ogroups.append(glist[j].trimmed());
        }
    }

    return true;
}

bool SysFunClass::getGroupList(QStringList &groups)
{
    QString cmd = "awk -F: \'{print $1}\' /etc/group 2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
     QStringList strl = resStr.split('\n');
     groups.clear();
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：获取当前所有用户组失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    strl.removeLast();
    groups = strl;
    return true;
}

bool SysFunClass::isGroupExist(QString groupName)
{
    try
    {
        QStringList grouplist;
        getGroupList(grouplist);
        return (grouplist.indexOf(groupName)==-1?false:true);
    }catch(Exception exp)
            {
        throw exp;
    }
    return true;
}

bool SysFunClass::addUser(UserInfo userInfo)
{
    QString othgrps="";
    if(userInfo.ogroups.length()>0)
    {
        foreach (QString grp, userInfo.ogroups) {
            othgrps +=grp+",";
        }
        othgrps = othgrps.left(othgrps.length()-1);
    }
    QString cmd = "useradd " +
            (userInfo.group.length()!=0 && userInfo.group!=userInfo.uname ?" -g "+userInfo.group: " ")+
            (userInfo.ogroups.length()==0?" ":" -G " +othgrps) +" "+ userInfo.uname +
            (userInfo.uid.isEmpty()? "":" -u "+userInfo.uid) + " 2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
     {
        resStr.chop(strl.last().length());
        QString errContent =tr("执行操作：添加用户失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    return true;
}

bool SysFunClass::modifyUser(UserInfo userInfo)
{
    if(userInfo.group.length()==0)
        throw Exception("",tr("主组不能为空！"));

    QString othgrps="";
    if(userInfo.ogroups.length()>0)
    {
        foreach (QString grp, userInfo.ogroups) {
            othgrps +=grp+",";
        }
        othgrps = othgrps.left(othgrps.length()-1);
    }

    QString cmd = "usermod -g "+ userInfo.group +
            (userInfo.ogroups.length()==0?" -G \"\"":" -G " +othgrps)+" "+ userInfo.uname+" 2>&1; echo $?";
    QString resStr= GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent =tr("执行操作：修改用户失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    return true;
}

bool SysFunClass::delUser(UserInfo userInfo)
{
    QString cmd = "userdel "+ userInfo.uname +" 2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent =tr("执行操作：删除用户失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    return true;
}

void SysFunClass::getServicesSlot(SEVLIST sevrs)
{
    try
    {
        getServices(sevrs);
        emit emitGetServicesDone(0, Exception("0","0"), sevrs);
    }catch(Exception exp)
            {
        emit emitGetServicesDone(1, exp,sevrs);
    }
}

bool SysFunClass::getServices(QList<ServiceInfo> &sevrs)
{
    sevrs.clear();
    QString cmd = "systemctl list-unit-files --type=service 2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();

    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent =tr("执行操作：获取服务列表失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    if(strl.length()<4)
    {
        QString errContent =tr("执行操作：获取服务列表失败")+ tr("\n错误内容：解析结果失败");
        qDebug()<<errContent;
        throw Exception("", errContent);
    }
    strl.pop_front();
    strl.pop_back();
    strl.pop_back();
    strl.pop_back();
    foreach (QString s, strl) {
        QStringList info = s.simplified().split(' ');
        if(info.length()<2)
            continue;
      //  qDebug()<<s.simplified();
        ServiceInfo servInfo;
        servInfo.sName = info[0];
        if(info[1]=="static")
            servInfo.cfgStatus =STATIC;
        if(info[1]=="disabled")
            servInfo.cfgStatus = DISABLE;
        if(info[1]=="enabled")
            servInfo.cfgStatus = ENABLE;

        servInfo.runStat = is_serv_running(servInfo.sName);
        sevrs.append(servInfo);
    }
    if(sevrs.length()>0)
        return true;
    else
        return false;
}

RUNSTATE SysFunClass::servRunState(QString svName)
{
    QString cmd= "systemctl status "+ svName;
    cmd = GetCmdRes(cmd).trimmed();
    if(cmd.contains("Active: active (running)"))
    {
        return RUNNING;
    }else if(cmd.contains("Active: inactive"))
        return DEAD;
    else if(cmd.contains("Active: active (exited)"))
        return EXIT;
    else
        return OTHER;
}

bool SysFunClass::setUpDownWhenBoot(QString svName, int opt)                  //设置开启启动
{
    QString cmd = "systemctl ";
    if(opt == 0)
        cmd += "enable " + svName + " 2>&1; echo $?";
    else
        cmd += "disable " + svName + " 2>&1; echo $?";

    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：设置开机启动失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    return true;
}

void SysFunClass::setUpDownWhenBootSlot(QString svName, int opt)
{
    try
    {
        setUpDownWhenBoot(svName,opt);
        emit emitSetUpDownWhenBootDone(0, Exception("0","0"));
    }catch(Exception exp)
            {
        emit emitSetUpDownWhenBootDone(1, exp);
    }
}

bool SysFunClass::startOrStopService(QString svName, int opt)      //开启或关闭服务
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

void SysFunClass::startOrStopServiceSlot(QString svName, int opt)
{
    try
    {
        startOrStopService(svName, opt);
        emit emitStartOrStopServiceDone(0, Exception("",""));
    }catch(Exception exp)
            {
        emit emitStartOrStopServiceDone(1, exp);
    }
}

bool SysFunClass::setKernelParam(QString paramName, QString value)
{
    QString cmd = "echo "+value+" >"+paramName+" 2>&1 ;echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent =tr("执行操作：设置系统内核参数失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    return true;
}
