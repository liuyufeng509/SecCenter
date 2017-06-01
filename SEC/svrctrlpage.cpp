#include "svrctrlpage.h"
#include "ui_svrctrlpage.h"

SvrCtrlPage::SvrCtrlPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SvrCtrlPage)
{
    ui->setupUi(this);
    isRmOpen = false;
    updateUI();
}

 void SvrCtrlPage::getHtmlStr(QString labelStr, QString color, QString status)
 {
    htmlStr="<html><head/><body><p>"+
            labelStr+"	&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; <span style=\"font-weight:600; color:"+
        color+";\">"+status+"</span></p></body></html>";
 }

void SvrCtrlPage::updateUI()
{
    //安全策略状态
    try
    {
        SecFunClass::getInstance()->getSecStatus(secStatus);
    }catch(Exception exp)
            {
        secStatus.clear();
        errMsgBox(exp.getErroWhat(), this);
    }
    QString color = GreenColor;
    if(secStatus.curr_mode == tr("enforcing"))
    {
        ui->open_closeSecPolButton->setText(tr("关闭安全策略"));
        color = GreenColor;
    }
    else if(secStatus.curr_mode == tr("permissive"))
    {
        ui->open_closeSecPolButton->setText(tr("开启安全策略"));
        color = BlueColor;
    }else
        color = RedColor;
    getHtmlStr(tr("当前状态:"), color,secStatus.curr_mode);
    //ui->cur_status_label->setText(tr("当前状态:       ")+secStatus.curr_mode);
    ui->cur_status_label->setText(htmlStr);

        //sak开关状态
    try
    {
        SecFunClass::getInstance()->GetSakInfo(sakinfo);
        if(sakinfo.current_mode=="enable")
        {
            color = GreenColor;
            getHtmlStr(tr("当前sak状态:"), color, tr("开启"));
            //ui->cur_sakstatus_label->setText(tr("当前sak状态: 开启"));
            ui->cur_sakstatus_label->setText(htmlStr);
            ui->open_close_sak_Button->setText(tr("关闭当前SAK功能"));
        }else
        {
            color = BlueColor;
            //ui->cur_sakstatus_label->setText(tr("当前sak状态: 未开启"));
            getHtmlStr(tr("当前sak状态:"), color, tr("未开启"));
            ui->cur_sakstatus_label->setText(htmlStr);
            ui->open_close_sak_Button->setText(tr("开启当前SAK功能"));
        }

        if(sakinfo.default_mode=="enable")
        {
            color = GreenColor;
            getHtmlStr(tr("默认sak状态:"), color, tr("开启"));
            //ui->def_sakstatus_label->setText(tr("默认sak状态: 开启"));
            ui->def_sakstatus_label->setText(htmlStr);
            ui->open_close_def_sak_Button->setText(tr("关闭默认SAK功能"));
        }else
        {
            color = BlueColor;
            getHtmlStr(tr("默认sak状态:"), color, tr("未开启"));
            //ui->def_sakstatus_label->setText(tr("默认sak状态: 未开启"));
            ui->def_sakstatus_label->setText(htmlStr);
            ui->open_close_def_sak_Button->setText(tr("开启默认SAK功能"));
        }
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat(), this);
        sakinfo.current_mode="disable";
        sakinfo.default_mode="disable";
        color = RedColor;
        getHtmlStr(tr("获取当前sak状态失败!"), RedColor,"");
        ui->cur_sakstatus_label->setText(htmlStr);
        getHtmlStr(tr("获取默认sak状态失败!"), RedColor,"");
        ui->def_sakstatus_label->setText(htmlStr);

        ui->open_close_sak_Button->setText(tr("开启当前SAK功能"));
        ui->open_close_def_sak_Button->setText(tr("开启默认SAK功能"));
    }
    //客体重用
    try
    {
        SecFunClass::getInstance()->isRmOpened(isRmOpen);
        if(isRmOpen)
            {
            ui->close_client_reuse_Button->setText(tr("关闭客体重用"));
            getHtmlStr(tr("当前状态："), GreenColor,  tr("正在运行"));
             ui->cur_audstatus_label_2->setText(htmlStr);
        }else
            {
            ui->close_client_reuse_Button->setText(tr("开启客体重用"));
            getHtmlStr(tr("当前状态："), BlueColor,  tr("尚未运行"));
            ui->cur_audstatus_label_2->setText(htmlStr);
        }
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat(), this);
    }
}

SvrCtrlPage::~SvrCtrlPage()
{
    delete ui;
}

void SvrCtrlPage::on_openAllButton_clicked()
{
    try
    {
        SecFunClass::getInstance()->setEnforce(true);
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat(), this);
    }

    try
    {
        SecFunClass::getInstance()->SetSakInfo("enable");
        SecFunClass::getInstance()->SetDefaultSakInfo("enable");
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat(), this);
    }
    try
    {
        SecFunClass::getInstance()->setRmOpened(true);
         infoMsgBox(tr("客体重用设置后，仅在新的终端中起效， \n或者在终端中运行bash命令生效"), this);
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat(), this);
    }

    updateUI();
}

void SvrCtrlPage::on_open_closeSecPolButton_clicked()
{
    try
    {
        if(ui->open_closeSecPolButton->text() == tr("关闭安全策略"))
        {
            if(warnMsgBox(tr("关闭安全策略会使系统安全性降低，确定要进行该项操作？"), this)==QMessageBox::Cancel)
                return;
            SecFunClass::getInstance()->setEnforce(false);
            ui->open_closeSecPolButton->setText(tr("开启安全策略"));
        }
        else
        {
            SecFunClass::getInstance()->setEnforce(true);
            ui->open_closeSecPolButton->setText(tr("关闭安全策略"));
        }
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat(), this);
    }
    updateUI();
}

//void SvrCtrlPage::freshPolCtl(SecStatus secStatus)
//{
//    if(secStatus.curr_mode == tr("enforcing"))
//        ui->open_closeSecPolButton->setText(tr("关闭安全策略"));
//    else if(secStatus.curr_mode == tr("permissive"))
//        ui->open_closeSecPolButton->setText(tr("开启安全策略"));

//     ui->cur_status_label->setText(tr("当前状态:       ")+secStatus.curr_mode);
//}

void SvrCtrlPage::on_open_close_sak_Button_clicked()
{
    try
    {
        if(sakinfo.current_mode=="enable")
        {
            SecFunClass::getInstance()->SetSakInfo("disable");
        }else
            SecFunClass::getInstance()->SetSakInfo("enable");
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat(), this);
    }
    updateUI();
}

void SvrCtrlPage::on_open_close_def_sak_Button_clicked()
{
    try
    {
        if(sakinfo.default_mode=="enable")
        {
            SecFunClass::getInstance()->SetDefaultSakInfo("disable");
        }else
            SecFunClass::getInstance()->SetDefaultSakInfo("enable");
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat(), this);
    }
    updateUI();
}

void SvrCtrlPage::on_close_client_reuse_Button_clicked()
{
    try
    {
        SecFunClass::getInstance()->setRmOpened(!isRmOpen);
        updateUI();
        infoMsgBox(tr("客体重用设置后，仅在新的终端中起效， \n或者在终端中运行bash命令生效"), this);
    }catch(Exception exp)
            {
        errMsgBox(exp.getErroWhat(), this);
    }
}
