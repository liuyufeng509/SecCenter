#include "secrulespage.h"
#include "ui_secrulespage.h"

SecRulesPage::SecRulesPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SecRulesPage)
{
    ui->setupUi(this);

    //thread = new QThread;
   // SecFunClass::getInstance()->moveToThread(thread);
    qRegisterMetaType<TELIST> ("TELIST");
    qRegisterMetaType<F_PLIST>("F_PLIST");
    qRegisterMetaType<Exception>("Exception");
    connect(this, SIGNAL(emitGetSafePolicy(TELIST,F_PLIST)),SecFunClass::getInstance(), SLOT(getSafePolicySlot(TELIST,F_PLIST)));
    connect(SecFunClass::getInstance(), SIGNAL(emitGetSafePolicyDone(int,Exception, TELIST,F_PLIST)), this, SLOT(getSafePolicySlot(int,Exception, TELIST,F_PLIST)));
//    thread->start();
    bFirst = true;
}

void SecRulesPage::waitDialogAccept()
{
    waitD->accept();
    delete waitD;
}

void SecRulesPage::waitDiaogAppear()
{
    waitD = new WaitDialog(this);
    waitD->exec();
  //  waitD->deleteLater();
}

void SecRulesPage::getAllRules()
{
    if(bFirst)
        {
        emit emitGetSafePolicy(terules, fpconvs);
        waitDiaogAppear();
    }
}

void SecRulesPage::UpdateRuletabel(QList<TERule> &rules)
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

void SecRulesPage::UpdateFPTable(QList<FileProConV> &convs)
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

void SecRulesPage::InitRuleTab()
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

void SecRulesPage::InitFPTab()
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

void SecRulesPage::getSafePolicySlot(int res, Exception exp,TELIST teList, F_PLIST fpList)
{
   waitDialogAccept();
    terules = teList;
    fpconvs = fpList;
    if(res ==1)
    {
        errMsgBox(exp.getErroWhat());
        bFirst = true;
    }
    else
        bFirst = false;

    InitRuleTab();
    UpdateRuletabel(terules);
    InitFPTab();
    UpdateFPTable(fpconvs);

}

SecRulesPage::~SecRulesPage()
{
    delete ui;
}

void SecRulesPage::on_findButton_clicked()
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

void SecRulesPage::on_findButton2_clicked()
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
