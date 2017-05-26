#ifndef AUDINFOPAGE_H
#define AUDINFOPAGE_H

#include <QWidget>
#include"querydialog.h"
#include "common.h"
#include "audfunclass.h"
#include <QSettings>
#include <QKeyEvent>
namespace Ui {
class AudInfoPage;
}



class AudInfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit AudInfoPage(QWidget *parent = 0);
    ~AudInfoPage();

    void read_and_display_file();
    void queryAction(QString cmd);
    void reportAction(QString cmd);

    void keyPressEvent(QKeyEvent * event);      //添加enter响应
private slots:
    void on_openButton_clicked();

    void on_queryButton_clicked();

    void on_reportButton_clicked();

    void on_fresh_pushButton_clicked();

    void on_pre_pushButton_clicked();

    void on_next_pushButton_clicked();

private:
    Ui::AudInfoPage *ui;

    QString fileName;
    OP_TYPE op_type;
    QString cmd;
};

#endif // AUDINFOPAGE_H
