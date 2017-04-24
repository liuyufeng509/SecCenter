#include "secfunclass.h"

SecFunClass::SecFunClass(QObject *parent) : QObject(parent)
{

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
        throw Exception(list.last(), errContent);
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
    for(int i=1; i<strl.count(); i++)
    {
        QStringList tmpl = strl[i].simplified().split(' ');
        list<<tmpl[0];
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
        throw Exception(strl.last(), errContent);
    }

    strl.removeLast();      //去掉echo $?结果
    QStringList tmpl = strl[0].simplified().split(' ');
    if(tmpl.count()!=3)
        {
        QString errContent=tr("执行操作：获取当前安全状态失败")+tr("\n错误内容：selinux status解析失败");
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    status.selinux_status = tmpl.last();

    tmpl = strl[1].simplified().split(' ');
    if(tmpl.count()!=3)
        {
        QString errContent=tr("执行操作：获取当前安全状态失败")+tr("\n错误内容：SELinuxfs mount解析失败");
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    status.selinux_fs_mount = tmpl.last();

    tmpl = strl[2].simplified().split(' ');
    if(tmpl.count()!=4)
        {
        QString errContent=tr("执行操作：获取当前安全状态失败")+tr("\n错误内容：SELinux root directory解析失败");
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    status.selinux_root_dir =  tmpl.last();

    tmpl = strl[3].simplified().split(' ');
    if(tmpl.count()!=4)
        {
        QString errContent=tr("执行操作：获取当前安全状态失败")+tr("\n错误内容：Loaded policy name解析失败");
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    status.load_policy_name =  tmpl.last();

    tmpl = strl[4].simplified().split(' ');
    if(tmpl.count()!=3)
        {
        QString errContent=tr("执行操作：获取当前安全状态失败")+tr("\n错误内容：Current mode解析失败");
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    status.curr_mode =  tmpl.last();

    tmpl = strl[5].simplified().split(' ');
    if(tmpl.count()!=5)
        {
        QString errContent=tr("执行操作：获取当前安全状态失败")+tr("\n错误内容：Mode from config file解析失败");
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    status.mode_frm_cfg =  tmpl.last();

    tmpl = strl[6].simplified().split(' ');
    if(tmpl.count()!=4)
        {
        QString errContent=tr("执行操作：获取当前安全状态失败")+tr("\n错误内容：Policy MLS status解析失败");
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    status.mls_status =  tmpl.last();

    tmpl = strl[7].simplified().split(' ');
    if(tmpl.count()!=4)
        {
        QString errContent=tr("执行操作：获取当前安全状态失败")+tr("\n错误内容：Policy deny_unknown status解析失败");
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    status.policy_deny_stat =  tmpl.last();

    tmpl = strl[8].simplified().split(' ');
    if(tmpl.count()!=5)
        {
        QString errContent=tr("执行操作：获取当前安全状态失败")+tr("\n错误内容：Max kernel policy version解析失败");
        qDebug()<<errContent;
        throw Exception(strl.last(), errContent);
    }
    status.max_kern_policy_version =  tmpl.last();

    return true;
}
