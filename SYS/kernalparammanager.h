#ifndef KERNALPARAMMANAGER_H
#define KERNALPARAMMANAGER_H

#include <QWidget>

namespace Ui {
class KernalParamManager;
}

class KernParmMngWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KernParmMngWidget(QWidget *parent = 0);
    ~KernParmMngWidget();

private:
    Ui::KernalParamManager *ui;
};

#endif // KERNALPARAMMANAGER_H
