#ifndef AUDCONFPAGE_H
#define AUDCONFPAGE_H

#include <QWidget>

namespace Ui {
class AudConfPage;
}

class AudConfPage : public QWidget
{
    Q_OBJECT

public:
    explicit AudConfPage(QWidget *parent = 0);
    ~AudConfPage();

private:
    Ui::AudConfPage *ui;
};

#endif // AUDCONFPAGE_H
