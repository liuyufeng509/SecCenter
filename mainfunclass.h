#ifndef MAINFUNCLASS_H
#define MAINFUNCLASS_H

#include <QObject>
#include"datadefine.h"
#include"common.h"
//处理主窗口启动时的业务逻辑处理，抛出异常，显示到提示框中
class MainFunClass : public QObject
{
    Q_OBJECT
public:
    explicit MainFunClass(QObject *parent = 0);
	
    ROLE getUserRole() /*throw()*/;//获取当前系统用户的角色，获取失败抛出异常，如果不在三个管理员内，返回ERROR。
signals:

public slots:
};

#endif // MAINFUNCLASS_H
