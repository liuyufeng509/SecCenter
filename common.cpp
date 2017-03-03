#include"common.h"

QString GetCmdRes(QString cmd)
{
    FILE *pf;
    char buff[1000]={};
    QString res = "";
    pf = popen(cmd.toStdString().c_str(), "r");
    //fread(buff,sizeof(buff),sizeof(buff),pf);
    while(!feof(pf))
    {
        fread(buff,sizeof(buff)-1,1,pf);
        res += QString(buff);
       // qDebug()<<buff;
        memset(buff,0,sizeof(buff));
    }
    pclose(pf);
    //qDebug()<<res;
    return res;
}

ROLE get_user_role()
{
    QString cmd = "id -u";
    cmd = GetCmdRes(cmd).trimmed();
    ROLE res = NORMALUSER;
    if(cmd.toInt()==0)
    {
        res = ROOT;
    }

    cmd = get_user_str_role();
    if(cmd == "unconfined_r" && res !=ROOT)
        res = NORMALUSER;
    else if(cmd == "secadmin_r")
        res = SECADMIN;
    else if(cmd == "sysadmin_r")
        res = SYSADMIN;
    else if(cmd == "audadmin_r")
        res = AUDIADMIN;

    return res;
}

QStringList get_users()
{
    QString cmd = "awk -F: \'{print $1}\' /etc/passwd";
    cmd = GetCmdRes(cmd);
    qDebug()<<cmd;
    //return cmd;
    QStringList list = cmd.split('\n');
    list.removeAt(list.length()-1);
    return list;
}

QStringList get_groups()
{
    QString cmd = "awk -F: \'{print $1}\' /etc/group";
    cmd = GetCmdRes(cmd);
    qDebug()<<cmd;
    QStringList list = cmd.split('\n');
    list.removeAt(list.length()-1);
    return list;
}

QString get_cur_user()
{
    QString cmd = "whoami";
    cmd = GetCmdRes(cmd).trimmed();
    return cmd;
}

int get_cur_user_id()
{
    QString cmd = "id -u";
    cmd = GetCmdRes(cmd);
    return cmd.toInt();
}


QString get_cur_group()
{
    QString cmd = "id -gn";
    cmd = GetCmdRes(cmd);
    return cmd;
}

int get_cur_group_id()
{
    QString cmd = "id -g";
    cmd = GetCmdRes(cmd).trimmed();
    return cmd.toInt();
}

QString get_usr_name_by_id(int id)
{
    QString cmd = "awk -F: \'{if($3=="+QString::number(id)+"){print $1}}\'  /etc/passwd";
    cmd = GetCmdRes(cmd);
    return cmd;
}

QString get_usr_id_by_name(QString name)
{
    QString cmd = "awk -F: \'{if($1==\""+name+"\"){print $3}}\'  /etc/passwd";
    cmd = GetCmdRes(cmd).trimmed();
    return cmd;
}


QString get_enforce()
{
    QString cmd = "getenforce";
    cmd = GetCmdRes(cmd).trimmed();
    return cmd;
}

QString get_user_str_role()
{
    if(get_cur_user()=="root")
        return "root_r";
    QString cmd = "id -Z | awk -F \':\' \'{print $2}\'";
    cmd = GetCmdRes(cmd).trimmed();
    return cmd;
}

void set_userinfo(QList<UserInfo> &users)
{
    QString cmd = "awk -F: \'{print $3,$1}\'  /etc/passwd";
    cmd =GetCmdRes(cmd);
    QStringList list = cmd.split('\n');
    list.removeAt(list.length()-1);
    users.clear();
    for(int i=0; i<list.length(); i++)
    {
        UserInfo usrinfo;
        QStringList tmpl = list[i].split(' ');
        usrinfo.uid = tmpl[0].trimmed();
        usrinfo.uname = tmpl[1].trimmed();
        users.append(usrinfo);
    }
}

void set_userinfos_groups(QList<UserInfo> &users)
{
    for(int i=0; i<users.length(); i++)
    {
        QString cmd = "groups "+users[i].uname;
        cmd = GetCmdRes(cmd);
        QStringList list = cmd.split(':');
        if(list.length()<2)
        {
            qDebug()<<"user:"+users[i].uname<<"'s group list lenght error";
            return;
        }
        QStringList glist = list[1].trimmed().split(' ');
        if(glist.length()<1)
        {
            qDebug()<<"user:"+users[i].uname<<"'s group list2 lenght error";
            return;
        }
        users[i].group =glist[0].trimmed();
        for(int j=1; j<glist.length(); j++)
        {
            users[i].othgroups.append(glist[j].trimmed());
        }
    }
}


QString add_user(UserInfo userinfo)
{
    QString othgrps="";
    if(userinfo.othgroups.length()>0)
    {
        foreach (QString grp, userinfo.othgroups) {
            othgrps +=grp+",";
        }
        othgrps = othgrps.left(othgrps.length()-1);
    }
    QString cmd = "useradd " +
            (userinfo.group.length()!=0 && userinfo.group!=userinfo.uname ?" -g "+userinfo.group: " ")+
            (userinfo.othgroups.length()==0?" ":" -G " +othgrps) +" "+ userinfo.uname;
    cmd = GetCmdRes(cmd).trimmed();
    return cmd;
}

bool is_group_exist(QString groupname)
{
    QStringList grouplist = get_groups();

    return (grouplist.indexOf(groupname)==-1?false:true);
}

QString change_groups(UserInfo userinfo)
{
    if(userinfo.group.length()==0)
        return "group lenth is 0";

    QString othgrps="";
    if(userinfo.othgroups.length()>0)
    {
        foreach (QString grp, userinfo.othgroups) {
            othgrps +=grp+",";
        }
        othgrps = othgrps.left(othgrps.length()-1);
    }

    QString cmd = "usermod -g "+ userinfo.group +
            (userinfo.othgroups.length()==0?" ":" -G " +othgrps)+" "+ userinfo.uname;
    cmd = GetCmdRes(cmd).trimmed();
    return cmd;
}

QString del_user(UserInfo userinfo)
{
    QString cmd = "userdel "+ userinfo.uname;
    cmd = GetCmdRes(cmd).trimmed();
    return cmd;
}

RUNSTATE is_serv_running(QString svName)
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

bool get_services(QList<ServiceInfo> &sevrs)
{
    sevrs.clear();
    QString cmd = "systemctl list-unit-files --type=service";
    cmd = GetCmdRes(cmd).trimmed();
  //  qDebug()<<cmd;

    QStringList slist = cmd.split('\n');
    if(slist.length()<3)
    {
        qDebug()<<"row number less than 3";
        return false;
    }
    slist.pop_front();
    slist.pop_back();
    slist.pop_back();
    foreach (QString s, slist) {
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

bool up_service_when_start(QString sname)
{
    QString cmd = "systemctl enable " + sname;
    cmd = GetCmdRes(cmd).trimmed();
    if(cmd.length()==0||cmd.contains("Created symlink"))
    {
        return true;
    }else
        return false;

    if(cmd.contains("no [Install] section"))
    {
        return false;
    }
}


bool down_service_when_start(QString sname)
{
    QString cmd = "systemctl disable " + sname;
    cmd = GetCmdRes(cmd).trimmed();
    if(cmd.length()==0|| cmd.contains("Removed symlink"))
    {
        return true;
    }else
        return false;
}


bool stop_service(QString sname)
{
    QString cmd = "systemctl stop  " + sname;
    cmd = GetCmdRes(cmd).trimmed();
    if(cmd.length()==0)
    {
        return true;
    }else
        return false;

    if(cmd.contains("Failed to stop"))
        return false;
}

bool start_service(QString sname)
{
    QString cmd = "systemctl start  " + sname;
    cmd = GetCmdRes(cmd).trimmed();
    RUNSTATE st = is_serv_running(sname);
    if(st==RUNNING || st==EXIT)
        return true;
    else
        return false;
}
