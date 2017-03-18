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
    QString cmd = "id -u; echo $?";
    cmd = GetCmdRes(cmd).trimmed();
    QStringList strl = cmd.split('\n');
    if(strl[strl.length()-1].toInt()!=0)
    {
        qDebug()<<"id -u execute failed";
        return NORMALUSER;
    }
    cmd = strl[0];
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
    QString cmd = "awk -F: \'{print $1}\' /etc/passwd; echo $?";
    cmd = GetCmdRes(cmd).trimmed();
    QStringList strl = cmd.split('\n');
    if(strl[strl.length()-1].toInt()!=0)
    {
        qDebug()<<"get_users command:\"awk -F: \'{print $1}\' /etc/passwd\" execute failed";
        return QStringList();
    }
    qDebug()<<cmd;

    strl.removeLast();
    return strl;
}

QStringList get_groups()
{
    QString cmd = "awk -F: \'{print $1}\' /etc/group; echo $?";
    cmd = GetCmdRes(cmd).trimmed();
     QStringList strl = cmd.split('\n');
    if(strl[strl.length()-1].toInt()!=0)
    {
        qDebug()<<"get_groupss command:\"awk -F: \'{print $1}\' /etc/passwd\" execute failed";
        return QStringList();
    }
    qDebug()<<cmd;
    strl.removeLast();
    return strl;
}

QString get_cur_user()
{
    QString cmd = "whoami; echo $?";
    cmd = GetCmdRes(cmd).trimmed();
    QStringList strl = cmd.split('\n');
   if(strl.last().toInt()!=0)
   {
       qDebug()<<"get_cur_user command:\"awk -F: \'{print $1}\' /etc/passwd\" execute failed";
       return "";
   }
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


struct cpu_record g = { 0, };

int  get_n_cpu()
{
    //cpu 个数
    const glibtop_sysinfo *info;
    info = glibtop_get_sysinfo();
    return info->ncpu;
}

void get_load (std::vector<CPU>  &cpus)
{
    guint i;
    glibtop_cpu cpu;
    glibtop_get_cpu (&cpu);
    int n_cpu = get_n_cpu();
    cpus.clear();

#undef NOW
#undef LAST
#define NOW (g.times[g.now])
#define LAST (g.times[g.now ^ 1])

    if (n_cpu == 1)
    {
        NOW[0][CPU_TOTAL] = cpu.total;
        NOW[0][CPU_USED] = cpu.user + cpu.nice + cpu.sys;
    }
    else
    {
        for (i = 0; i < n_cpu; i++)
        {
            NOW[i][CPU_TOTAL] = cpu.xcpu_total[i];
            NOW[i][CPU_USED] = cpu.xcpu_user[i] + cpu.xcpu_nice[i]
                + cpu.xcpu_sys[i];
        }

    }

    // on the first call, LAST is 0
    // which means data is set to the average load since boot
    // that value has no meaning, we just want all the
    // immediately
    for (i = 0; i < n_cpu; i++)
    {
        float load;
        float total, used;

        total = NOW[i][CPU_TOTAL] - LAST[i][CPU_TOTAL];
        used = NOW[i][CPU_USED] - LAST[i][CPU_USED];

        load = used / MAX(total, 1.0f);
        g_print("load[%d]=%.1f%%/n",i,load*100);
        CPU cpu;
        cpu.useage = load*100;
        cpus.push_back(cpu);

    }
    g.now ^= 1;

#undef NOW
#undef LAST

}

void getmem(MeM &memInfo)
{
    float mempercent;

    glibtop_mem mem;

    glibtop_get_mem (&mem);

    /* There's no swap on LiveCD : 0.0f is better than NaN :) */
    mempercent  = (float)mem.user  / (float)mem.total;
    memInfo.total = mem.total;
    memInfo.used = mem.used;
    memInfo.percent = mempercent;

    g_print("mem.total = %d   mem.used=%d mempercent=%.3f\n", mem.total,mem.used,mempercent);
}

void get_swap(SWAP &swapInfo)
{
    float swappercent;
     glibtop_swap swap;
     glibtop_get_swap (&swap);
     swappercent = (swap.total ? (float)swap.used / (float)swap.total : 0.0f);
    swapInfo.total = swap.total;
    swapInfo.used = swap.used;
    swapInfo.percent = swappercent;

     g_print("swap.total = %d   swap.used=%d swappercent=%.3f\n", swap.total,swap.used,swappercent);
}

#define MAX_SIZE  255

QString getCwdPath()
{
    char current_absolute_path[MAX_SIZE];
    int cnt = readlink("/proc/self/exe", current_absolute_path, MAX_SIZE);
    if (cnt < 0 || cnt >= MAX_SIZE)
    {
        qCritical()<<"***Error***\n";
        //exit(-1);
    }

    int i;
    for (i = cnt; i >=0; --i)
    {
        if (current_absolute_path[i] == '/')
        {
            current_absolute_path[i+1] = '\0';
            break;
        }
    }
    return QString(current_absolute_path);
}

QString getHintNum(quint64 size)
{
    long g = 1024 * 1024 * 1024;
    long m = 1024 * 1024;
    long k = 1024;
    if (size < k && size >= 1)
    {
        return QString("%1B").arg(size);
    }
    else if (size < m && size >= 1024)
    {
        return QString("%1KB").arg(size/(float)k, 0, 'f', 1);
    }
    else if (size < g && size >= m)
    {
        return QString("%1MB").arg(size/(float)m, 0, 'f', 1);
    }
    else if (size >= g)
    {
        return QString("%1GB").arg(size/(float)g, 0, 'f', 1);
    }else
        return "0B";
}


void
fsusage_stats(const glibtop_fsusage *buf,
              guint64 *bused, guint64 *bfree, guint64 *bavail, guint64 *btotal,
              gint *percentage)
{
    guint64 total = buf->blocks * buf->block_size;

    if (!total) {
        /* not a real device */
        *btotal = *bfree = *bavail = *bused = 0ULL;
        *percentage = 0;
    } else {
        int percent;
        *btotal = total;
        *bfree = buf->bfree * buf->block_size;
        *bavail = buf->bavail * buf->block_size;
        *bused = *btotal - *bfree;
        /* percent = 100.0f * *bused / *btotal; */
        percent = 100 * *bused / (*bused + *bavail);
        *percentage = CLAMP(percent, 0, 100);
    }
}

void get_fsInfo(QList<DISK> &disks)
{

    disks.clear();
    glibtop_mountlist list;
    glibtop_mountentry *entries = glibtop_get_mountlist(&list, 1);

    for (unsigned i = 0; i != list.number; ++i) {
        struct stat buf;

        if (stat(entries[i].devname, &buf) != -1)
        {
            DISK disk;
            disk.devid = buf.st_rdev;
            disk.devName = QString(entries[i].devname);
            disk.mountDir = QString(entries[i].mountdir);
            disk.ftype = QString(entries[i].type);

            glibtop_fsusage usage;

            glibtop_get_fsusage(&usage, entries[i].mountdir);

            fsusage_stats(&usage, &disk.used, &disk.bfree, &disk.bavail, &disk.btotal, &disk.percent);
            disks.append(disk);
        }
    }

   g_free(entries);
}

void shutdown()
{
    QString cmd = "shutdown now";
    cmd = GetCmdRes(cmd);
}

void reboot()
{
    QString cmd = "reboot";
    cmd = GetCmdRes(cmd);
}

void logout()
{
    QString cmd = "gnome-session-quit --logout --no-prompt";
    cmd = GetCmdRes(cmd);
}

bool get_trylock_services(QStringList &list)
{
    list.clear();
    QString cmd = "enhanced-trylock  -l;echo $?";
    cmd = GetCmdRes(cmd).trimmed();
    list = cmd.split("\n");
    if(list.last().toInt()!=0)
    {
        qDebug()<<"enhanced-trylock -l failed";
        list.clear();
        return false;
    }
    list.removeFirst();     //delete "avilable:"
    list.removeLast();
    return true;
}

bool trylock_service(QString cmd)
{
     cmd = GetCmdRes(cmd).trimmed();
     QStringList list = cmd.split("\n");
     if(list.last().toInt()!=0)
     {
         qDebug()<<"trylock service failed";
         return false;
     }
     return true;
}

//bool get_all_locked_users(QStringList &list)
//{
//    QString cmd = "pam_tally2 |sed \'s/ / /\' | awk \'{print $1}\'; echo $?";
//    cmd = GetCmdRes(cmd).trimmed();
//    list = cmd.split('\n');
//    list.removeFirst();
//    if(list.last().toInt()!=0)
//    {

//    }
//}

bool unlock_all_users()
{
    QString cmd = "pam_tally2 -r; echo $?";
    cmd = GetCmdRes(cmd).trimmed();
    QStringList list = cmd.split("\n");
    if(list.last().toInt()!=0)
    {
        qDebug()<<"pam_tally2 -r failed";
        return false;
    }
    return true;
}

bool set_pwd_rule(QString cmd)
{
    cmd = GetCmdRes(cmd).trimmed();
    QStringList list = cmd.split("\n");
    if(list.last().toInt()!=0)
    {
        qDebug()<<"set_pwd_rule failed";
        return false;
    }
    return true;
}











