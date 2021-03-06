#ifndef HELPPAGE_H
#define HELPPAGE_H

#include <QWidget>
#include<QTextBrowser>
#include "common.h"
namespace Ui {
class HelpPage;
}

class HelpPage : public QWidget
{
    Q_OBJECT

public:
    explicit HelpPage(ROLE curRole, QWidget *parent = 0);
    ~HelpPage();

private:
    Ui::HelpPage *ui;
    ROLE curRole;
};

#endif // HELPPAGE_H
