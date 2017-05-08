#ifndef SECRULESPAGE_H
#define SECRULESPAGE_H

#include <QWidget>

namespace Ui {
class SecRulesPage;
}

class SecRulesPage : public QWidget
{
    Q_OBJECT

public:
    explicit SecRulesPage(QWidget *parent = 0);
    ~SecRulesPage();

private:
    Ui::SecRulesPage *ui;
};

#endif // SECRULESPAGE_H
