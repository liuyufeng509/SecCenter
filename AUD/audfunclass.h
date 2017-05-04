#ifndef AUDFUNCLASS_H
#define AUDFUNCLASS_H

#include <QObject>
#include "common.h"
class AudFunClass : public QObject
{
    Q_OBJECT
public:
    explicit AudFunClass(QObject *parent = 0);

    //审计规则相关操作
    bool getKernAudParam(KernAudParam &param);     //获取内核审计参数

    bool excuteAudCmd(QString cmd, QString optType, QString &res);     //执行审计命令

    bool startOrStopService(QString svName, int opt);       //开启或关闭服务
signals:

public slots:
};

#endif // AUDFUNCLASS_H
