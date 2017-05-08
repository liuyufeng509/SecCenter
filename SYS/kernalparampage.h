#ifndef KERNALPARAMPAGE_H
#define KERNALPARAMPAGE_H

#include <QWidget>

namespace Ui {
class KernalParamPage;
}

class KernalParamPage : public QWidget
{
    Q_OBJECT

public:
    explicit KernalParamPage(QWidget *parent = 0);
    ~KernalParamPage();

private:
    Ui::KernalParamPage *ui;
};

#endif // KERNALPARAMPAGE_H
