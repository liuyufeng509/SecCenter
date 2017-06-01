#include "secstatuspage.h"
#include "ui_secstatuspage.h"

SecStatusPage::SecStatusPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SecStatusPage)
{
    ui->setupUi(this);
//    QFont font;
//    font.setPointSize(11);
//    font.setFamily("Cantarell");
//    ui->label_9->setFont(font);
//    ui->label_10->setFont(font);
//    ui->label_11->setFont(font);
//    ui->label_12->setFont(font);
//    ui->label_14->setFont(font);
//    ui->label_15->setFont(font);
//    ui->label_16->setFont(font);
//    ui->label_17->setFont(font);

//    ui->seStalabel->setFont(font);
//    ui->fsMountlabel->setFont(font);
//    ui->rootDirlabel->setFont(font);
//    ui->polnamelabel->setFont(font);
//    ui->modeCfglabel->setFont(font);
//    ui->mlslabel->setFont(font);
//    ui->denylabel->setFont(font);
//    ui->versionlabel->setFont(font);

    UpdateToSecStatus();
}

void SecStatusPage::UpdateToSecStatus()
{
    try
    {
        SecFunClass::getInstance()->getSecStatus(secStatus);
    }catch(Exception exp)
            {
        secStatus.clear();
        errMsgBox(exp.getErroWhat(), this);
    }

//    if(secStatus.curr_mode == tr("enforcing"))
//        ui->open_closeSecPolButton->setText(tr("关闭安全策略"));
//    else if(secStatus.curr_mode == tr("permissive"))
//        ui->open_closeSecPolButton->setText(tr("开启安全策略"));

    ui->seStalabel->setText(secStatus.selinux_status);
    ui->fsMountlabel->setText(secStatus.selinux_fs_mount);
    ui->rootDirlabel->setText(secStatus.selinux_root_dir);
    ui->polnamelabel->setText(secStatus.load_policy_name);
    //ui->cur_status_label->setText(tr("当前状态:       ")+secStatus.curr_mode);
    ui->modeCfglabel->setText(secStatus.mode_frm_cfg);
    ui->mlslabel->setText(secStatus.mls_status);
    ui->denylabel->setText(secStatus.policy_deny_stat);
    ui->versionlabel->setText(secStatus.max_kern_policy_version);
}

SecStatusPage::~SecStatusPage()
{
    delete ui;
}

void SecStatusPage::on_freshSafeStatusButton_clicked()
{
    UpdateToSecStatus();
  //  emit freshClicked(secStatus);
}
