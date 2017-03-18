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

private slots:
    void on_openButton_clicked();

//    void on_queryButton_clicked();

//    void on_reportButton_clicked();

//    void on_roleButton_clicked();

//    void on_setCfgButton_clicked();

private:
    Ui::TabAuditPage *ui;
};

#endif // TABAUDITPAGE_H
