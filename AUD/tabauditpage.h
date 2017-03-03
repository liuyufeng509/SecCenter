#ifndef TABAUDITPAGE_H
#define TABAUDITPAGE_H

#include <QWidget>

namespace Ui {
class TabAuditPage;
}

class TabAuditPage : public QWidget
{
    Q_OBJECT

public:
    explicit TabAuditPage(QWidget *parent = 0);
    ~TabAuditPage();

private:
    Ui::TabAuditPage *ui;
};

#endif // TABAUDITPAGE_H
