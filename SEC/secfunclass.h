#ifndef SECFUNCLASS_H
#define SECFUNCLASS_H

#include <QObject>
#include "common.h"

class SecFunClass : public QObject
{
    Q_OBJECT
public:
    explicit SecFunClass(QObject *parent = 0);

    //用户安全管理相关操作
    bool getLockServices(QStringList &list);        //获取可被锁定的服务列表
    bool getLockedUsers(QStringList &list);         //获取当前被锁定的用户

    bool tryLockOption(TryLockInfo info);           //设置锁定规则
    bool getCurPwdInfo(PwdInfo &pwdInfo);        //获取当前系统的密码设置规则

    //安全状态查看功能
    bool getSecStatus(SecStatus &status);   //获取当前的安全状态


signals:

public slots:
};

#endif // SECFUNCLASS_H
