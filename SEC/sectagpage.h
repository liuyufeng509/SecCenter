#ifndef SECTAGPAGE_H
#define SECTAGPAGE_H

#include <QWidget>
#include "secfunclass.h"
#include "waitdialog.h"
#include <QThread>
namespace Ui {
class SecTagPage;
}

class SecTagPage : public QWidget
{
    Q_OBJECT

public:
    explicit SecTagPage(QWidget *parent = 0);
    ~SecTagPage();
    void Init();
    void waitDiaogAppear();
    void waitDialogAccept();
private slots:
    void on_freshUserSafeTagButton_clicked();

    void on_setUserTagButton_clicked();

    //多线程结果处理
    void setUserTagInfoSlot(int res, Exception exp);

    void on_browserButton_clicked();

    void on_freshFileTagButton_clicked();

    void on_setFileTagButton_clicked();

signals:
    void emitSetUserTagInfo(UserTag, int);      //设置用户安全标签
private:
    Ui::SecTagPage *ui;
    QList<UserTag> userTagList;
    WaitDialog *waitD;
    QString filePath;
    bool isDir;
    bool isFirst;
};

#endif // SECTAGPAGE_H
