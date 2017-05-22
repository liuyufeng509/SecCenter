#ifndef AUDKERNRULE_H
#define AUDKERNRULE_H

#include <QWidget>
#include "audfunclass.h"
namespace Ui {
class AudKernRulePage;
}

class AudKernRulePage : public QWidget
{
    Q_OBJECT

public:
    explicit AudKernRulePage(QWidget *parent = 0);
    ~AudKernRulePage();
    void updateUI();
    void saveDataFromUI();
private slots:
    void on_aplButton_clicked();

    void on_forEverButton_clicked();

private:
    Ui::AudKernRulePage *ui;
     KernAudParam kernAudParam;
};

#endif // AUDKERNRULE_H
