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

    QHeaderView *headerGoods = ui->ruleTableWidget->horizontalHeader();
    //SortIndicator为水平标题栏文字旁边的三角指示器
    headerGoods->setSortIndicator(0, Qt::AscendingOrder);
    headerGoods->setSortIndicatorShown(true);
    headerGoods->setClickable(true);
    connect(headerGoods, SIGNAL(sectionClicked(int)), ui->ruleTableWidget, SLOT (sortByColumn(int)));

    QHeaderView *headerGoods1 = ui->f_p_tableWidget->horizontalHeader();
    //SortIndicator为水平标题栏文字旁边的三角指示器
    headerGoods1->setSortIndicator(0, Qt::AscendingOrder);
    headerGoods1->setSortIndicatorShown(true);
    headerGoods1->setClickable(true);
    connect(headerGoods1, SIGNAL(sectionClicked(int)), ui->f_p_tableWidget, SLOT (sortByColumn(int)));
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
    ui->domainTypeBox->addItem(STR_WU);
    ui->fileTypeBox->addItem(STR_WU);
    ui->classBox->addItem(STR_WU);
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
    ui->sourceTypeBox->addItem(STR_WU);
    ui->targetTypeBox->addItem(STR_WU);
    ui->classTypeBox->addItem(STR_WU);
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
        errMsgBox(exp.getErroWhat(), this);
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
#define SET_CURRENT_TEXT(ui, data)  \
ui->setCurrentIndex(0); \
for(int i=0; i<ui->count();i++)   \
{   \
    if(ui->itemText(i) == data)   \
    {   \
        ui->setCurrentIndex(i);   \
        break;  \
    }   \
}

//void SecRulesPage::on_domainTypeBox_activated(const QString &arg1)
//{
//    QList<TERule> tmprules, tmprules2;
//    QString file_t = ui->fileTypeBox->currentText();
//    QString class_t = ui->classBox->currentText();
//    //1.查找满足当前选中的文本的集合
//    if(arg1==STR_WU)
//    {
//        tmprules = terules;
//    }else
//        {
//        for(int i=0;i<terules.size();i++)
//        {
//            if(terules[i].domain_type == arg1)
//            {
//                tmprules.append(terules[i]);
//            }
//        }
//    }
//    QSet<QString> file_types;
//    QSet<QString> class_types;
//    //如果另外两个都是无，则直接把上面的结果更新到另外两个的下拉框中
//    if(file_t==STR_WU && class_t==STR_WU)
//    {
//        ui->fileTypeBox->clear();
//        ui->classBox->clear();
//        for(int i=0; i<tmprules.size(); i++)
//        {
//            file_types.insert(tmprules[i].file_type);
//            class_types.insert(tmprules[i].class_type);
//        }
//        ui->fileTypeBox->addItem(STR_WU);
//        ui->classBox->addItem(STR_WU);
//        foreach (const QString &value, file_types)
//             ui->fileTypeBox->addItem(value);
//        foreach (const QString &value, class_types)
//             ui->classBox->addItem(value);
//        ui->fileTypeBox->setCurrentIndex(0);
//        ui->classBox->setCurrentIndex(0);
//    }else if(file_t==STR_WU && class_t!=STR_WU)       //file是无，class不是无
//    {
//        for(int i=0;i<tmprules.size();i++)
//        {
//            if(class_t== tmprules[i].class_type )        //满足classt的
//                tmprules2.append(tmprules[i]);
//        }
//        ui->fileTypeBox->clear();
//        ui->classBox->clear();
//        ui->fileTypeBox->addItem(STR_WU);
//        ui->classBox->addItem(STR_WU);
//        for(int i=0; i<tmprules2.count();i++)
//        {
//            file_types.insert(tmprules2[i].file_type);
//            class_types.insert(tmprules2[i].class_type);
//        }
//        foreach (const QString &value, file_types)
//             ui->fileTypeBox->addItem(value);
//        foreach (const QString &value, class_types)
//             ui->classBox->addItem(value);
//        ui->fileTypeBox->setCurrentIndex(0);
//        SET_CURRENT_TEXT(ui->classBox, class_t);
//    }else if(file_t!=STR_WU && class_t==STR_WU)       //file是无，class不是无
//    {
//        for(int i=0;i<tmprules.size();i++)
//        {
//            if(file_t== tmprules[i].file_type)        //满足classt的
//                tmprules2.append(tmprules[i]);
//        }
//        ui->fileTypeBox->clear();
//        ui->classBox->clear();
//        ui->fileTypeBox->addItem(STR_WU);
//        ui->classBox->addItem(STR_WU);
//        for(int i=0; i<tmprules2.count();i++)
//        {
//            file_types.insert(tmprules2[i].file_type);
//            class_types.insert(tmprules2[i].class_type);
//        }
//        foreach (const QString &value, file_types)
//             ui->fileTypeBox->addItem(value);
//        foreach (const QString &value, class_types)
//             ui->classBox->addItem(value);
//        ui->classBox->setCurrentIndex(0);
//        SET_CURRENT_TEXT(ui->fileTypeBox, file_t);
//    }else  if(file_t!=STR_WU && class_t!=STR_WU)       //file是无，class不是无
//    {
//        for(int i=0;i<tmprules.size();i++)
//        {
//            if(class_t== tmprules[i].class_type && file_t== tmprules[i].file_type)        //满足classt的
//                tmprules2.append(tmprules[i]);
//        }
//        ui->fileTypeBox->clear();
//        ui->classBox->clear();
//        ui->fileTypeBox->addItem(STR_WU);
//        ui->classBox->addItem(STR_WU);
//        for(int i=0; i<tmprules2.count();i++)
//        {
//            file_types.insert(tmprules2[i].file_type);
//            class_types.insert(tmprules2[i].class_type);
//        }
//        foreach (const QString &value, file_types)
//             ui->fileTypeBox->addItem(value);
//        foreach (const QString &value, class_types)
//             ui->classBox->addItem(value);
//        SET_CURRENT_TEXT(ui->classBox, class_t);
//        SET_CURRENT_TEXT(ui->fileTypeBox, file_t);
//    }

//}

//void SecRulesPage::on_fileTypeBox_activated(const QString &arg1)
//{
//    QList<TERule> tmprules, tmprules2;
//    QString domain_t = ui->domainTypeBox->currentText();
//    QString class_t = ui->classBox->currentText();
//    //1.查找满足当前选中的文本的集合
//    if(arg1==STR_WU)
//    {
//        tmprules = terules;
//    }else
//        {
//        for(int i=0;i<terules.size();i++)
//        {
//            if(terules[i].file_type == arg1)
//            {
//                tmprules.append(terules[i]);
//            }
//        }
//    }
//    QSet<QString> domain_types;
//    QSet<QString> class_types;
//    //如果另外两个都是无，则直接把上面的结果更新到另外两个的下拉框中
//    if(domain_t==STR_WU && class_t==STR_WU)
//    {
//        ui->domainTypeBox->clear();
//        ui->classBox->clear();
//        for(int i=0; i<tmprules.size(); i++)
//        {
//            domain_types.insert(tmprules[i].domain_type);
//            class_types.insert(tmprules[i].class_type);
//        }
//        ui->domainTypeBox->addItem(STR_WU);
//        ui->classBox->addItem(STR_WU);
//        foreach (const QString &value, domain_types)
//             ui->domainTypeBox->addItem(value);
//        foreach (const QString &value, class_types)
//             ui->classBox->addItem(value);
//        ui->domainTypeBox->setCurrentIndex(0);
//        ui->classBox->setCurrentIndex(0);
//    }else if(domain_t==STR_WU && class_t!=STR_WU)       //file是无，class不是无
//    {
//        for(int i=0;i<tmprules.size();i++)
//        {
//            if(class_t== tmprules[i].class_type )        //满足classt的
//                tmprules2.append(tmprules[i]);
//        }
//        ui->domainTypeBox->clear();
//        ui->classBox->clear();
//        ui->domainTypeBox->addItem(STR_WU);
//        ui->classBox->addItem(STR_WU);
//        for(int i=0; i<tmprules2.count();i++)
//        {
//            domain_types.insert(tmprules2[i].domain_type);
//            class_types.insert(tmprules2[i].class_type);
//        }
//        foreach (const QString &value, domain_types)
//             ui->domainTypeBox->addItem(value);
//        foreach (const QString &value, class_types)
//             ui->classBox->addItem(value);
//        ui->domainTypeBox->setCurrentIndex(0);
//        SET_CURRENT_TEXT(ui->classBox, class_t);
//    }else if(domain_t!=STR_WU && class_t==STR_WU)       //file是无，class不是无
//    {
//        for(int i=0;i<tmprules.size();i++)
//        {
//            if(domain_t== tmprules[i].domain_type)        //满足classt的
//                tmprules2.append(tmprules[i]);
//        }
//        ui->domainTypeBox->clear();
//        ui->classBox->clear();
//        ui->domainTypeBox->addItem(STR_WU);
//        ui->classBox->addItem(STR_WU);
//        for(int i=0; i<tmprules2.count();i++)
//        {
//            domain_types.insert(tmprules2[i].domain_type);
//            class_types.insert(tmprules2[i].class_type);
//        }
//        foreach (const QString &value, domain_types)
//             ui->domainTypeBox->addItem(value);
//        foreach (const QString &value, class_types)
//             ui->classBox->addItem(value);
//        ui->classBox->setCurrentIndex(0);
//        SET_CURRENT_TEXT(ui->domainTypeBox, domain_t);
//    }else  if(domain_t!=STR_WU && class_t!=STR_WU)       //file是无，class不是无
//    {
//        for(int i=0;i<tmprules.size();i++)
//        {
//            if(class_t== tmprules[i].class_type && domain_t== tmprules[i].domain_type)        //满足classt的
//                tmprules2.append(tmprules[i]);
//        }
//        ui->domainTypeBox->clear();
//        ui->classBox->clear();
//        ui->domainTypeBox->addItem(STR_WU);
//        ui->classBox->addItem(STR_WU);
//        for(int i=0; i<tmprules2.count();i++)
//        {
//            domain_types.insert(tmprules2[i].file_type);
//            class_types.insert(tmprules2[i].class_type);
//        }
//        foreach (const QString &value, domain_types)
//             ui->domainTypeBox->addItem(value);
//        foreach (const QString &value, class_types)
//             ui->classBox->addItem(value);
//        SET_CURRENT_TEXT(ui->classBox, class_t);
//        SET_CURRENT_TEXT(ui->domainTypeBox, domain_t);
//    }
//}

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
            if(domain_t == STR_WU && file_t !=STR_WU && class_t !=STR_WU && terules[i].file_type == file_t && class_t == terules[i].class_type)
                tmprules.append(terules[i]);
            else if(domain_t != STR_WU && file_t==STR_WU && class_t !=STR_WU&&
                    terules[i].domain_type == domain_t && class_t == terules[i].class_type)
                tmprules.append(terules[i]);
            else if(domain_t != STR_WU && file_t!=STR_WU && class_t ==STR_WU&&
                    terules[i].domain_type == domain_t && file_t == terules[i].file_type)
                tmprules.append(terules[i]);
            else if(domain_t != STR_WU && file_t ==STR_WU && class_t ==STR_WU&&
                    terules[i].domain_type == domain_t )
                tmprules.append(terules[i]);
            else if(domain_t == STR_WU && file_t!=STR_WU && class_t ==STR_WU&&
                    terules[i].file_type == file_t )
                tmprules.append(terules[i]);
            else if(domain_t == STR_WU && file_t==STR_WU && class_t !=STR_WU&&
                    terules[i].class_type == class_t )
                tmprules.append(terules[i]);
            else  if(domain_t != STR_WU && file_t!=STR_WU && class_t !=STR_WU&&
                     terules[i].domain_type == domain_t&&
                   terules[i].file_type == file_t &&
                   terules[i].class_type == class_t)
                    tmprules.append(terules[i]);

        }
        if(tmprules.size()==0)
        {
            infoMsgBox(tr("未查找到符合条件的项") , this);
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
            if(src_t == STR_WU && targ_t !=STR_WU && class_t !=STR_WU && fpconvs[i].targ_type == targ_t && class_t == fpconvs[i].class_type)
                tmpconvs.append(fpconvs[i]);
            else if(src_t != STR_WU && targ_t==STR_WU && class_t !=STR_WU&&
                    fpconvs[i].src_type == src_t && class_t == fpconvs[i].class_type)
                tmpconvs.append(fpconvs[i]);
            else if(src_t != STR_WU && targ_t!=STR_WU && class_t ==STR_WU&&
                    fpconvs[i].src_type == src_t && targ_t == fpconvs[i].targ_type)
                tmpconvs.append(fpconvs[i]);
            else if(src_t != STR_WU && targ_t ==STR_WU && class_t ==STR_WU&&
                    fpconvs[i].src_type == src_t )
                tmpconvs.append(fpconvs[i]);
            else if(src_t == STR_WU && targ_t!=STR_WU && class_t ==STR_WU&&
                    fpconvs[i].targ_type == targ_t )
                tmpconvs.append(fpconvs[i]);
            else if(src_t == STR_WU && targ_t==STR_WU && class_t !=STR_WU&&
                    fpconvs[i].class_type == class_t )
                tmpconvs.append(fpconvs[i]);
            else  if(src_t != STR_WU && targ_t!=STR_WU && class_t !=STR_WU&&
                     fpconvs[i].src_type == src_t&&
                   fpconvs[i].targ_type == targ_t &&
                   fpconvs[i].class_type == class_t)
                    tmpconvs.append(fpconvs[i]);

//            if(fpconvs[i].src_type=="tgtd_t" && fpconvs[i].targ_type=="var_lib_t"&& fpconvs[i].class_type=="dir")
//                qDebug()<<"find it i="<<i;

        }
        if(tmpconvs.size()==0)
        {
            infoMsgBox(tr("未查找到符合条件的项"), this );
        }else
            UpdateFPTable(tmpconvs);
    }
}





