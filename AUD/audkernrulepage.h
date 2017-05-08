#ifndef AUDKERNRULE_H
#define AUDKERNRULE_H

#include <QWidget>

namespace Ui {
class AudKernRulePage;
}

class AudKernRulePage : public QWidget
{
    Q_OBJECT

public:
    explicit AudKernRulePage(QWidget *parent = 0);
    ~AudKernRulePage();

private:
    Ui::AudKernRulePage *ui;
};

#endif // AUDKERNRULE_H
