#ifndef INDEXWIDGET_H
#define INDEXWIDGET_H

#include <QWidget>
#include"common.h"
#include "indexfunclass.h"

namespace Ui {
class IndexWidget;
}

class IndexWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IndexWidget(ROLE curRole,QWidget *parent = 0);
    ~IndexWidget();
    void initUi();
private:
    Ui::IndexWidget *ui;
    IndexFunClass m_indexFunModel;
    ROLE curRole;
};

#endif // INDEXWIDGET_H
