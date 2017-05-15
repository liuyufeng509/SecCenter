#include "svrctrlpage.h"
#include "ui_svrctrlpage.h"

SvrCtrlPage::SvrCtrlPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SvrCtrlPage)
{
    ui->setupUi(this);

    updateUI();
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
        errMsgBox(exp.getErroWhat());
    }

    if(secStatus.curr_mode == tr("enforcing"))
        ui->open_closeSecPolButton->setText(tr("关闭安全策略"));
    else if(secStatus.curr_mode == tr("permissive"))
        ui->open_closeSecPolButton->setText(tr("开启安全策略"));
    ui->cur_status_label->setText(tr("当前状态:       ")+secStatus.curr_mode);

        //sak开关状态
    try
    {
        SecFunClass::getInstance()->GetSakInfo(sakinfo);
        if(sakinfo.current_mode=="enable")
        {
            ui->cur_sakstatus_label->setText(tr("当前sak状态: 开启"));
            ui->open_close_sak_Button->setText(tr("关闭当前SAK功能"));
        }else
        {
            ui->cur_sakstatus_label->setText(tr("当前sak状态: 未开启"));
            ui->open_close_sak_Button->setText(tr("开启当前SAK功能"));
        }
        if(sakinfo.default_mode=="enable")
        {
            ui->def_sakstatus_label->setText(tr("默认sak状态: 开启"));
            ui->open_close_def_sak_Button->setText(tr("关闭默认SAK功能"));
        }else
        {
            ui->def_sakstatus_label->setText(tr("默认sak状态: 未开启"));
            ui->open_close_def_sak_Button->setText(tr("开启默认SAK功能"));
        }
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat());
        sakinfo.current_mode="disable";
        sakinfo.default_mode="disable";
        ui->cur_sakstatus_label->setText(tr("获取当前sak状态失败!"));
        ui->def_sakstatus_label->setText(tr("获取默认sak状态失败!"));
        ui->open_close_sak_Button->setText(tr("开启当前SAK功能"));
        ui->open_close_def_sak_Button->setText(tr("开启默认SAK功能"));
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
        errMsgBox(exp.getErroWhat());
    }

    try
    {
        SecFunClass::getInstance()->SetSakInfo("enable");
        SecFunClass::getInstance()->SetDefaultSakInfo("enable");
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat());
    }
    updateUI();
}

void SvrCtrlPage::on_open_closeSecPolButton_clicked()
{
    try
    {
        if(ui->open_closeSecPolButton->text() == tr("关闭安全策略"))
        {
            if(warnMsgBox(tr("关闭安全策略会使系统安全性降低，确定要进行该项操作？"))==QMessageBox::Cancel)
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
        errMsgBox(exp.getErroWhat());
    }
    updateUI();
}

void SvrCtrlPage::freshPolCtl(SecStatus secStatus)
{
    if(secStatus.curr_mode == tr("enforcing"))
        ui->open_closeSecPolButton->setText(tr("关闭安全策略"));
    else if(secStatus.curr_mode == tr("permissive"))
        ui->open_closeSecPolButton->setText(tr("开启安全策略"));

     ui->cur_status_label->setText(tr("当前状态:       ")+secStatus.curr_mode);
}

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
        errMsgBox(exp.getErroWhat());
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
        errMsgBox(exp.getErroWhat());
    }
    updateUI();
}
