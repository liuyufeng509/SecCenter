#ifndef SECFUNCLASS_H
#define SECFUNCLASS_H

#include <QObject>
#include "common.h"
#include <QDate>


class SecFunClass : public QObject
{
    Q_OBJECT
public:

    static SecFunClass*  getInstance()
       {
           if(m_pInstance == NULL)
           {
               m_pInstance = new SecFunClass();
           }
           return m_pInstance;
       }
    //用户安全管理相关操作
    bool getLockServices(QStringList &list);        //获取可被锁定的服务列表
    bool getLockedUsers(QStringList &list);         //获取当前被锁定的用户
    bool unLockUser(QString uName);                 //解锁被锁定的用户

    bool tryLockOption(TryLockInfo info);           //设置锁定规则
    bool getCurPwdInfo(PwdInfo &pwdInfo);        //获取当前系统的密码设置规则
    bool setCurPwdInfo(PwdInfo &pwdInfo);
    bool getSecUserList(QList<SecUserInfo> &secUserList);       //获取当前系统用户的安全管理信息
    bool getCurLockInfo(TryLockInfo &info);                     //获取当前的用户锁定规则

    //Ukey 功能操作
    bool resetPINOfUkey(UkeyInfo ukeyInfo);     //更改UKey的PIN
    bool setUserOfUkey(UkeyInfo ukeyInfo);     //设置Ukey绑定或解绑

    //安全状态查看功能
    bool getSecStatus(SecStatus &status);   //获取当前的安全状态

    //用户安全标签管理
    bool getUserTagInfoList(QList<UserTag> &reslist);   //获取所有用户的安全管理标签
    bool getUserSafeTagInfo(UserTag &userTag);   //获取用户的安全标签
    bool getUserWholeTagInfo(UserTag &userTag); //获取用户完整性标签
    bool setUserTagInfo(UserTag usrtag, int opt);           //设置用户安全标签. opt=0添加用户，opt=1编辑用户

    //文件安全标签管理
    bool setFileTagInfo(FileTag filetag);                      //设置文件安全性标签
   // bool setFileWholeTagInfo(FileTag &filetag);
    bool setFileSafeTagInfo(FileTag &filetag);
    bool getFileTagInfo(FileTag &filetag);                      //获取文件安全标签
    bool getFileSafeTagInfo(FileTag &filetag);
    bool getFileWholeTagInfo(FileTag &filetag);
    bool getUserNames(QList<UserTag> &reslist);              //获取所有的用户名

    //安全策略查看功能
    bool getTeRules(QList<TERule > &telist);                          //获取te策略
    bool getFileProcessRules(QList<FileProConV> &fpconvs);  //获取文件进程策略

    //开关管理相关
    RUNSTATE servRunState(QString svName);      //获取服务运行状态
    bool GetSakInfo(SakInfo &sakinfo);                 //获取sak开关信息
    bool SetSakInfo(QString sta);                           //设置sak
    bool SetDefaultSakInfo(QString sta);                           //设置sak
    bool setEnforce(bool bOpen);                        //开启关闭安全策略
    bool startOrStopService(QString svName, int opt);       //开启或关闭服务
    bool isRmOpened(bool &isOpen);              //客体重用是否启用
    bool setRmOpened(bool isOpen);

    bool isBiBaOpened(bool &isOpen);
    bool setBiBaOpen(bool isOpen);
private:
    explicit SecFunClass(QObject *parent = 0);
    static SecFunClass *m_pInstance;
signals:
    void emitSetUserTagInfoDone(int res, Exception);   //用户安全标签管理多线程操作,设置完毕，返回结果
    void emitGetSafePolicyDone(int res, Exception exp, TELIST telist, F_PLIST fpList);      //获取安全策略结束

    void emitGetUserTagInfoListDone(int,Exception, USERTAGLIST);

    //UKey
    void emitResetPINOfUkeyDone(int res, Exception);
    void emitSetUserOfUkeyDone(int res, Exception);

public slots:
    //用户安全标签设置多线程操作
    void setUserTagInfoSlot(UserTag usrtag, int opt);
    void getSafePolicySlot(TELIST teList,F_PLIST fpList);
    void resetPINOfUkeySlot(UkeyInfo ukeyInfo);
    void setUserOfUkeySlot(UkeyInfo ukeyInfo);

    void getUserTagInfoListSlot();
private:
    bool getUserListOfShaddow(QList<SecUserInfo> &secUserList); //判断/etc/passwd，用户是否已经被创建过
    bool getUserUkey(SecUserInfo &secUser);                         //获取绑定的Ukey
    bool getCurLockInfoOfLogin(TryLockInfo &info);                  //获取login的锁定规则
    bool getCurLockInfoOfSsh(TryLockInfo &info);                    //获取ssh的锁定规则
    bool getCurLockInfoOfGdm(TryLockInfo &info);                    //获取gdm的锁定规则
};

#endif // SECFUNCLASS_H
