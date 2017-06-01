#include "sectagpage.h"
#include "ui_sectagpage.h"
#include <QFileDialog>
SecTagPage::SecTagPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SecTagPage)
{
    ui->setupUi(this);

    //设置用户安全标签
    try
    {
        SecFunClass::getInstance()->getUserTagInfoList(userTagList);
        if(userTagList.size()>0)
        {
            for(int i=0; i<userTagList.size();i++)
                ui->users_comboBox->addItem(userTagList[i].username);

            ui->users_comboBox->setCurrentIndex(0);
            for(int i=0; i<ui->u_sec_tagcomboBox->count();i++)
            {
                if(ui->u_sec_tagcomboBox->itemText(i)==userTagList[0].safeTag)
                    ui->u_sec_tagcomboBox->setCurrentIndex(i);
            }
            for(int i=0; i<ui->u_whole_tagcomboBox->count();i++)
            {
                if(ui->u_whole_tagcomboBox->itemText(i)==userTagList[0].wholeTag)
                    ui->u_whole_tagcomboBox->setCurrentIndex(i);
            }

        }else
        {
            ui->users_comboBox->addItem(STR_WU);
            ui->u_sec_tagcomboBox->setCurrentIndex(0);
            ui->u_whole_tagcomboBox->setCurrentIndex(0);
        }
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat(), this);
    }

    //文件安全标签

    //多线程处理
   // thread = new QThread;
    //SecFunClass::getInstance()->moveToThread(thread);
    qRegisterMetaType<UserTag> ("UserTag");
    qRegisterMetaType<Exception> ("Exception");
    connect(this, SIGNAL(emitSetUserTagInfo(UserTag,int)),SecFunClass::getInstance(), SLOT(setUserTagInfoSlot(UserTag,int)));
    connect(SecFunClass::getInstance(), SIGNAL(emitSetUserTagInfoDone(int,Exception)), this, SLOT(setUserTagInfoSlot(int ,Exception)));
  //  thread->start();
}

SecTagPage::~SecTagPage()
{
    delete ui;
}

void SecTagPage::on_freshUserSafeTagButton_clicked()
{
    //1.更新用户列表
    QString curUser = ui->users_comboBox->currentText();
    //2.刷新当前选中的用户的信息
    try
    {
        if(SecFunClass::getInstance()->getUserTagInfoList(userTagList))
        {
            ui->users_comboBox->clear();
            if(userTagList.size()>0)
            {
                for(int i=0; i<userTagList.size();i++)
                    ui->users_comboBox->addItem(userTagList[i].username);
            }else
            {
                ui->users_comboBox->addItem(STR_WU);
            }

            ui->users_comboBox->setEditText(curUser);

            bool flag = false;
            int index = 0;
            for(int i=0; i<userTagList.length();i++)
            {
                if(userTagList[i].username == ui->users_comboBox->currentText())
                {
                    flag = true;
                    index = i;
                    break;
                }
            }
            if(!flag)
            {
                errMsgBox( tr("用户不存在!"), this);
            }else
            {
                for(int i=0; i<ui->u_sec_tagcomboBox->count();i++)
                {
                    if(ui->u_sec_tagcomboBox->itemText(i)==userTagList[index].safeTag)
                        ui->u_sec_tagcomboBox->setCurrentIndex(i);
                }
                for(int i=0; i<ui->u_whole_tagcomboBox->count();i++)
                {
                    if(ui->u_whole_tagcomboBox->itemText(i)==userTagList[index].wholeTag)
                        ui->u_whole_tagcomboBox->setCurrentIndex(i);
                }
            }
        }
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat(), this);
    }

}

void SecTagPage::waitDialogAccept()
{
    waitD->accept();
    delete waitD;
}

void SecTagPage::waitDiaogAppear()
{
    waitD = new WaitDialog(this);
    waitD->exec();
  //  waitD->deleteLater();
}

void SecTagPage::setUserTagInfoSlot(int res, Exception exp)
{
    waitDialogAccept();
    if(res == 0)
    {
        infoMsgBox(tr("设置用户安全标签成功"), this);
    }else
        errMsgBox(exp.getErroWhat(), this);
}

void SecTagPage::on_setUserTagButton_clicked()
{
    if(ui->users_comboBox->currentText().trimmed().length()==0)
    {
            infoMsgBox(tr("用户名不能为空!"), this);
            return;
    }
    UserTag usrtag;
    usrtag.username = ui->users_comboBox->currentText();
    usrtag.safeTag = ui->u_sec_tagcomboBox->currentText();
    usrtag.wholeTag = usrtag.safeTag;
    //usrtag.wholeTag = ui->u_whole_tagEdit->text();
    emit emitSetUserTagInfo(usrtag, userTagList.contains(usrtag)?1:0);

    waitDiaogAppear();
}

void SecTagPage::on_browserButton_clicked()
{
    if(ui->isDirheckBox->isChecked())
    {
        filePath = QFileDialog::getExistingDirectory(this, tr("选择目录"), "./");
        isDir = true;
    }else
    {
          filePath = QFileDialog::getOpenFileName(this, tr("打开文件"), ".", NULL);
          isDir = false;
    }

    ui->filenameEdit->setText(filePath);
    on_freshFileTagButton_clicked();
}

void SecTagPage::on_freshFileTagButton_clicked()
{
    if(filePath.trimmed().length()==0)
    {
        infoMsgBox(tr("文件名为空"), this);
        return;
    }

    FileTag fileinfo;
    fileinfo.filename = filePath;
    fileinfo.isDir = isDir;
    try
    {
        SecFunClass::getInstance()->getFileTagInfo(fileinfo);
        for(int i=0; i<ui->f_sec_tagcomboBox->count();i++)
        {
            if(ui->f_sec_tagcomboBox->itemText(i)==fileinfo.safeTag)
                ui->f_sec_tagcomboBox->setCurrentIndex(i);
        }

        for(int i=0; i<ui->f_whole_tagcomboBox->count();i++)
        {
            if(ui->f_whole_tagcomboBox->itemText(i)==fileinfo.wholeTag)
                ui->f_whole_tagcomboBox->setCurrentIndex(i);
        }
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat(), this);
    }
}

void SecTagPage::on_setFileTagButton_clicked()
{
    if(filePath.trimmed().length()==0)
    {
        infoMsgBox(tr("文件名为空"),this);
        return;
    }

    FileTag fileinfo;
    fileinfo.filename = filePath;
    fileinfo.safeTag = ui->f_sec_tagcomboBox->currentText();
    fileinfo.wholeTag = ui->f_whole_tagcomboBox->currentText();
    try
    {
        SecFunClass::getInstance()->setFileTagInfo(fileinfo);
        infoMsgBox(tr("设置成功!"), this);
    }catch(Exception exp)
            {
         errMsgBox(exp.getErroWhat(), this);
    }
}
