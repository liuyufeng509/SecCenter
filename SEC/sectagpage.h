#ifndef SECTAGPAGE_H
#define SECTAGPAGE_H

#include <QWidget>

namespace Ui {
class SecTagPage;
}

class SecTagPage : public QWidget
{
    Q_OBJECT

public:
    explicit SecTagPage(QWidget *parent = 0);
    ~SecTagPage();

private:
    Ui::SecTagPage *ui;
};

#endif // SECTAGPAGE_H
