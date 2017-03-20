#include "tabsecritypage.h"
#include "ui_tabsecritypage.h"

TabSecrityPage::TabSecrityPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabSecrityPage)
{
    ui->setupUi(this);
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
    ui->listWidget->setCurrentRow(0);

    get_services();

    ui->comboBox->addItems(services);

    QRegExp regExp("[1-9][0-9]{0,2}");   //^[1-9][0-9]*$ 和 ^[1-9]{1}[/d]*$

                                                //上面的正则表达式表示只能输入大于0的正整数

    ui->tmsLineEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->secLineEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->tmsLineEdit->setText("3");
    ui->secLineEdit->setText("30");
    ui->comboBox->setCurrentIndex(0);


    //security status
    initSecStatusUI();
    if(!get_sec_status(secStatus))
    {
        secStatus.clear();
    }else
    {
        UpdateToSecStatus();
    }

    if(secStatus.curr_mode == tr("enforcing"))
        ui->open_closeButton->setText(tr("关闭安全策略"));
    else
        ui->open_closeButton->setText(tr("开启安全策略"));


    //user security tag
    get_user_taginfo(user_list);
}



void TabSecrityPage::UpdateToSecStatus()
{
    ui->seStaEdit->setText(secStatus.selinux_status);
    ui->fsMountEdit->setText(secStatus.selinux_fs_mount);
    ui->rootDirEdit->setText(secStatus.selinux_root_dir);
    ui->polnameEdit->setText(secStatus.load_policy_name);
    ui->modeEdit->setText(secStatus.curr_mode);
    ui->modeCfgEdit->setText(secStatus.mode_frm_cfg);
    ui->mlsEdit->setText(secStatus.mls_status);
    ui->denyEdit->setText(secStatus.policy_deny_stat);
    ui->versionEdit->setText(secStatus.max_kern_policy_version);
}

void TabSecrityPage::initSecStatusUI()
{
    ui->seStaEdit->setReadOnly(true);
    ui->fsMountEdit->setReadOnly(true);
    ui->rootDirEdit->setReadOnly(true);
    ui->polnameEdit->setReadOnly(true);
    ui->modeEdit->setReadOnly(true);
    ui->modeCfgEdit->setReadOnly(true);
    ui->mlsEdit->setReadOnly(true);
    ui->denyEdit->setReadOnly(true);
    ui->versionEdit->setReadOnly(true);
}

void TabSecrityPage::get_services()
{
    //enhanced-trylock  -l
    if(!get_trylock_services(services))
    {
        services<<"login"<<"gdm"<<"ssh";
    }

}

TabSecrityPage::~TabSecrityPage()
{
    delete ui;
}

void TabSecrityPage::on_setusButton_clicked()
{
    QString cmd = "enhanced-trylock -d "+ui->tmsLineEdit->text() + (ui->evrCheckBox->isChecked()? " -e -r ":" -u ")
            + ui->secLineEdit->text()+ " -s " + ui->comboBox->currentText() + "; echo $?";

    if(trylock_service(cmd))
    {
        QMessageBox::information(this, tr("提示"), tr("操作成功"));
    }else
        QMessageBox::warning(this, tr("提示"), tr("操作失败"));
}

void TabSecrityPage::on_unlockButton_clicked()
{
    if(unlock_all_users())
    {
        QMessageBox::information(this, tr("提示"), tr("操作成功"));
    }else
        QMessageBox::warning(this, tr("提示"), tr("操作失败"));
}

void TabSecrityPage::on_setPwButton_clicked()
{
    QString cmd = "enhanced-passwd "+
            (ui->minlenEdit->text().toInt()!=0? ("-m "+ui->minlenEdit->text()+ " "): " ")+
            (ui->dlenEdit->text().toInt()!=0? ("-d -"+ui->dlenEdit->text()+ " "): " ")+
            (ui->uplenEdit->text().toInt()!=0? ("-u -"+ui->uplenEdit->text()+ " "): " ")+
            (ui->lowlenEdit->text().toInt()!=0? ("-l -"+ui->lowlenEdit->text()+ " "): " ")+
            (ui->othlenEdit->text().toInt()!=0? ("-o -"+ui->othlenEdit->text()+ " "): " ")+
            (ui->evrCheckBox_2->isChecked()? "-e ": " ")+ " ; echo $?";

    if(set_pwd_rule(cmd))
    {
        QMessageBox::information(this, tr("提示"), tr("操作成功"));
    }else
        QMessageBox::warning(this, tr("提示"), tr("操作失败"));
}


void TabSecrityPage::on_open_closeButton_clicked()
{
    if(ui->open_closeButton->text() == tr("关闭安全策略"))
    {
        open_close_sec_policy(false);
        ui->open_closeButton->setText(tr("开启安全策略"));
    }
    else
    {
        open_close_sec_policy(true);
        ui->open_closeButton->setText(tr("关闭安全策略"));
    }
    on_fresh_staButton_clicked();
}


void TabSecrityPage::on_fresh_staButton_clicked()
{
    initSecStatusUI();
    if(!get_sec_status(secStatus))
    {
        secStatus.clear();
    }else
    {
        UpdateToSecStatus();
    }
}
