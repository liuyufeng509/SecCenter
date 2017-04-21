#ifndef KERNALPARAMMANAGER_H
#define KERNALPARAMMANAGER_H

#include <QWidget>
#include "common.h"
#include "sysfunclass.h"
namespace Ui {
class KernalParamManager;
}

class KernParmMngWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KernParmMngWidget(QWidget *parent = 0);
    ~KernParmMngWidget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::KernalParamManager *ui;
    SysFunClass m_sysFunModel;
};

#endif // KERNALPARAMMANAGER_H
