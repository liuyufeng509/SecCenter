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
    bool unLockUser(QString uName);                 //解锁被锁定的用户

    bool tryLockOption(TryLockInfo info);           //设置锁定规则
    bool getCurPwdInfo(PwdInfo &pwdInfo);        //获取当前系统的密码设置规则
    bool getSecUserList(QList<SecUserInfo> &secUserList);       //获取当前系统用户的安全管理信息

    //Ukey 功能操作
    bool resetPINOfUkey(UkeyInfo ukeyInfo);     //更改UKey的PIN

    //安全状态查看功能
    bool getSecStatus(SecStatus &status);   //获取当前的安全状态

    //用户安全标签管理
    bool getUserTagInfoList(QList<UserTag> &reslist);   //获取所有用户的安全管理标签
    bool setUserTagInfo(UserTag usrtag, int opt);           //设置用户安全标签. opt=0添加用户，opt=1编辑用户

    //文件安全标签管理
    bool setFileTagInfo(FileTag filetag);                      //设置文件安全性标签
    bool getFileTagInfo(FileTag &filetag);                      //获取文件安全标签

    //安全策略查看功能
    bool getTeRules(QList<TERule> &telist);                          //获取te策略
    bool getFileProcessRules(QList<FileProConV> &fpconvs);  //
signals:
    //用户安全标签管理多线程操作
    void emitSetUserTagInfoDone(int res, Exception);   //设置完毕，返回结果

public slots:
    //用户安全标签设置多线程操作
    void setUserTagInfoSlot(UserTag usrtag, int opt);


private:
    bool getUserListOfShaddow(QList<SecUserInfo> &secUserList); //判断/etc/passwd，用户是否已经被创建过
    bool getUserUkey(SecUserInfo &secUser);                         //获取绑定的Ukey
};

#endif // SECFUNCLASS_H