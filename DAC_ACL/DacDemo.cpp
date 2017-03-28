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
#include "DacDemo.h"



int checkFileProperties(char* FileName,struct stat *buf,int *error)
{
	if(FileName == NULL)
	{
		return -1;
	}

	int res = 0;
	res = stat(FileName,buf);
	if(res == -1)
	{
		*error = errno;
		char *mes = strerror(errno);
		printf("%s\n",mes);
	}
	return res;
}



int getFileUserAndPerssion(char * ErrorMes,char *FileName,int *SPermission,char *UserName,int *UserPerssion,
		char *GroupName, int *GrpPerssion,int *OtherPerssion)
{
	if(FileName == NULL)
	{
		return -1;
	}
	int res = 0,MyErrorno;
	struct stat FileStat;
    res = checkFileProperties(FileName,&FileStat,&MyErrorno);
	if(res == -1)
	{
		strcpy(ErrorMes,strerror(MyErrorno));
		return res;
	}
	struct passwd *FilePasswd;
	FilePasswd = getpwuid(FileStat.st_uid);
	//printf("%s\n",MyPasswd->pw_name);
	strcpy(UserName,FilePasswd->pw_name);
	struct group *FileGroup;
	FileGroup = getgrgid(FilePasswd->pw_gid);
	strcpy(GroupName,FileGroup->gr_name);
    int p_r = 0,p_w = 0,p_x = 0/*,p_s = 0*/;
	printf("%x,ssss,%x\n",FileStat.st_mode&S_IRUSR,S_IRUSR);

	p_r = (FileStat.st_mode & S_ISUID) == S_ISUID ?  4 : 0;
	p_w = (FileStat.st_mode & S_ISGID) == S_ISGID ?  2 : 0;
	p_x = (FileStat.st_mode & S_ISVTX) == S_ISVTX ?  1 : 0;
	*SPermission =  p_r + p_w + p_x;
	p_r = (FileStat.st_mode & S_IRUSR) == S_IRUSR ?  4 : 0;
	p_w = (FileStat.st_mode & S_IWUSR) == S_IWUSR ?  2 : 0;
	p_x = (FileStat.st_mode & S_IXUSR) == S_IXUSR ?  1 : 0;
	*UserPerssion =  p_r + p_w + p_x;
	p_r = (FileStat.st_mode & S_IRGRP) == S_IRGRP ?  4 : 0;
	p_w = (FileStat.st_mode & S_IWGRP) == S_IWGRP ?  2 : 0;
	p_x = (FileStat.st_mode & S_IXGRP) == S_IXGRP ?  1 : 0;
	*GrpPerssion =  p_r + p_w + p_x;
	p_r = (FileStat.st_mode & S_IROTH) == S_IROTH ?  4 : 0;
	p_w = (FileStat.st_mode & S_IWOTH) == S_IWOTH ?  2 : 0;
	p_x = (FileStat.st_mode & S_IXOTH) == S_IXOTH ?  1 : 0;
	*OtherPerssion =  p_r + p_w + p_x;
	return res;
}

int setFilePerssion(char* path,mode_t mode,char *ErrorMes)
{
	if(path == NULL||ErrorMes == NULL)
	{
		return -1;
	}
	int res = chmod(path,mode);
	if(res == -1)
	{
		char *mes = strerror(errno);
		strcpy(ErrorMes,mes);
	}
	return res;
}

int getFileAcl(char *FileName,char *Message,int MessageLong)
{
	if(FileName == NULL||Message == NULL)
	{
		return -1;
	}
	char cmd[512] = "getfacl ";
	strcat(cmd,FileName);
	FILE *file = popen(cmd,"r");
	if(file == NULL)
	{
		strcpy(Message,"operation failed");
		return -1;
	}
	char buffer[MessageLong];
	memset(buffer,'\0',MessageLong);
	fread(buffer,1,MessageLong,file);
	fclose(file);
	//printf("%s\n",buffer);
	strcpy(Message,buffer);
	return 0;
}

//int SetFileAcl(char* opition,char *name,char *perssion,char *file,char* Message)
int setFileAcl(char* cmd,char* Message)
{
	/*
	char cmd[512] = "setfacl -"
	strcat(cmd,name);
	strcat(cmd," \"")
*/
	if(cmd == NULL||Message == NULL)
	{
		return -1;
	}
	FILE *file = popen(cmd,"r");
	if(file == NULL)
	{
		strcpy(Message,"operation failed");
		return -1;
	}
	char buffer[512]= {};
	fread(buffer,1,512,file);
	fclose(file);
	//printf("%s\n",buffer);
	strcpy(Message,buffer);
	//printf("%s\n",Message);

	return 0;
}



