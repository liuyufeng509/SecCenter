#ifndef AUDINFOPAGE_H
#define AUDINFOPAGE_H

#include <QWidget>
#include"querydialog.h"
namespace Ui {
class AudInfoPage;
}

class AudInfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit AudInfoPage(QWidget *parent = 0);
    ~AudInfoPage();

private slots:
    void on_openButton_clicked();

    void on_queryButton_clicked();

    void on_reportButton_clicked();

private:
    Ui::AudInfoPage *ui;
};

#endif // AUDINFOPAGE_H
