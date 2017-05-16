#ifndef AUDINFOPAGE_H
#define AUDINFOPAGE_H

#include <QWidget>
#include"querydialog.h"
#include "common.h"
#include "audfunclass.h"
#include <QSettings>
namespace Ui {
class AudInfoPage;
}

enum OP_TYPE
{
    OPEN_CLOSE_AUD,
    QUERY_SET,
    AUD_REPORT,
    KER_AUD_PARM,
    FILE_AUD_PARM,
    SYS_CALL_RULE,
    CUSTOM_RULE,
    AUD_CONFIG,
    AUD_DISPLAY

};

class AudInfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit AudInfoPage(QWidget *parent = 0);
    ~AudInfoPage();

    void read_and_display_file();
    void queryAction(QString cmd);
    void reportAction(QString cmd);
private slots:
    void on_openButton_clicked();

    void on_queryButton_clicked();

    void on_reportButton_clicked();

    void on_fresh_pushButton_clicked();

private:
    Ui::AudInfoPage *ui;

    QString fileName;
    OP_TYPE op_type;
    QString cmd;
};

#endif // AUDINFOPAGE_H
