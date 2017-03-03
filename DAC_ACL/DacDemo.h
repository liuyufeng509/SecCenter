/*************************************************************************
	> File Name: DacDemo.c
	> Author: zhanghao
	> Mail: zhanghao@nfs-qd.com 
	> Created Time: 2017年02月20日 星期一 10时04分42秒
 ************************************************************************/
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <fcntl.h>
#include <sys/types.h>
extern "C"
{
/*
 *函数名称： 
 *函数功能：
 *输入参数：
 *函数返回：
 * */
int CheckFileProperties(char* FileName,struct stat *buf,int *error);

/*
 *函数名称：GetFileUserAndPerssion 
 *函数功能：获得文件的属主、组以及读写执行权限
 *输入参数：ErrorMes 用于返回错误信息，FileName 所要查看的文件名称，SPermission 用于返回suid，sgid和ticket位
	    UserName用于返回文件属主名称，UserPerssion 属主权限，GroupName组名称，GrpPerssion 组权限，
	    Othererssion其他权限
 *函数返回：正确返回0错误返回-1
 * */
int GetFileUserAndPerssion(char * ErrorMes,char *FileName,int *SPermission,char *UserName,int *UserPerssion,
		char *GroupName, int *GrpPerssion,int *OtherPerssion);

/*
 *函数名称： SetFilePerssion
 *函数功能：设置文件权限
 *输入参数：path文见名称，mode 文件权限码，ErrorMes 错误信息
 *函数返回：正确返回0错误返回-1
 * */
int SetFilePerssion(char* path,mode_t mode,char *ErrorMes);


/*
 *函数名称：GetFileAcl 
 *函数功能：获取文件acl信息
 *输入参数：FileName 文件名称，Message文件acl信息，MessageLong 信息长度 
 *函数返回：正确返回0，错误返回-1
 * */
int GetFileAcl(char *FileName,char *Message,int MessageLong);


//int SetFileAcl(char* opition,char *name,char *perssion,char *file,char* Message)
/*
 *函数名称： SetFileAcl
 *函数功能：设置文件acl权限
 *输入参数：cmd 设置命令，message 返回信息
 *函数返回：正确返回0，错误返回-1
 * */
int SetFileAcl(char* cmd,char* Message);
}


