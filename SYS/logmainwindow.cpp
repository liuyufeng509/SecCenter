#include "logmainwindow.h"
#include "ui_logmainwindow.h"
#include<QSplitter>
#include<QVBoxLayout>
#include "common.h"
LogMainWindow::LogMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LogMainWindow)
{
    ui->setupUi(this);
    QSplitter *splitter = new QSplitter(ui->centralwidget);

     model = new QDirModel;
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
    preButton = new QPushButton("<");
    lineEdit = new QLineEdit;
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(lineEdit);
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

void LogMainWindow::findnext()
{

    if(!browser->find(lineEdit->text()))
        infoMsgBox(tr("未查找到关键字"));

}

void LogMainWindow::findPre()
{
    if(!browser->find(lineEdit->text(),QTextDocument::FindBackward))
        infoMsgBox(tr("未查找到关键字"));

}


void LogMainWindow::browserIndex(QModelIndex index)
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

LogMainWindow::~LogMainWindow()
{
    delete ui;
}
