#ifndef AUDCONFPAGE_H
#define AUDCONFPAGE_H

#include <QWidget>
#include<QCompleter>
#include"common.h"
#include "audfunclass.h"

namespace Ui {
class AudConfPage;
}

class AudConfPage : public QWidget
{
    Q_OBJECT

public:
    explicit AudConfPage(QWidget *parent = 0);
    ~AudConfPage();
    void getAudConfigInfo();
    void UpdateUI();
    void saveDataFromUI();

    bool IsValidChar(char ch);
    bool IsValidEmail(QString strEmail);
private slots:
    void on_okButton_clicked();

    void on_restartAduButton_clicked();

private:
    Ui::AudConfPage *ui;
    QCompleter *completer;
    AudConfigInfo audCfgInfo;
    QMap<QString, QString> config_settings;
};

#endif // AUDCONFPAGE_H
