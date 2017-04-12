#include "tabsecritypage.h"
#include "ui_tabsecritypage.h"
#include<QFileDialog>
#define SEV_NAME  "auditd"
TabSecrityPage::TabSecrityPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabSecrityPage)
{
    ui->setupUi(this);
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(init_data_of_page(int)));
    ui->listWidget->setCurrentRow(0);

    on_getlockusrsButton_clicked();

    get_services();

    ui->comboBox->addItems(services);

    QRegExp regExp("[1-9][0-9]{0,2}");   //^[1-9][0-9]*$ 和 ^[1-9]{1}[/d]*$

                                                //上面的正则表达式表示只能输入大于0的正整数

    ui->tmsLineEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->secLineEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->tmsLineEdit->setText("3");
    ui->secLineEdit->setText("30");
    ui->comboBox->setCurrentIndex(0);
    ui->minlenEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->dlenEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->uplenEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->lowlenEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->othlenEdit->setValidator(new QRegExpValidator(regExp, this));

    display_cur_pwd_info();

    //security status
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
    get_user_taginfos(user_list);
    if(user_list.size()>0)
    {
        for(int i=0; i<user_list.size();i++)
            ui->users_comboBox->addItem(user_list[i].username);

        ui->users_comboBox->setCurrentIndex(0);
        for(int i=0; i<ui->u_sec_tagcomboBox->count();i++)
        {
            if(ui->u_sec_tagcomboBox->itemText(i)==user_list[0].safeTag)
                ui->u_sec_tagcomboBox->setCurrentIndex(i);
        }
        for(int i=0; i<ui->u_whole_tagcomboBox->count();i++)
        {
            if(ui->u_whole_tagcomboBox->itemText(i)==user_list[0].wholeTag)
                ui->u_whole_tagcomboBox->setCurrentIndex(i);
        }

    }else
    {
        ui->users_comboBox->addItem(tr("无"));
        ui->u_sec_tagcomboBox->setCurrentIndex(0);
        ui->u_whole_tagcomboBox->setCurrentIndex(0);
    }

    //file security tag

    //te policy

    //audit service start/stop
    if(is_serv_running(tr(SEV_NAME))!=RUNNING)
    {
        ui->closeAduButton->setText(tr("开启审计服务"));
        ui->cur_audstatus_label->setText(tr("当前状态：       尚未运行"));
    }else
    {
        ui->closeAduButton->setText(tr("关闭审计服务"));
        ui->cur_audstatus_label->setText(tr("当前状态：       正在运行"));
    }
}

void TabSecrityPage::display_cur_pwd_info()
{
    if(get_cur_pwd_info(pwdInfo))
    {
        QString text = QString(tr("最小长度:"))+(pwdInfo.minLen.isEmpty()?tr("无限制"):pwdInfo.minLen)+" 数字个数:"+
                (pwdInfo.dcredit.isEmpty()?tr("无限制"):pwdInfo.dcredit)+" 小写字母:"+
                (pwdInfo.lcredit.isEmpty()?tr("无限制"):pwdInfo.lcredit)+" 大写字母:"+
                (pwdInfo.ucredit.isEmpty()?tr("无限制"):pwdInfo.ucredit)+" 其他字符:"+
                (pwdInfo.ocredit.isEmpty()?tr("无限制"):pwdInfo.ocredit);
        ui->cur_pwd_label->setText(tr("当前规则：")+text);
    }else
        ui->cur_pwd_label->setText(tr("当前规则：")+tr("获取当前密码规则失败"));
}

void TabSecrityPage::InitRuleTab()
{
    ui->ruleTableWidget->horizontalHeader()->setStretchLastSection(true);
    QHeaderView* headerView = ui->ruleTableWidget->verticalHeader();
    headerView->setHidden(true);
    ui->ruleTableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->ruleTableWidget->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    ui->ruleTableWidget->setEditTriggers ( QAbstractItemView::NoEditTriggers );
    for(int i=0; i<terules.size(); i++)
    {
        domain_types.insert(terules[i].domain_type);
        file_types.insert(terules[i].file_type);
        class_types.insert(terules[i].class_type);
    }
    ui->domainTypeBox->addItem(tr("无"));
    ui->fileTypeBox->addItem(tr("无"));
    ui->classBox->addItem(tr("无"));
    foreach (const QString &value, domain_types)
         ui->domainTypeBox->addItem(value);
    foreach (const QString &value, file_types)
         ui->fileTypeBox->addItem(value);
    foreach (const QString &value, class_types)
         ui->classBox->addItem(value);
}

void TabSecrityPage::InitFPTab()
{
    ui->f_p_tableWidget->horizontalHeader()->setStretchLastSection(true);
    QHeaderView* headerView = ui->f_p_tableWidget->verticalHeader();
    headerView->setHidden(true);
    ui->f_p_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->f_p_tableWidget->setSelectionMode ( QAbstractItemView::SingleSelection); //设置选择模式，选择单行
    ui->f_p_tableWidget->setEditTriggers ( QAbstractItemView::NoEditTriggers );
    for(int i=0; i<fpconvs.size(); i++)
    {
        src_types.insert(fpconvs[i].src_type);
        targ_types.insert(fpconvs[i].targ_type);
        class_types_of_fp.insert(fpconvs[i].class_type);
    }
    ui->sourceTypeBox->addItem(tr("无"));
    ui->targetTypeBox->addItem(tr("无"));
    ui->classTypeBox->addItem(tr("无"));
    foreach (const QString &value, src_types)
         ui->sourceTypeBox->addItem(value);
    foreach (const QString &value, targ_types)
         ui->targetTypeBox->addItem(value);
    foreach (const QString &value, class_types_of_fp)
         ui->classTypeBox->addItem(value);
}

void TabSecrityPage::UpdateRuletabel(QList<TERule> &rules)
{
    ui->ruleTableWidget->setRowCount(0);
    ui->ruleTableWidget->setRowCount(rules.size());
    for(int i=0;i<rules.size();i++)
    {
        ui->ruleTableWidget->setItem(i, 0, new QTableWidgetItem(rules[i].domain_type));
        ui->ruleTableWidget->setItem(i, 1, new QTableWidgetItem(rules[i].file_type));
        ui->ruleTableWidget->setItem(i, 2, new QTableWidgetItem(rules[i].class_type));
        ui->ruleTableWidget->setItem(i, 3, new QTableWidgetItem(rules[i].permmisions));
    }
}

void TabSecrityPage::UpdateFPTable(QList<FileProConV> &convs)
{
    ui->f_p_tableWidget->setRowCount(0);
    ui->f_p_tableWidget->setRowCount(convs.size());
    for(int i=0;i<convs.size();i++)
    {
        ui->f_p_tableWidget->setItem(i, 0, new QTableWidgetItem(convs[i].src_type));
        ui->f_p_tableWidget->setItem(i, 1, new QTableWidgetItem(convs[i].targ_type));
        ui->f_p_tableWidget->setItem(i, 2, new QTableWidgetItem(convs[i].class_type));
        ui->f_p_tableWidget->setItem(i, 3, new QTableWidgetItem(convs[i].default_type));
    }
}

void TabSecrityPage::init_data_of_page(int page)
{
    if(page == 4)       //servies
    {
        get_te_rules(terules);
        InitRuleTab();
        UpdateRuletabel(terules);

        get_f_p_types(fpconvs);
        InitFPTab();
        UpdateFPTable(fpconvs);
        disconnect(ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(init_data_of_page(int)));
    }
}

void TabSecrityPage::UpdateToSecStatus()
{
    ui->seStalabel->setText(secStatus.selinux_status);
    ui->fsMountlabel->setText(secStatus.selinux_fs_mount);
    ui->rootDirlabel->setText(secStatus.selinux_root_dir);
    ui->polnamelabel->setText(secStatus.load_policy_name);
    ui->cur_status_label->setText(tr("当前状态:       ")+secStatus.curr_mode);
    ui->modeCfglabel->setText(secStatus.mode_frm_cfg);
    ui->mlslabel->setText(secStatus.mls_status);
    ui->denylabel->setText(secStatus.policy_deny_stat);
    ui->versionlabel->setText(secStatus.max_kern_policy_version);
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
    QString cmd = "nfs-enhanced-trylock -d "+ui->tmsLineEdit->text() + " -u "
            + ui->secLineEdit->text()+ " -s " + ui->comboBox->currentText() + "; echo $?";

    if(trylock_service(cmd))
    {
        QMessageBox::information(this, tr("提示"), tr("操作成功"));
    }else
        QMessageBox::warning(this, tr("提示"), tr("操作失败"));
}

void TabSecrityPage::on_unlockButton_clicked()
{
//    if(unlock_all_users())
//    {
//        QMessageBox::information(this, tr("提示"), tr("操作成功"));
//    }else
//        QMessageBox::warning(this, tr("提示"), tr("操作失败"));
    if(unlock_user(ui->locked_usr_comboBox->currentText()))
    {
        QMessageBox::information(this, tr("提示"), tr("操作成功"));
    }else
        QMessageBox::warning(this, tr("提示"), tr("操作失败"));
    on_getlockusrsButton_clicked();
}

void TabSecrityPage::on_setPwButton_clicked()
{
    if(!ui->minlenEdit->text().isEmpty() && ui->minlenEdit->text().toInt()<8)
    {
         QMessageBox::information(this, tr("提示"), tr("最小长度不能小于8"));
        return;
    }
    QString cmd = "nfs-enhanced-passwd "+
            (ui->minlenEdit->text().toInt()!=0? ("-m "+ui->minlenEdit->text()+ " "): " ")+
            (ui->dlenEdit->text().toInt()!=0? ("-d -"+ui->dlenEdit->text()+ " "): " ")+
            (ui->uplenEdit->text().toInt()!=0? ("-u -"+ui->uplenEdit->text()+ " "): " ")+
            (ui->lowlenEdit->text().toInt()!=0? ("-l -"+ui->lowlenEdit->text()+ " "): " ")+
            (ui->othlenEdit->text().toInt()!=0? ("-o -"+ui->othlenEdit->text()+ " "): " ")+
            " ; echo $?";

    if(set_pwd_rule(cmd))
    {
        QMessageBox::information(this, tr("提示"), tr("操作成功"));
        display_cur_pwd_info();
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
    if(!get_sec_status(secStatus))
    {
        secStatus.clear();
    }else
    {
        UpdateToSecStatus();
    }
}

void TabSecrityPage::on_freshButton_clicked()
{
    if(get_user_taginfos(user_list))
    {
        bool flag = false;
        int index = 0;
        for(int i=0; i<user_list.length();i++)
        {
            if(user_list[i].username == ui->users_comboBox->currentText())
            {
                flag = true;
                index = i;
                break;
            }
        }
        if(!flag)
        {
            QMessageBox::information(this, tr("提示"), tr("用户不存在!"));
        }else
        {
            for(int i=0; i<ui->u_sec_tagcomboBox->count();i++)
            {
                if(ui->u_sec_tagcomboBox->itemText(i)==user_list[index].safeTag)
                    ui->u_sec_tagcomboBox->setCurrentIndex(i);
            }
            for(int i=0; i<ui->u_whole_tagcomboBox->count();i++)
            {
                if(ui->u_whole_tagcomboBox->itemText(i)==user_list[index].wholeTag)
                    ui->u_whole_tagcomboBox->setCurrentIndex(i);
            }
        }
    }
}

void TabSecrityPage::on_setButton_clicked()
{
    if(ui->users_comboBox->currentText().trimmed().length()==0)
    {
            QMessageBox::information(this,tr("提示"), tr("用户名不能为空!"));
            return;
    }
    UserTag usrtag;
    usrtag.username = ui->users_comboBox->currentText();
    usrtag.safeTag = ui->u_sec_tagcomboBox->currentText();
    usrtag.wholeTag = usrtag.safeTag;
    //usrtag.wholeTag = ui->u_whole_tagEdit->text();


    if(set_user_tagInfo(usrtag, !user_list.contains(usrtag)))
    {
        QMessageBox::information(this,tr("提示"), tr("设置成功!"));
    }
    else
    {
        QMessageBox::information(this,tr("提示"), tr("设置失败!"));
    }

}

void TabSecrityPage::on_f_freshButton_clicked()
{
    if(filePath.trimmed().length()==0)
    {
        QMessageBox::information(this, tr("提示"), tr("文件名为空"));
        return;
    }

    FileTag fileinfo;
    fileinfo.filename = filePath;
    fileinfo.isDir = isDir;
    if(get_filetag_info(fileinfo))
    {
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
    }else
        QMessageBox::information(this, tr("提示"), tr("刷新失败"));

}

void TabSecrityPage::on_browserButton_clicked()
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

    if(filePath.length() == 0) {
            QMessageBox::information(NULL, tr("路径"), tr("你未选择任何文件"));
    } else {
            QMessageBox::information(NULL, tr("路径"), tr("你选择了") + filePath);
    }

    ui->filenameEdit->setText(filePath);
    on_f_freshButton_clicked();
}

void TabSecrityPage::on_f_setButton_clicked()
{
    if(filePath.trimmed().length()==0)
    {
        QMessageBox::information(this, tr("提示"), tr("文件名为空"));
        return;
    }

    FileTag fileinfo;
    fileinfo.filename = filePath;
    fileinfo.safeTag = ui->f_sec_tagcomboBox->currentText();
    fileinfo.wholeTag = ui->f_whole_tagcomboBox->currentText();
    if(set_filetag_info(fileinfo))
    {
        QMessageBox::information(this,tr("提示"), tr("设置成功!"));
    }else
        QMessageBox::information(this,tr("提示"), tr("设置失败!"));
}

void TabSecrityPage::on_findButton_clicked()
{
    QList<TERule> tmprules;
    QString domain_t = ui->domainTypeBox->currentText();
    QString file_t = ui->fileTypeBox->currentText();
    QString class_t = ui->classBox->currentText();
    if(ui->domainTypeBox->currentIndex()==0 && ui->fileTypeBox->currentIndex()==0 && ui->classBox->currentIndex()==0)
        UpdateRuletabel(terules);
    else
    {
        for(int i=0;i<terules.size();i++)
        {
            if(domain_t == tr("无") && file_t !=tr("无") && class_t !=tr("无") && terules[i].file_type == file_t && class_t == terules[i].class_type)
                tmprules.append(terules[i]);
            else if(domain_t != tr("无") && file_t==tr("无") && class_t !=tr("无")&&
                    terules[i].domain_type == domain_t && class_t == terules[i].class_type)
                tmprules.append(terules[i]);
            else if(domain_t != tr("无") && file_t!=tr("无") && class_t ==tr("无")&&
                    terules[i].domain_type == domain_t && file_t == terules[i].file_type)
                tmprules.append(terules[i]);
            else if(domain_t != tr("无") && file_t ==tr("无") && class_t ==tr("无")&&
                    terules[i].domain_type == domain_t )
                tmprules.append(terules[i]);
            else if(domain_t == tr("无") && file_t!=tr("无") && class_t ==tr("无")&&
                    terules[i].file_type == file_t )
                tmprules.append(terules[i]);
            else if(domain_t == tr("无") && file_t==tr("无") && class_t !=tr("无")&&
                    terules[i].class_type == class_t )
                tmprules.append(terules[i]);
            else  if(domain_t != tr("无") && file_t!=tr("无") && class_t !=tr("无")&&
                     terules[i].domain_type == domain_t&&
                   terules[i].file_type == file_t &&
                   terules[i].class_type == file_t)
                    tmprules.append(terules[i]);

        }
        if(tmprules.size()==0)
        {
            QMessageBox::information(this,tr("提示"), tr("未查找到符合条件的项") );
        }else
            UpdateRuletabel(tmprules);
    }
}

void TabSecrityPage::on_findButton2_clicked()
{
    QList<FileProConV> tmpconvs;
    QString src_t = ui->sourceTypeBox->currentText();
    QString targ_t = ui->targetTypeBox->currentText();
    QString class_t = ui->classTypeBox->currentText();
    if(ui->sourceTypeBox->currentIndex()==0 && ui->targetTypeBox->currentIndex()==0 && ui->classTypeBox->currentIndex()==0)
        UpdateFPTable(fpconvs);
    else
    {
        for(int i=0;i<fpconvs.size();i++)
        {
            if(src_t == tr("无") && targ_t !=tr("无") && class_t !=tr("无") && fpconvs[i].targ_type == targ_t && class_t == fpconvs[i].class_type)
                tmpconvs.append(fpconvs[i]);
            else if(src_t != tr("无") && targ_t==tr("无") && class_t !=tr("无")&&
                    fpconvs[i].src_type == src_t && class_t == fpconvs[i].class_type)
                tmpconvs.append(fpconvs[i]);
            else if(src_t != tr("无") && targ_t!=tr("无") && class_t ==tr("无")&&
                    fpconvs[i].src_type == src_t && targ_t == fpconvs[i].targ_type)
                tmpconvs.append(fpconvs[i]);
            else if(src_t != tr("无") && targ_t ==tr("无") && class_t ==tr("无")&&
                    fpconvs[i].src_type == src_t )
                tmpconvs.append(fpconvs[i]);
            else if(src_t == tr("无") && targ_t!=tr("无") && class_t ==tr("无")&&
                    fpconvs[i].targ_type == targ_t )
                tmpconvs.append(fpconvs[i]);
            else if(src_t == tr("无") && targ_t==tr("无") && class_t !=tr("无")&&
                    fpconvs[i].class_type == class_t )
                tmpconvs.append(fpconvs[i]);
            else  if(src_t != tr("无") && targ_t!=tr("无") && class_t !=tr("无")&&
                     fpconvs[i].src_type == src_t&&
                   fpconvs[i].targ_type == targ_t &&
                   fpconvs[i].class_type == class_t)
                    tmpconvs.append(fpconvs[i]);

//            if(fpconvs[i].src_type=="tgtd_t" && fpconvs[i].targ_type=="var_lib_t"&& fpconvs[i].class_type=="dir")
//                qDebug()<<"find it i="<<i;

        }
        if(tmpconvs.size()==0)
        {
            QMessageBox::information(this,tr("提示"), tr("未查找到符合条件的项") );
        }else
            UpdateFPTable(tmpconvs);
    }
}

void TabSecrityPage::on_getlockusrsButton_clicked()
{
    int erron=0;
    get_all_locked_users(users, erron);
    ui->locked_usr_comboBox->clear();
    if(users.isEmpty())
        ui->locked_usr_comboBox->addItem(tr("无"));
    else
    {
        for(int i=0; i<users.size();i++)
        {
            ui->locked_usr_comboBox->addItem(users[i]);
        }
    }

}

void TabSecrityPage::on_closeAduButton_clicked()
{
    if(ui->closeAduButton->text() == tr("开启审计服务"))
    {
        if(!start_service(SEV_NAME))
        {
            QMessageBox::information(this, tr("提示"), tr("启动失败"));
            return;
        }
        ui->closeAduButton->setText(tr("关闭审计服务"));
        ui->cur_audstatus_label->setText(tr("当前状态：       正在运行"));
    }else
    {
        if(!stop_service(SEV_NAME))
        {
            QMessageBox::information(this, tr("提示"), tr("停止失败"));
            return;
        }
        ui->closeAduButton->setText(tr("开启审计服务"));
        ui->cur_audstatus_label->setText(tr("当前状态：       尚未运行"));
    }
}

void TabSecrityPage::on_close_client_reuse_Button_clicked()
{

}
