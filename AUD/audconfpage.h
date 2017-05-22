#ifndef AUDCONFPAGE_H
#define AUDCONFPAGE_H

#include <QWidget>
#include<QCompleter>
#include"common.h"
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
private:
    Ui::AudConfPage *ui;
    QCompleter *completer;
    AudConfigInfo audCfgInfo;
};

#endif // AUDCONFPAGE_H
