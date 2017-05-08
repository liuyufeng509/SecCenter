#ifndef SVRMGRPAGE_H
#define SVRMGRPAGE_H

#include <QWidget>

namespace Ui {
class SvrMgrPage;
}

class SvrMgrPage : public QWidget
{
    Q_OBJECT

public:
    explicit SvrMgrPage(QWidget *parent = 0);
    ~SvrMgrPage();

private:
    Ui::SvrMgrPage *ui;
};

#endif // SVRMGRPAGE_H
