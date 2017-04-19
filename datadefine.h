#ifndef DATADEFINE_H
#define DATADEFINE_H
#include<QString>
#include <QStringList>
//Exception 类，用于调用命令时抛出异常。
class Exception
{
public:
    Exception()
    {

    }

	Exception(QString errNo, QString errWhat)
	{
		m_errNo = errNo;
		m_errWhat=errWhat;		
	}
	QString getErroNo(){return m_errNo;}
	QString getErroWhat(){return m_errWhat;}
private:
	QString m_errNo;
	QString m_errWhat;
};

/*****************************************************
 *主界面逻辑处理所需类型定义
 *
*****************************************************/
enum ROLE{
    ROOT,
	SECADMIN,
	SYSADMIN,
    AUDIADMIN,
	ERROR,
};

/*******************************************************
 * 系统管理界面逻辑处理所需类型定义
 * ****************************************************/
//用户信息
struct UserInfo
{
	QString uid;
	QString uname;
	QString group;
    QStringList ogroups;
	bool isShow;
};
//用户操作类型
enum UserOpt
{
    Add,
    Edt,
};

//服务信息
#endif
