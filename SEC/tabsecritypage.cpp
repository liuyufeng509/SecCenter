#include "tabsecritypage.h"
#include "ui_tabsecritypage.h"
#include<QFileDialog>
#include"ukeydialog.h"
#define SEV_NAME  "auditd"
TabSecrityPage::TabSecrityPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TabSecrityPage)
{
    ui->setupUi(this);
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
    connect(ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(listItemChangedSlot(int)));
    ui->listWidget->setCurrentRow(0);

    //用户安全管理界面
    ui->userTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->userTableWidget->setEditTriggers ( QAbstractItemView::NoEditTriggers );
    QHeaderView* headerView = ui->userTableWidget->verticalHeader();
    headerView->setHidden(true);

    ui->userTableWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    secUserMenu = new QMenu(ui->userTableWidget);
    unLockAction = new QAction(tr("解锁用户"), this);
    ukeyPINAction = new QAction(tr("更改UkeyPIN"), this);
    ukeyBindAction = new QAction(tr("解绑UKey"), this);
    freshAction = new QAction(tr("刷新"),this);
    connect(unLockAction, SIGNAL(triggered()), this, SLOT(unLockActionSlot()));
    connect(ukeyPINAction, SIGNAL(triggered()), this, SLOT(ukeyPINActionSlot()));
    connect(ukeyBindAction, SIGNAL(triggered()), this, SLOT(ukeyBindActionSlot()));
    connect(freshAction, SIGNAL(triggered()), this, SLOT(freshActionSlot()));

    updateSecUserUI();

    //用户锁定规则设置
    getLockServices();
    ui->lockSvrComboBox->addItems(services);

    //密码规则设置
    QRegExp regExp("[1-9][0-9]{0,2}");   //^[1-9][0-9]*$ 和 ^[1-9]{1}[/d]*$

                                                //上面的正则表达式表示只能输入大于0的正整数

    ui->tmsLineEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->secLineEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->lockSvrComboBox->setCurrentIndex(0);
    ui->minlenEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->dlenEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->uplenEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->lowlenEdit->setValidator(new QRegExpValidator(regExp, this));
    ui->othlenEdit->setValidator(new QRegExpValidator(regExp, this));

    display_cur_pwd_info();

    //安全规则设置
    UpdateToSecStatus();
    //用户安全标签
    try
    {
        m_secFunModel.getUserTagInfoList(userTagList);
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
            ui->users_comboBox->addItem(tr("无"));
            ui->u_sec_tagcomboBox->setCurrentIndex(0);
            ui->u_whole_tagcomboBox->setCurrentIndex(0);
        }
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat());
    }

    //audit service start/stop
    if(m_secFunModel.servRunState(tr(SEV_NAME))!=RUNNING)
    {
        ui->closeAduButton->setText(tr("开启审计服务"));
        ui->cur_audstatus_label->setText(tr("当前状态：       尚未运行"));
    }else
    {
        ui->closeAduButton->setText(tr("关闭审计服务"));
        ui->cur_audstatus_label->setText(tr("当前状态：       正在运行"));
    }

    //open/close manager
    init_sak_ui();

    //多线程处理
    thread = new QThread;

    m_secFunModel.moveToThread(thread);
    //设置用户安全标签
    qRegisterMetaType<UserTag> ("UserTag");
    connect(this, SIGNAL(emitSetUserTagInfo(UserTag,int)),&m_secFunModel, SLOT(setUserTagInfoSlot(UserTag,int)));
    connect(&m_secFunModel, SIGNAL(emitSetUserTagInfoDone(int,Exception)), this, SLOT(setUserTagInfoSlot(int ,Exception)));
    //安全策略
    qRegisterMetaType<TELIST> ("TELIST");
    qRegisterMetaType<F_PLIST>("F_PLIST");
    connect(this, SIGNAL(emitGetSafePolicy(TELIST,F_PLIST)),&m_secFunModel, SLOT(getSafePolicySlot(TELIST,F_PLIST)));
    connect(&m_secFunModel, SIGNAL(emitGetSafePolicyDone(int,Exception, TELIST,F_PLIST)), this, SLOT(getSafePolicySlot(int,Exception, TELIST,F_PLIST)));
    bFirst = true;
    thread->start();
}

void TabSecrityPage::updateSecUserUI()
{
    try
    {
        m_secFunModel.getSecUserList(secUserList);
        ui->userTableWidget->setRowCount(0);
        ui->userTableWidget->setRowCount(secUserList.size());
        ui->userTableWidget->setToolTip(tr("右键可对用户进行操作"));
        for(int i=0; i<secUserList.length(); i++)
        {
            ui->userTableWidget->setItem(i, 0, new QTableWidgetItem(secUserList[i].uName));
            ui->userTableWidget->setItem(i, 1, new QTableWidgetItem(secUserList[i].bLocked?tr("是"):tr("否")));
            ui->userTableWidget->setItem(i, 2, new QTableWidgetItem(secUserList[i].bBindKey?tr("是"):tr("否")));
            if(!secUserList[i].bShow)
                ui->userTableWidget->setRowHidden(i, true);
        }

    }catch(Exception exp)
     {
        errMsgBox(exp.getErroWhat());
    }
}

void TabSecrityPage::on_userTableWidget_customContextMenuRequested(const QPoint &pos)
{
    pos.isNull();
    int row = ui->userTableWidget->currentRow();
    if(row<0)
        return;
    unLockAction->setEnabled(secUserList[row].bLocked); //被锁用户才可以使用
    secUserMenu->addAction(unLockAction);
    secUserMenu->addAction(ukeyPINAction);
    secUserList[row].bBindKey?ukeyBindAction->setText(tr("解绑Ukey")):ukeyBindAction->setText(tr("绑定Ukey"));
    secUserMenu->addAction(ukeyBindAction);
    secUserMenu->addAction(freshAction);
    secUserMenu->exec(QCursor::pos());
}

void TabSecrityPage::unLockActionSlot()
{
    int row = ui->userTableWidget->currentRow();
    if(row<0)
    {
        errMsgBox(tr("未选中要操作的用户!"));
        return;
    }
    try
    {
        m_secFunModel.unLockUser(secUserList[row].uName);
        infoMsgBox(tr("解锁成功"));
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat());
    }
    updateSecUserUI();
}

void TabSecrityPage::ukeyPINActionSlot()
{
    UkeyDialog ukdialog(UkeyDialog::Reset_PIN, "", this);
    ukdialog.exec();
}

void TabSecrityPage::ukeyBindActionSlot()
{
    int row = ui->userTableWidget->currentRow();
    if(row<0)
    {
        errMsgBox(tr("未选中要操作的用户!"));
        return;
    }

    if(ukeyBindAction->text()==tr("解绑Ukey"))
        {
        UkeyDialog ukdialog(UkeyDialog::UnBUND_User, secUserList[row].uName,this);
        ukdialog.exec();
    }else
        {
        UkeyDialog ukdialog(UkeyDialog::BUND_User, secUserList[row].uName,this);
        ukdialog.exec();
    }

}

void TabSecrityPage::freshActionSlot()
{
    updateSecUserUI();
}

void TabSecrityPage::setUserTagInfoSlot(int res, Exception exp)
{
    waitDialogAccept();
    if(res == 0)
    {
        infoMsgBox(tr("设置用户安全标签成功"));
    }else
        errMsgBox(exp.getErroWhat());
}

void TabSecrityPage::getSafePolicySlot(int res, Exception exp,TELIST teList, F_PLIST fpList)
{
   waitDialogAccept();
    terules = teList;
    fpconvs = fpList;
    if(res ==1)
    {
        errMsgBox(exp.getErroWhat());
        bFirst = false;
    }
    else
        bFirst = true;

    InitRuleTab();
    UpdateRuletabel(terules);
    InitFPTab();
    UpdateFPTable(fpconvs);

}

void TabSecrityPage::init_sak_ui()
{
    try
    {
        m_secFunModel.GetSakInfo(sakinfo);
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

void TabSecrityPage::display_cur_pwd_info()
{
    try
    {
        m_secFunModel.getCurPwdInfo(pwdInfo);
//        QString text = QString(tr("最小长度:"))+(pwdInfo.minLen.isEmpty()?tr("无限制"):pwdInfo.minLen)+" 数字个数:"+
//                (pwdInfo.dcredit.isEmpty()?tr("无限制"):pwdInfo.dcredit)+" 小写字母:"+
//                (pwdInfo.lcredit.isEmpty()?tr("无限制"):pwdInfo.lcredit)+" 大写字母:"+
//                (pwdInfo.ucredit.isEmpty()?tr("无限制"):pwdInfo.ucredit)+" 其他字符:"+
//                (pwdInfo.ocredit.isEmpty()?tr("无限制"):pwdInfo.ocredit);
        ui->minlenEdit->setText(pwdInfo.minLen.isEmpty()?tr("无限制"):pwdInfo.minLen);
        ui->dlenEdit->setText(pwdInfo.dcredit.isEmpty()?tr("无限制"):pwdInfo.dcredit);
        ui->uplenEdit->setText(pwdInfo.ucredit.isEmpty()?tr("无限制"):pwdInfo.ucredit);
        ui->lowlenEdit->setText(pwdInfo.lcredit.isEmpty()?tr("无限制"):pwdInfo.lcredit);
        ui->othlenEdit->setText(pwdInfo.ocredit.isEmpty()?tr("无限制"):pwdInfo.ocredit);
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat());
    }
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

void TabSecrityPage::listItemChangedSlot(int page)
{
    if(page == 4 && bFirst)       //servies
    {
        emit emitGetSafePolicy(terules, fpconvs);
        waitDiaogAppear();
//        try
//        {
//            m_secFunModel.getTeRules(terules);
//            InitRuleTab();
//            UpdateRuletabel(terules);
//            m_secFunModel.getFileProcessRules(fpconvs);
//            InitFPTab();
//            UpdateFPTable(fpconvs);
//            disconnect(ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(listItemChangedSlot(int)));
//        }catch(Exception exp)
//        {
//            errMsgBox(exp.getErroWhat());
//        }

    }
}

void TabSecrityPage::UpdateToSecStatus()
{
    try
    {
        m_secFunModel.getSecStatus(secStatus);
    }catch(Exception exp)
            {
        secStatus.clear();
        errMsgBox(exp.getErroWhat());
    }

    if(secStatus.curr_mode == tr("enforcing"))
        ui->open_closeSecPolButton->setText(tr("关闭安全策略"));
    else if(secStatus.curr_mode == tr("permissive"))
        ui->open_closeSecPolButton->setText(tr("开启安全策略"));

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


void TabSecrityPage::getLockServices()
{
    //enhanced-trylock  -l
    try
    {
        m_secFunModel.getLockServices(services);
    }catch(Exception exp)
            {
        exp.getErroWhat();
        services.clear();
        services<<"login"<<"gdm"<<"ssh";
    }
}

TabSecrityPage::~TabSecrityPage()
{
    delete ui;
}


void TabSecrityPage::on_setPwButton_clicked()
{
    if(!ui->minlenEdit->text().isEmpty() && ui->minlenEdit->text().toInt()<8)
    {
         errMsgBox(tr("最小长度不能小于8"));
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
        infoMsgBox(tr("操作成功"));
        display_cur_pwd_info();
    }else
        errMsgBox(tr("操作失败"));
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

//    if(filePath.length() == 0) {
//            infoMsgBox( tr("你未选择任何文件"));
//    } else {
//            infoMsgBox(tr("你选择了") + filePath);
//    }

    ui->filenameEdit->setText(filePath);
    on_freshFileTagButton_clicked();
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
            infoMsgBox(tr("未查找到符合条件的项") );
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
            infoMsgBox(tr("未查找到符合条件的项") );
        }else
            UpdateFPTable(tmpconvs);
    }
}


void TabSecrityPage::on_closeAduButton_clicked()
{
    try
    {
        if(ui->closeAduButton->text() == tr("开启审计服务"))
        {
            m_secFunModel.startOrStopService(SEV_NAME, 0);
            ui->closeAduButton->setText(tr("关闭审计服务"));
            ui->cur_audstatus_label->setText(tr("当前状态：       正在运行"));
        }else
        {
            m_secFunModel.startOrStopService(SEV_NAME,1);
            stop_service(SEV_NAME);
            ui->closeAduButton->setText(tr("开启审计服务"));
            ui->cur_audstatus_label->setText(tr("当前状态：       尚未运行"));
        }
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat());
    }
}

void TabSecrityPage::on_close_client_reuse_Button_clicked()
{

}

void TabSecrityPage::on_open_close_sak_Button_clicked()
{
    try
    {
        if(sakinfo.current_mode=="enable")
        {
            m_secFunModel.SetSakInfo("disable");
        }else
            m_secFunModel.SetSakInfo("enable");
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat());
    }

    init_sak_ui();
}

void TabSecrityPage::on_open_close_def_sak_Button_clicked()
{
    try
    {
        if(sakinfo.default_mode=="enable")
        {
            m_secFunModel.SetDefaultSakInfo("disable");
        }else
            m_secFunModel.SetDefaultSakInfo("enable");
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat());
    }

    init_sak_ui();
}


void TabSecrityPage::on_setTryLockButton_clicked()
{
    QString cmd = "nfs-enhanced-trylock -d "+ui->tmsLineEdit->text() + " -u "
            + ui->secLineEdit->text()+ " -s " + ui->lockSvrComboBox->currentText() + "; echo $?";
    TryLockInfo info;
    info.dParam =ui->tmsLineEdit->text();
    info.uParam = ui->secLineEdit->text();
    info.sParam = ui->lockSvrComboBox->currentText();
    try
    {
        m_secFunModel.tryLockOption(info);
        infoMsgBox(tr("用户锁定规则设置成功"));
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat());
    }
}

void TabSecrityPage::on_freshSafeStatusButton_clicked()
{
    UpdateToSecStatus();
}

void TabSecrityPage::on_freshUserSafeTagButton_clicked()
{
    //1.更新用户列表
    QString curUser = ui->users_comboBox->currentText();
    //2.刷新当前选中的用户的信息
    try
    {
        if(m_secFunModel.getUserTagInfoList(userTagList))
        {
            ui->users_comboBox->clear();
            if(userTagList.size()>0)
            {
                for(int i=0; i<userTagList.size();i++)
                    ui->users_comboBox->addItem(userTagList[i].username);
            }else
            {
                ui->users_comboBox->addItem(tr("无"));
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
                errMsgBox( tr("用户不存在!"));
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
        errMsgBox(exp.getErroWhat());
    }

}

void TabSecrityPage::on_setUserTagButton_clicked()
{
    if(ui->users_comboBox->currentText().trimmed().length()==0)
    {
            infoMsgBox(tr("用户名不能为空!"));
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

void TabSecrityPage::waitDialogAccept()
{
    waitD->accept();
    delete waitD;
}

void TabSecrityPage::waitDiaogAppear()
{
    waitD = new WaitDialog(this);
    waitD->exec();
    waitD->deleteLater();
}

void TabSecrityPage::on_freshFileTagButton_clicked()
{
    if(filePath.trimmed().length()==0)
    {
        infoMsgBox(tr("文件名为空"));
        return;
    }

    FileTag fileinfo;
    fileinfo.filename = filePath;
    fileinfo.isDir = isDir;
    try
    {
        m_secFunModel.getFileTagInfo(fileinfo);
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
        errMsgBox(exp.getErroWhat());
    }
}

void TabSecrityPage::on_setFileTagButton_clicked()
{
    if(filePath.trimmed().length()==0)
    {
        errMsgBox(tr("文件名为空"));
        return;
    }

    FileTag fileinfo;
    fileinfo.filename = filePath;
    fileinfo.safeTag = ui->f_sec_tagcomboBox->currentText();
    fileinfo.wholeTag = ui->f_whole_tagcomboBox->currentText();
    try
    {
        m_secFunModel.setFileTagInfo(fileinfo);
        infoMsgBox(tr("设置成功!"));
    }catch(Exception exp)
            {
         errMsgBox(exp.getErroWhat());
    }
}


void TabSecrityPage::on_lockSvrComboBox_currentIndexChanged(const QString &arg1)
{
    TryLockInfo info;
    info.sParam = arg1;

    try
    {
        m_secFunModel.getCurLockInfo(info);
        ui->tmsLineEdit->setText(info.dParam);
        ui->secLineEdit->setText(info.uParam);
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat());
    }
}



void TabSecrityPage::on_open_closeSecPolButton_clicked()
{
    try
    {
        if(ui->open_closeSecPolButton->text() == tr("关闭安全策略"))
        {
            m_secFunModel.setEnforce(false);
            ui->open_closeSecPolButton->setText(tr("开启安全策略"));
        }
        else
        {
            m_secFunModel.setEnforce(true);
            ui->open_closeSecPolButton->setText(tr("关闭安全策略"));
        }
    }catch(Exception exp)
    {
        errMsgBox(exp.getErroWhat());
    }

    UpdateToSecStatus();
}
