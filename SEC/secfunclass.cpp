#include "secfunclass.h"
#include <QFile>
#include <QTextStream>
#include "qreadconfig.h"
SecFunClass* SecFunClass::m_pInstance = NULL;
SecFunClass::SecFunClass(QObject *parent) : QObject(parent)
{

}

bool SecFunClass::setRmOpened(bool isOpen)
{
    QFile file("/etc/bashrc");
    QString fileStr;
    QStringList fileConList;
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream inout(&file);
        fileStr = inout.readAll();
        fileConList = fileStr.split('\n');
        if(isOpen)
            {
            if(fileConList.count("alias \"rm=rm -z\"")==0)
                {
                fileConList<<"alias \"rm=rm -z\"";
            }
        }else
            {
            if(fileConList.count("alias \"rm=rm -z\"")!=0)
                {
                fileConList.removeAll("alias \"rm=rm -z\"");
            }
        }
        fileStr = "";
        for(int i=0; i<fileConList.count(); i++)
        {
            fileStr += fileConList[i]+"\n";
        }
        file.close();
    }
    else
    {
      throw Exception("", file.errorString());
    }
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream inout(&file);
        inout<<fileStr;
        inout.flush();
        file.close();
    }
    else
    {
        QString errContent=tr("执行操作：设置客体重用开关")+tr("\n错误内容：")+file.errorString();
        qDebug()<<errContent;
        throw Exception("", errContent);
    }
    return true;
}

bool SecFunClass::isRmOpened(bool &isOpen)             //客体重用是否启用
{
    QFile file("/etc/bashrc");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&file);
        QString fileStr = in.readAll();
        QStringList list = fileStr.split('\n');
        if(list.count("alias \"rm=rm -z\"")==0)
        {
            isOpen = false;
        }else
            {
            isOpen = true;
        }

        file.close();
    }
    else
    {
        throw Exception("", file.errorString());
    }
    return true;
}

bool SecFunClass::getLockServices(QStringList &list)
{
    list.clear();
    QString cmd = "nfs-enhanced-trylock  -l 2>&1;echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    list = resStr.split("\n");
    if(list.last().toInt()!=0)
    {
        resStr.chop(list.last().length());
        QString errContent=tr("执行操作：获取锁定服务列表失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+list.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(list.last(), errContent);
    }
    if(list.count()<2)
        {
        QString errContent=tr("执行操作：获取锁定服务列表失败")+ tr("\n错误内容：解析服务列表失败");
        qDebug()<<errContent;
        throw Exception("", errContent);
    }
    list.removeFirst();     //delete "avilable:"
    list.removeLast();
    return true;
}

bool SecFunClass::getLockedUsers(QStringList &list)
{
    list.clear();
    QString cmd = "pam_tally2  2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：获取锁定的用户列表失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    strl.removeLast();
    QDateTime now = QDateTime::currentDateTime();
    for(int i=1; i<strl.count(); i++)
    {
        QStringList tmpl = strl[i].simplified().split(' ');
        if(tmpl.count()!=5)
            continue;
        QDate date;QTime time;
        QDateTime dateTime(date.fromString(tmpl[2], "MM/dd/yy"),time.fromString(tmpl[3], "hh:mm:ss"));
       dateTime =  dateTime.addYears(now.date().year()-dateTime.date().year());
        //int secs = now.secsTo(dateTime);
        int secs = dateTime.secsTo(now);
        TryLockInfo info;
        if(tmpl[4].contains("unknown"))
        {
            info.sParam="gdm";
        }else if(tmpl[4].contains("tty"))
        {
            info.sParam="login";
        }else
            {
            info.sParam = "ssh";
        }
        try
        {
            getCurLockInfo(info);
        }catch(Exception exp)
        {
            throw exp;
        }
        qDebug()<<info.sParam;
        if(secs<info.uParam.toInt()&&tmpl[1].toInt()>info.dParam.toInt())
        {
            list<<tmpl[0];
        }
    }
    return true;

}

bool SecFunClass::unLockUser(QString uName)
{
    QString cmd = "pam_tally2 -u " +uName+ " -r 2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：解锁用户失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    return true;
}

bool SecFunClass::tryLockOption(TryLockInfo info)           //设置锁定规则
{
    QString cmd = "nfs-enhanced-trylock -d "+info.dParam + " -u "
            + info.uParam+ " -s " + info.sParam + " 2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：设置用户锁定规则")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    return true;
}

bool SecFunClass::setCurPwdInfo(PwdInfo &pwdInfo)
{
    QString cmd = "nfs-enhanced-passwd "+
            (pwdInfo.minLen.isEmpty()? " ": ("-m "+pwdInfo.minLen+ " "))+
            (pwdInfo.dcredit.isEmpty()? " ": ("-d -"+pwdInfo.dcredit+ " "))+
            (pwdInfo.ucredit.isEmpty()? " ": ("-u -"+pwdInfo.ucredit+ " "))+
            (pwdInfo.lcredit.isEmpty()?  " ": ("-l -"+pwdInfo.lcredit+ " "))+
            (pwdInfo.ocredit.isEmpty()? " ": ("-o -"+pwdInfo.ocredit+ " "))+
            " 2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：设置密码规则")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    return true;
}

bool SecFunClass::getCurPwdInfo(PwdInfo &pwdInfo)
{
    QString cmd = "grep \"^password.*pam_pwquality.so\"  /etc/pam.d/system-auth-ac 2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：获取当前密码规则失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    resStr.chop(strl.last().length());
    resStr = resStr.simplified();
    QStringList list  = resStr.split(" ");
    for(int i=0; i<list.length(); i++)
    {
        if(list[i].contains("minlen"))
        {
            pwdInfo.minLen = list[i].right(list[i].length()-list[i].indexOf('=')-1);
        }else if(list[i].contains("dcredit"))
        {
            pwdInfo.dcredit = list[i].right(list[i].length()-list[i].indexOf('-')-1);
        }else if(list[i].contains("ucredit"))
        {
            pwdInfo.ucredit = list[i].right(list[i].length()-list[i].indexOf('-')-1);
        }else if(list[i].contains("lcredit"))
        {
            pwdInfo.lcredit = list[i].right(list[i].length()-list[i].indexOf('-')-1);
        }else if(list[i].contains("ocredit"))
        {
            pwdInfo.ocredit = list[i].right(list[i].length()-list[i].indexOf('-')-1);
        }
    }

    return true;
}

bool SecFunClass::getCurLockInfoOfLogin(TryLockInfo &info)                  //获取login的锁定规则
{
    QString cmd = "cat /etc/pam.d/login 2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：获取login服务的锁定规则失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    strl.removeLast();
    for(int i=0; i<strl.count(); i++)
    {
        if(strl[i].contains("pam_tally2.so"))
        {
            QStringList tmpl = strl[i].simplified().split(' ');
            for(int j=0; j<tmpl.count();j++)
            {
                if(tmpl[j].contains("deny="))
                {
                    info.dParam = tmpl[j].right(tmpl[j].length() - tmpl[j].indexOf("=")-1);
                }
                if(tmpl[j].contains("unlock_time="))
                {
                    info.uParam = tmpl[j].right(tmpl[j].length() - tmpl[j].indexOf("=")-1);
                }
            }
        }
    }

    return true;
}

bool SecFunClass::getCurLockInfoOfSsh(TryLockInfo &info)                    //获取ssh的锁定规则
{
    QString cmd = "cat /etc/pam.d/sshd 2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：获取ssh服务的锁定规则失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }

    strl.removeLast();
    for(int i=0; i<strl.count(); i++)
    {
        if(strl[i].contains("pam_tally2.so"))
        {
            QStringList tmpl = strl[i].simplified().split(' ');
            for(int j=0; j<tmpl.count();j++)
            {
                if(tmpl[j].contains("deny="))
                {
                    info.dParam = tmpl[j].right(tmpl[j].length() - tmpl[j].indexOf("=")-1);
                }
                if(tmpl[j].contains("unlock_time="))
                {
                    info.uParam = tmpl[j].right(tmpl[j].length() - tmpl[j].indexOf("=")-1);
                }
            }
        }
    }

    return true;
}

bool SecFunClass::getCurLockInfoOfGdm(TryLockInfo &info)                    //获取gdm的锁定规则
{
    QString cmd = "cat /etc/pam.d/gdm-password 2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：获取gdm服务的锁定规则失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }

    strl.removeLast();
    for(int i=0; i<strl.count(); i++)
    {
        if(strl[i].contains("pam_tally2.so"))
        {
            QStringList tmpl = strl[i].simplified().split(' ');
            for(int j=0; j<tmpl.count();j++)
            {
                if(tmpl[j].contains("deny="))
                {
                    info.dParam = tmpl[j].right(tmpl[j].length() - tmpl[j].indexOf("=")-1);
                }
                if(tmpl[j].contains("unlock_time="))
                {
                    info.uParam = tmpl[j].right(tmpl[j].length() - tmpl[j].indexOf("=")-1);
                }
            }
        }
    }

    return true;
}

bool SecFunClass::getCurLockInfo(TryLockInfo &info)                     //获取当前的用户锁定规则
{
    try
    {
        if(info.sParam == "login")
        {
            return getCurLockInfoOfLogin(info);
        }
        if(info.sParam == "ssh")
        {
            return getCurLockInfoOfSsh(info);
        }
        if(info.sParam == "gdm")
        {
            return getCurLockInfoOfGdm(info);
        }
    }catch(Exception exp)
    {
        throw exp;
    }
    return true;
}

bool SecFunClass::getSecStatus(SecStatus &status)
{
    QString cmd = "sestatus 2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：获取当前安全状态失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    if(strl.count()!=10)
        {
        QString errContent=tr("执行操作：获取当前安全状态失败")+tr("\n错误内容：结果解析失败");
        qDebug()<<errContent;
        throw Exception("", errContent);
    }

    strl.removeLast();      //去掉echo $?结果
    QStringList tmpl = strl[0].simplified().split(' ');
    if(tmpl.count()!=3)
        {
        QString errContent=tr("执行操作：获取当前安全状态失败")+tr("\n错误内容：selinux status解析失败");
        qDebug()<<errContent;
        throw Exception("", errContent);
    }
    status.selinux_status = tmpl.last();

    tmpl = strl[1].simplified().split(' ');
    if(tmpl.count()!=3)
        {
        QString errContent=tr("执行操作：获取当前安全状态失败")+tr("\n错误内容：SELinuxfs mount解析失败");
        qDebug()<<errContent;
        throw Exception("", errContent);
    }
    status.selinux_fs_mount = tmpl.last();

    tmpl = strl[2].simplified().split(' ');
    if(tmpl.count()!=4)
        {
        QString errContent=tr("执行操作：获取当前安全状态失败")+tr("\n错误内容：SELinux root directory解析失败");
        qDebug()<<errContent;
        throw Exception("", errContent);
    }
    status.selinux_root_dir =  tmpl.last();

    tmpl = strl[3].simplified().split(' ');
    if(tmpl.count()!=4)
        {
        QString errContent=tr("执行操作：获取当前安全状态失败")+tr("\n错误内容：Loaded policy name解析失败");
        qDebug()<<errContent;
        throw Exception("", errContent);
    }
    status.load_policy_name =  tmpl.last();

    tmpl = strl[4].simplified().split(' ');
    if(tmpl.count()!=3)
        {
        QString errContent=tr("执行操作：获取当前安全状态失败")+tr("\n错误内容：Current mode解析失败");
        qDebug()<<errContent;
        throw Exception("", errContent);
    }
    status.curr_mode =  tmpl.last();

    tmpl = strl[5].simplified().split(' ');
    if(tmpl.count()!=5)
        {
        QString errContent=tr("执行操作：获取当前安全状态失败")+tr("\n错误内容：Mode from config file解析失败");
        qDebug()<<errContent;
        throw Exception("", errContent);
    }
    status.mode_frm_cfg =  tmpl.last();

    tmpl = strl[6].simplified().split(' ');
    if(tmpl.count()!=4)
        {
        QString errContent=tr("执行操作：获取当前安全状态失败")+tr("\n错误内容：Policy MLS status解析失败");
        qDebug()<<errContent;
        throw Exception("", errContent);
    }
    status.mls_status =  tmpl.last();

    tmpl = strl[7].simplified().split(' ');
    if(tmpl.count()!=4)
        {
        QString errContent=tr("执行操作：获取当前安全状态失败")+tr("\n错误内容：Policy deny_unknown status解析失败");
        qDebug()<<errContent;
        throw Exception("", errContent);
    }
    status.policy_deny_stat =  tmpl.last();

    tmpl = strl[8].simplified().split(' ');
    if(tmpl.count()!=5)
        {
        QString errContent=tr("执行操作：获取当前安全状态失败")+tr("\n错误内容：Max kernel policy version解析失败");
        qDebug()<<errContent;
        throw Exception("", errContent);
    }
    status.max_kern_policy_version =  tmpl.last();

    return true;
}

bool SecFunClass::getUserNames(QStringList &users)
{
    users.clear();
    QString cmd = "awk -F: \'{print $3,$1, $NF}\'  /etc/passwd  2>&1; echo $?";
    QString resStr =GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：获取当前用户名列表失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
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
        if(tmpl[2].contains("nologin")||tmpl[2].contains("false") ||usrinfo.uid.toInt()<1000)
            usrinfo.isShow = false;
        else
            usrinfo.isShow = true;
        if(usrinfo.isShow)
            users.append(usrinfo.uname);
    }
    return true;
}

bool SecFunClass::getUserTagInfoList(QList<UserTag> &reslist)
{
    reslist.clear();
    QStringList usernames;
    try
    {
        getUserNames(usernames);
    }catch(Exception exp)
            {
        throw exp;
    }

    for(int i=0; i<usernames.count();i++)
    {
        UserTag userTag;
        userTag.username = usernames[i];
        reslist.append(userTag);
    }


    //获取安全性标签
    for(int i=0; i<reslist.count();i++)
    {
        char userlevel[LevelSize];
        char name[33];
        strcpy(name, reslist[i].username.toStdString().c_str());
        int errno = getlinuxuserlevel(name, userlevel);
        if(errno==0)
        {
             reslist[i].safeTag = userlevel;
        }else
            {
            QString errContent = QGlobalClass::getInstance()->errMap[errno+GetUserLevelErrBase];
            qDebug()<<errContent;
            throw Exception(QString::number(errno+GetUserLevelErrBase), errContent);
        }
    }

    //获取完整性标签

//    QString cmd = "semanage user -l 2>&1; echo $?";
//    QString resStr = GetCmdRes(cmd).trimmed();
//    QStringList strl = resStr.split('\n');
//    if(strl.last().toInt()!=0)
//    {
//        resStr.chop(strl.last().length());
//        QString errContent=tr("执行操作：获取所有用户安全标签失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
//        qDebug()<<errContent;
//        throw Exception(strl.last(), errContent);
//    }

//    if(strl.count()<4)
//    {
//        QString errContent=tr("执行操作：获取所有用户安全标签失败")+ tr("\n错误内容：解析结果失败");
//        qDebug()<<errContent;
//        throw Exception("", errContent);
//    }

////    strl.removeFirst();
//    strl.removeFirst();
//    strl.removeFirst();
//    strl.removeFirst();     //去掉前四行的标题栏
//    strl.removeLast();      //去掉echo $?

//    for(int i=0; i<strl.count(); i++)
//    {
//        QStringList tmpl = strl[i].simplified().split(' ');
//        if(tmpl.count()<5)
//        {
//            QString errContent=tr("执行操作：获取所有用户安全标签失败")+ tr("\n错误内容：解析单条结果失败");
//            qDebug()<<errContent;
//            throw Exception("", errContent);
//        }

//        UserTag usrinfo;
//        usrinfo.username = tmpl[0];
//        usrinfo.safeTag = tmpl[2];
//        usrinfo.wholeTag = tmpl[2];
//        reslist.append(usrinfo);
//    }

    return true;
}

bool SecFunClass::setUserTagInfo(UserTag usrtag, int opt)          //设置用户安全标签. opt=0添加用户，opt=1编辑用户
{

//    QString cmd = "semanage user ";
//    if(opt==0)
//    {
//        cmd += "-a "+usrtag.username+" -L "+usrtag.safeTag+" -R user_r -r s0-s0  2>&1; echo $?";
//    }else if(opt==1)
//        {
//        cmd +=  "-m "+usrtag.username+" -L "+usrtag.safeTag+" -R user_r -r s0-s0 2>&1; echo $?";
//    }

//    QString resStr = GetCmdRes(cmd).trimmed();
//    QStringList strl = resStr.split('\n');
//    if(strl.last().toInt()!=0)
//    {
//        resStr.chop(strl.last().length());
//        QString errContent=tr("执行操作：设置用户安全标签失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
//        qDebug()<<errContent;
//        throw Exception(strl.last(), errContent);
//    }
    if(usrtag.safeTag!=tr("不设置"))
        {
        char user[33];
        char level[LevelSize];
        strcpy(user, usrtag.username.toStdString().c_str());
        strcpy(level, usrtag.safeTag.toStdString().c_str());

        int errno = setlinuxuserlevel(user, level);
        if(errno!=0)
        {
            QString errContent = QGlobalClass::getInstance()->errMap[errno+SetUserLevelErrBase];
            qDebug()<<errContent;
            throw Exception(QString::number(errno+SetUserLevelErrBase), errContent);
            return false;
        }
    }

    //设置完整性标签
//    if(usrtag.wholeTag!=tr("不设置"))
//        {

//    }


}

void SecFunClass::setUserTagInfoSlot(UserTag usrtag, int opt)
{
    try
    {
        setUserTagInfo(usrtag, opt);
        emit emitSetUserTagInfoDone(0, Exception("",""));
    }catch(Exception exp)
    {
        emit emitSetUserTagInfoDone(1, exp);
    }
}

bool SecFunClass::setFileTagInfo(FileTag filetag)
{
//    QString cmd ="chcon -l " + filetag.safeTag + " \""+ filetag.filename+ "\" 2>&1;echo $?";
//    QString resStr = GetCmdRes(cmd).trimmed();
//    QStringList strl = resStr.split('\n');
//    if(strl.last().toInt()!=0)
//    {
//        resStr.chop(strl.last().length());
//        QString errContent=tr("执行操作：设置文件安全标签失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
//        qDebug()<<errContent;
//        throw Exception(strl.last(), errContent);
//    }
    if(filetag.safeTag!=tr("不设置"))
    {
        char filename[MaxPath];
        char safetag[LevelSize];
        strcpy(filename, filetag.filename.toStdString().c_str());
        strcpy(safetag, filetag.safeTag.toStdString().c_str());
        int errno = setfilelevel(filename, safetag);
        if(errno!=0)
            {
            QString errContent = QGlobalClass::getInstance()->errMap[errno+SetFileLevelErrBase];
            qDebug()<<errContent;
            throw Exception(QString::number(errno+SetFileLevelErrBase), errContent);
        }
    }

    //完整性标签设置

    return true;
}

void SecFunClass::resetPINOfUkeySlot(UkeyInfo ukeyInfo)
{
    try
    {
        resetPINOfUkey(ukeyInfo);
        emit emitResetPINOfUkeyDone(0, Exception("",""));
    }catch(Exception exp)
    {
        emit emitResetPINOfUkeyDone(1, exp);
    }
}

void SecFunClass::setUserOfUkeySlot(UkeyInfo ukeyInfo)
{
    try
    {
        setUserOfUkey(ukeyInfo);
        emit emitSetUserOfUkeyDone(0, Exception("",""));
    }catch(Exception exp)
    {
        emit emitSetUserOfUkeyDone(1, exp);
    }
}

bool SecFunClass::getFileTagInfo(FileTag &filetag)                     //获取文件安全标签
{
    //safeTag
    char  filename[MaxPath];
    char fileLevel[LevelSize];
    strcpy(filename, filetag.filename.toStdString().c_str());

    int errno = getfilelevel(filename, fileLevel);
    if(errno==0)
        {
        filetag.safeTag = fileLevel;
    }else
        {
        QString errContent = QGlobalClass::getInstance()->errMap[errno+GetFileLevelErrBase];
        qDebug()<<errContent;
        throw Exception(QString::number(errno+GetFileLevelErrBase), errContent);

    }

    //wholeTag

    return true;
//    QString cmd = "ls ";
//    if(filetag.isDir)
//        cmd += "-d --scontext \""+filetag.filename+"\" 2>&1; echo $?";
//    else
//        cmd += " --scontext \""+filetag.filename+"\" 2>&1; echo $?";

//    QString resStr = GetCmdRes(cmd).trimmed();
//    QStringList strl = resStr.split('\n');
//    if(strl.last().toInt()!=0)
//    {
//        resStr.chop(strl.last().length());
//        QString errContent=tr("执行操作：获取文件安全标签失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
//        qDebug()<<errContent;
//        throw Exception(strl.last(), errContent);
//    }
//    strl.removeLast();  //去掉echo $?
//    if(strl.count()!=1)
//        {
//        QString errContent=tr("执行操作：获取文件安全标签失败")+ tr("\n错误内容：解析结果失败");
//        qDebug()<<errContent;
//        throw Exception("", errContent);
//    }
//    QStringList tmpl = strl[0].simplified().split(' ');
//    if(tmpl.count()<2)
//        {
//        QString errContent=tr("执行操作：获取文件安全标签失败")+ tr("\n错误内容：解析结果失败");
//        qDebug()<<errContent;
//        throw Exception("", errContent);
//    }

//    QStringList tmpl2 = tmpl[0].split(':');
//    if(tmpl2.count()!=4)
//    {
//        QString errContent=tr("执行操作：获取文件安全标签失败")+ tr("\n错误内容：解析结果失败");
//        qDebug()<<errContent;
//        throw Exception("", errContent);
//    }

//    filetag.safeTag = tmpl2[3];
//    filetag.wholeTag = tmpl2[3];

//    return true;
}

bool SecFunClass::getUserListOfShaddow(QList<SecUserInfo> &secUserList)
{
    QString cmd = "awk -F: \'{print $2}\' /etc/shadow  2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList list = resStr.split('\n');
    if(list.last().toInt()!=0)
    {
        resStr.chop(list.last().length());
        QString errContent=tr("执行操作：获取安全管理用户列表失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+list.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(list.last(), errContent);
        return false;
    }
    list.removeLast();
    if(list.length()!=secUserList.length())
    {
        QString errContent=tr("执行操作：获取安全管理用户列表失败")+ tr("\n执行命令：")+cmd+tr("\n错误内容：/etc/shaddow中用户与/etc/passwd不一致");
        qDebug()<<errContent;
        throw Exception(list.last(), errContent);
        return false;
    }
    for(int i=0; i<secUserList.length();i++)
    {
        if(list[i] == "!!!" || secUserList[i].uId.toInt()<1000)
            secUserList[i].bShow = false;
        else
            secUserList[i].bShow = true;
    }

    return true;
}

bool SecFunClass::getSecUserList(QList<SecUserInfo> &secUserList)       //获取当前系统用户的安全管理信息
{
    QString cmd = "awk -F: \'{print  $3,$1, $NF}\'  /etc/passwd  2>&1; echo $?";
    QString resStr =GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    secUserList.clear();
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：获取安全管理用户列表失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    strl.removeLast();
    for(int i=0; i<strl.length(); i++)
    {
        SecUserInfo usrinfo;
        QStringList tmpl = strl[i].split(' ');
        usrinfo.uId = tmpl[0].trimmed();
        usrinfo.uName = tmpl[1].trimmed();
        if(tmpl[2].contains("nologin")||tmpl[2].contains("false") ||usrinfo.uId.toInt()<1000)
            usrinfo.bShow = false;
        else
            usrinfo.bShow = true;
        if(usrinfo.bShow)
            secUserList.append(usrinfo);
    }

    //验证在/etc/shadow中的情况，判断是否是曾经存在的用户
//    try
//    {
//        getUserListOfShaddow(secUserList);
//    }catch(Exception exp)
//    {
//        secUserList.clear();
//        throw exp;
//    }

    //获取是否被锁定
    try
    {
        QStringList users;
        getLockedUsers(users);
        for(int i=0; i<secUserList.count(); i++)
        {
            if(users.contains(secUserList[i].uName))
            {
                secUserList[i].bLocked = true;
            }else
                secUserList[i].bLocked = false;
        }

    }catch(Exception exp)
    {
        throw exp;
    }

    //验证UKey
    try
    {
        for(int i=0; i<secUserList.count(); i++)
        {
            if(secUserList[i].bShow)
                getUserUkey(secUserList[i]);
        }

    }catch(Exception exp)
    {
       throw exp;
    }

    return true;
}

bool SecFunClass::getUserUkey(SecUserInfo &secUser)
{
    QString cmd = "nfsukey -c "+secUser.uName+" 2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：获取用户的Ukey绑定信息失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    if(resStr.contains(" not "))
        secUser.bBindKey = false;
    else
        secUser.bBindKey = true;
    return true;
}

RUNSTATE SecFunClass::servRunState(QString svName)
{
    QString cmd= "systemctl status "+ svName + " 2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
  //  QStringList strl = resStr.split('\n');
//    if(strl.last().toInt()!=0)
//    {
//        resStr.chop(strl.last().length());
//        QString errContent =tr("执行操作：获取服务运行状态失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
//        qDebug()<<errContent;
//        throw Exception(strl.last(), errContent);
//    }
    if(resStr.contains("Active: active (running)"))
    {
        return RUNNING;
    }else if(resStr.contains("Active: inactive"))
        return DEAD;
    else if(resStr.contains("Active: active (exited)"))
        return EXIT;
    else
        return OTHER;
}

bool SecFunClass::resetPINOfUkey(UkeyInfo ukeyInfo)
{
    QString cmd = "nfsukey "+ ukeyInfo.cur_pin+ " -s "+ukeyInfo.new_pin+" 2>&1;echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    qDebug()<<cmd <<"\n"<<strl.last();
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：重置Ukey的PIN失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    return true;
}

bool SecFunClass::setUserOfUkey(UkeyInfo ukif)
{
    QString cmd = "nfsukey "+ ukif.cur_pin;
    QString opt = "";
    switch(ukif.type)
    {
    case BUND_User:
        cmd += " -b "+ukif.user+" 2>&1;echo $?";
        opt=tr("执行操作：绑定Ukey失败");
        break;
    case UnBUND_User:
        cmd += " -d "+ukif.user+" 2>&1;echo $?";
        opt=tr("执行操作：解绑Ukey失败");
        break;
    case Clean_Ukey:
        cmd += " -e  2>&1;echo $?";
        opt=tr("执行操作：清除Ukey证书失败");
        break;
    }

    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    qDebug()<<cmd <<"\n"<<strl.last();

    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=opt+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    return true;
}

bool SecFunClass::getTeRules(QList<TERule> &telist)                         //获取te策略
{
    telist.clear();
    QString cmd = "sesearch --allow 2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
  //  qDebug()<<cmd <<"\n"<<strl.last();
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：获取TE安全策略失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }

    strl.removeLast();      //去掉echo $?
    strl.removeLast();      //去掉最后一行空行
    strl.removeFirst();     //去掉第一行

    QRegExp regexp = QRegExp("\\s+");
    for(int i=0; i<strl.count(); i++)
    {
        TERule terule;
        QStringList tmpl = strl[i].trimmed().split(regexp);
        int begindex = strl[i].indexOf('{'), endindex = strl[i].indexOf('}');
        if(begindex!=-1 &&  endindex!=-1)
            terule.permmisions = strl[i].mid(begindex+1, endindex-begindex-1 ).trimmed();
        else
            terule.permmisions =  tmpl[5];
        terule.domain_type = tmpl[1];
        terule.file_type = tmpl[2];
        terule.class_type = tmpl[4];
        telist.append(terule);
    }

    return true;
}

bool SecFunClass::getFileProcessRules(QList<FileProConV> &fpconvs)
{
    fpconvs.clear();
    QString cmd = "sesearch --type 2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：获取文件/进程安全策略失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }

    strl.removeLast();      //去掉echo $?
    strl.removeLast();      //去掉最后一行空行
    strl.removeFirst();     //去掉第一行

    QRegExp regexp = QRegExp("\\s+");
    bool filename_trans= false;
    for(int i=0; i<strl.length(); i++)
    {
        FileProConV fpconv;
        strl[i] = strl[i].simplified();
        if(strl[i].length()!=0)
        {
            if(!filename_trans)
            {
                if(strl[i].right(1)==";")
                    strl[i] = strl[i].left(strl[i].length()-1);
                QStringList tmpl = strl[i].trimmed().split(regexp);
                if(tmpl.length()<6)
                    {
                    QString errContent=tr("执行操作：获取文件/进程安全策略失败")+tr("\n错误内容：");
                    qDebug()<<errContent;
                    throw Exception(strl.last(), errContent);
                }
                fpconv.src_type = tmpl[1];
                fpconv.targ_type = tmpl[2];
                fpconv.class_type = tmpl[4];
                fpconv.default_type = tmpl[5];
                fpconvs.append(fpconv);
            }else
            {
                if(strl[i].right(1)==";")
                    strl[i] = strl[i].left(strl[i].length()-1);
                QStringList tmpl = strl[i].trimmed().split(regexp);
                fpconv.src_type = tmpl[1];
                fpconv.targ_type = tmpl[2];
                fpconv.class_type = tmpl[4];
                fpconv.default_type = tmpl[5]+" "+tmpl[6];
                fpconvs.append(fpconv);
            }

        }else
        {
            i++;    //遇到空白行，越过下一行
            filename_trans = true;
        }

    }
    return true;
}

void SecFunClass::getSafePolicySlot(TELIST teList,F_PLIST fpList)
{
    try
    {
        getTeRules(teList);
        getFileProcessRules(fpList);
        emit emitGetSafePolicyDone(0, Exception("",""),teList, fpList);
    }catch(Exception exp)
    {
        emit emitGetSafePolicyDone(1, exp, teList, fpList);
    }
}

bool SecFunClass::GetSakInfo(SakInfo &sakinfo)                 //获取sak开关信息
{
    QString cmd = "nfs-getsak 2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：获取SAK信息失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }

    strl.removeLast();      //去掉echo $?
    for(int i=0; i<strl.count(); i++)
    {
        strl[i] = strl[i].simplified();
        QStringList tmpl = strl[i].split(' ');
        if(tmpl.count()!=4)
        {
            QString errContent=tr("执行操作：获取SAK信息失败")+ tr("\n执行命令：")+cmd+tr("\n错误内容：分析结果失败");
            qDebug()<<errContent;
            throw Exception(strl.last(), errContent);
        }
        if(strl[i].contains("CURRENT:"))
            sakinfo.current_mode = tmpl.last();
        if(strl[i].contains("DEFAULT:"))
            sakinfo.default_mode = tmpl.last();
    }

    return true;
}

bool SecFunClass::SetSakInfo(QString sta)
{
    QString cmd = "nfs-setsak "+sta+" 2>&1; echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：设置SAK信息失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }

    return true;
}

bool SecFunClass::SetDefaultSakInfo(QString sta)                           //设置sak
{
    QString cmd = "nfs-setsak default_"+sta+" 2>&1;echo $?";
    QString resStr = GetCmdRes(cmd).trimmed();
    QStringList strl = resStr.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：设置SAK Default信息失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }

    return true;
}

bool SecFunClass::setEnforce(bool bOpen)
{
    QString cmd = QString("setenforce ") + (bOpen?"1":"0" )+ " 2>&1; echo $?";
    QString resStr= GetCmdRes(cmd).trimmed();

    QStringList strl = cmd.split('\n');
    if(strl.last().toInt()!=0)
    {
        resStr.chop(strl.last().length());
        QString errContent=tr("执行操作：开启/关闭安全策略失败")+ tr("\n执行命令：")+cmd+tr("\n错误码：")+strl.last()+tr("\n错误内容：")+resStr;
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }

    return true;
}

bool SecFunClass::startOrStopService(QString svName, int opt)      //开启或关闭服务
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
