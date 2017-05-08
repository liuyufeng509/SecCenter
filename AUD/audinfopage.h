#ifndef AUDINFOPAGE_H
#define AUDINFOPAGE_H

#include <QWidget>

namespace Ui {
class AudInfoPage;
}

class AudInfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit AudInfoPage(QWidget *parent = 0);
    ~AudInfoPage();

private:
    Ui::AudInfoPage *ui;
};

#endif // AUDINFOPAGE_H
