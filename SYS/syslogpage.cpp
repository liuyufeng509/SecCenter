#include "syslogpage.h"
#include "ui_syslogpage.h"
#include<QSplitter>
#include<QVBoxLayout>
#include "common.h"

SysLogPage::SysLogPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysLogPage)
{
    ui->setupUi(this);
    QSplitter *splitter = new QSplitter(this);

    model = new QFileSystemModel;
    QStringList logFilter;
    logFilter<<"messages*"<<"dmesg*"<<"secure*"<<"Xorg*";
    model->setNameFilterDisables(false);
    model->setNameFilters(logFilter);
    model->setFilter(QDir::Files);
    model->setRootPath("/var/log/");
    treeView = new QTreeView(splitter);

    treeView->setModel(model);
    treeView->setRootIndex(model->index("/var/log/"));
    treeView->setColumnHidden(1,true);
    treeView->setColumnHidden(2,true);
    treeView->setColumnHidden(3,true);
    treeView->sortByColumn(0,Qt::AscendingOrder);
    treeView->setMaximumWidth(200);

    QHBoxLayout *layout = new QHBoxLayout;


    ui->groupBox->setLayout(layout);
    nexButton = new QPushButton(">");
    nexButton->setMaximumWidth(30);
    nexButton->setObjectName("nexButton");
    preButton = new QPushButton("<");
    preButton->setObjectName("preButton");
    preButton->setMaximumWidth(30);
    comBox = new QComboBox;
    comBox->setEditable(true);
    QStringList combList;
    combList<<""<<"permission deny"<<"not permitted"<<"error"<<"loops";
    comBox->addItems(combList);
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(comBox);
    hlayout->addWidget(preButton);
    hlayout->addWidget(nexButton);
    browser = new QTextBrowser;

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addLayout(hlayout);
    vlayout->addWidget(browser);
     layout->addWidget(splitter);
    layout->addLayout(vlayout);
    connect(treeView, SIGNAL(clicked(QModelIndex)), this, SLOT(browserIndex(QModelIndex)));
    connect(nexButton, SIGNAL(clicked()), this, SLOT(findnext()));
    connect(preButton, SIGNAL(clicked()), this, SLOT(findPre()));

    treeView->header()->setHidden(true);
}

void SysLogPage::findnext()
{

    if(!browser->find(comBox->currentText()))
        infoMsgBox(tr("未查找到关键字"), this);

}

void SysLogPage::findPre()
{
    if(comBox->currentText().isEmpty())
        {
        infoMsgBox(tr("请输入关键字"), this);
        return;
    }

    if(!browser->find(comBox->currentText(),QTextDocument::FindBackward))
        infoMsgBox(tr("未查找到关键字"), this);

}


void SysLogPage::browserIndex(QModelIndex index)
{
//    QString path = model->data(index).toString();
//    QFileInfo fi(path);
    if(model->fileInfo(index).isDir())
        return;
    else
    {
        browser->setText("");

        QFile file(model->fileInfo(index).absoluteFilePath());
        if ( file.open( QFile::ReadOnly | QFile::Text) ) {
                QTextStream stream( &file );
//                QString line;
//                int n = 1;
//                while ( !stream.atEnd() ) {
//                    line = stream.readLine(); // 不包括“\n”的一行文本
//                    browser->append(line+"\n");
//                }
                browser->setText(stream.readAll());
                file.close();
        }
    }

}

SysLogPage::~SysLogPage()
{
    delete ui;
}
