#ifndef COMMTAB_H
#define COMMTAB_H

#include <QWidget>
#include<unistd.h>
#include <sys/stat.h>
#include"common.h"
#include<QSettings>
namespace Ui {
class CommTab;
}


class CommTab : public QWidget
{
    Q_OBJECT

public:
    explicit CommTab(QWidget *parent = 0);
    ~CommTab();

    bool getFileAtrribute(QString path);

    void cleanUI();
    void updateUI();

    void SetUiEnable(bool enable);

    void on_ownerComboBox_currentIndexChanged(int index);

    void on_groupComboBox_currentIndexChanged(int index);

    void persionSet(bool);

    void messageInfo(QString info);

private slots:
    void on_pushButton_clicked();
    void on_setMaskButton_clicked();

    void on_isDirheckBox_clicked();

    void on_setALCButton_clicked();

private:
    Ui::CommTab *ui;

    FileAttr fileAttr;
};

#endif // COMMTAB_H
