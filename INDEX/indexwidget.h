#ifndef INDEXWIDGET_H
#define INDEXWIDGET_H

#include <QWidget>
#include"common.h"
namespace Ui {
class IndexWidget;
}

class IndexWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IndexWidget(QWidget *parent = 0);
    ~IndexWidget();

private:
    Ui::IndexWidget *ui;
};

#endif // INDEXWIDGET_H
