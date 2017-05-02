#ifndef INDEXFUNCLASS_H
#define INDEXFUNCLASS_H

#include <QObject>
#include "common.h"

class IndexFunClass : public QObject
{
    Q_OBJECT
public:
    explicit IndexFunClass(QObject *parent = 0);
    bool getCurrentUserName(QString &name);
    bool getEnforce(QString &enforce);
signals:

public slots:
};

#endif // INDEXFUNCLASS_H
