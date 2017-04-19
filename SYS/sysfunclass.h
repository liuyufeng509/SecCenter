#ifndef SYSFUNCLASS_H
#define SYSFUNCLASS_H

#include <QObject>
#include "common.h"
class SysFunClass : public QObject
{
    Q_OBJECT
public:
    explicit SysFunClass(QObject *parent = 0);

    bool getUserList(QList<UserInfo> &users);  //获取所有用户信息，保存到列表中
    bool isGroupExist(QString groupName);
    bool addUser(UserInfo userInfo);
    bool modifyUser(UserInfo userInfo);
    bool delUser(UserInfo userInfo);

    //服务相关操作函数
    bool getServices(QList<ServiceInfo> &sevrs);
    bool setUpDownWhenBoot(QString svName, int opt);                   //设置开启启动  opt=0 开机启动，1关闭开机启动
signals:
    void emitGetServicesDone(int res, Exception exp, SEVLIST servs);     //线程获取服务列表 res=0代表成功，否则代表失败
    void emitSetUpDownWhenBootDone(int res, Exception);             //设置开机启动结束，res=0代表成功,否则代表失败
public slots:
    void getServicesSlot(SEVLIST sevrs);            //接收主线程请求，开始获取服务列表
    void setUpDownWhenBootSlot(QString svName, int opt);        //接收主线程请求，开始设置开机启动
private:
    bool getUserListOfShaddow(QList<UserInfo> &users); //判断/etc/passwd，用户是否已经被创建过
    bool getGroupsOfUserList(QList<UserInfo> &users); //获取所有用户的所属组信息
    bool getGroupList(QStringList &groups);     //获取所有用户组
    RUNSTATE servRunState(QString svName);

};

#endif // SYSFUNCLASS_H
