#ifndef AUDRULEPAGE_H
#define AUDRULEPAGE_H

#include <QWidget>

namespace Ui {
class AudRulePage;
}

class AudRulePage : public QWidget
{
    Q_OBJECT

public:
    explicit AudRulePage(QWidget *parent = 0);
    ~AudRulePage();

private:
    Ui::AudRulePage *ui;
};

#endif // AUDRULEPAGE_H
