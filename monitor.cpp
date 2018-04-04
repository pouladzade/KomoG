#include "monitor.h"
#include "ui_monitor.h"
#include<QMessageBox>
#include<QResizeEvent>
#include<QScrollBar>


Monitor::Monitor(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Monitor)
{
    ui->setupUi(this);
    Started = false;

}

Monitor::~Monitor()
{
    delete ui;
}

void Monitor::processFinished(int exitCode)
{
    CustomProcess* process = reinterpret_cast<CustomProcess*>(sender());
    QTextBrowser* textBrowser = nullptr;
    findTextBrowser(process->getProcessKey(), textBrowser);
    if(!textBrowser)
        return;

    if(process->getProcessKey() == ProcessKey::IDE_PROCESS_MONITORING_PERFORMANCE)
        textBrowser->clear();

    Program& program = getProgram(static_cast<ProcessKey>(process->getProcessKey()));

    if(exitCode == 0)
        textBrowser->append("<font color=\"green\">" + program.Label+ QString(" finished successfully").arg(exitCode) + "</font>");
    else
        textBrowser->append("<font color=\"red\">" + program.Label+ QString(" finished with error code %1").arg(exitCode) + "</font>");

    textBrowser->verticalScrollBar()->setValue(textBrowser->verticalScrollBar()->maximum());
}

void Monitor::closeEvent(QCloseEvent *event)
{
    for(size_t i = 0 ; i < Programs.size()  ; i++ )
    {
        CustomProcess* &process = Programs[i].Process;

        disconnect (process, SIGNAL(readyReadStandardOutput()), this, SLOT(PrintOutPut()));

        disconnect (process, SIGNAL(readyReadStandardError()), this, SLOT(PrintError()));

        disconnect (process, SIGNAL(finished(int)), this, SLOT(processFinished(int)));

    }

    Programs.clear();
    event->accept();
}

void Monitor::PrintError()
{
    CustomProcess* process = reinterpret_cast<CustomProcess*>(sender());
    QTextBrowser* textBrowser;
    findTextBrowser(process->getProcessKey(), textBrowser);
    if(!textBrowser)
        return;

    if(process->getProcessKey() == ProcessKey::IDE_PROCESS_MONITORING_PERFORMANCE)
        textBrowser->clear();

    QByteArray byteArray = process->readAllStandardError();
    QStringList strLines = QString(byteArray).split("\n");

    for (QString line: strLines){
        textBrowser->append("<font color=\"#FBAD35\">" + line + "</font>");
    }

    textBrowser->verticalScrollBar()->setValue(textBrowser->verticalScrollBar()->maximum());
}

void Monitor::PrintOutPut()
{
    CustomProcess* process = reinterpret_cast<CustomProcess*>(sender());
    QTextBrowser* textBrowser = nullptr;
    findTextBrowser(process->getProcessKey(), textBrowser);
    if(!textBrowser)
        return;

    if(process->getProcessKey() == ProcessKey::IDE_PROCESS_MONITORING_PERFORMANCE)
        textBrowser->clear();

    QByteArray byteArray = process->readAllStandardOutput();
    QStringList strLines = QString(byteArray).split("\n");

    for (QString line: strLines){
        textBrowser->append(line);
    }

    textBrowser->verticalScrollBar()->setValue(textBrowser->verticalScrollBar()->maximum());
}

void Monitor::findTextBrowser(ProcessKey key, QTextBrowser* &textBrowser)
{
    switch(key)
    {
        case ProcessKey::IDE_PROCESS_MONITORING_UP_LEFT:
        textBrowser = ui->textBrowser1;
            break;
        case ProcessKey::IDE_PROCESS_MONITORING_UP_RIGHT:
        textBrowser = ui->textBrowser2;
            break;
        case ProcessKey::IDE_PROCESS_MONITORING_DOWN_LEFT:
        textBrowser = ui->textBrowser3;
            break;
        case ProcessKey::IDE_PROCESS_MONITORING_DOWN_RIGHT:
        textBrowser = ui->textBrowser4;
            break;
        case ProcessKey::IDE_PROCESS_MONITORING_PERFORMANCE:
        textBrowser = ui->processBrowser;
            break;
        default :
            Q_ASSERT(false);
            textBrowser = nullptr;

    }
}

void Monitor::findLabel(ProcessKey key,QLabel* &lable)
{
    switch(key)
    {
        case ProcessKey::IDE_PROCESS_MONITORING_UP_LEFT:
        lable = ui->label1;
            break;
        case ProcessKey::IDE_PROCESS_MONITORING_UP_RIGHT:
        lable = ui->label2;
            break;
        case ProcessKey::IDE_PROCESS_MONITORING_DOWN_LEFT:
        lable = ui->label3;
            break;
        case ProcessKey::IDE_PROCESS_MONITORING_DOWN_RIGHT:
        lable = ui->label4;
            break;
        default :
            lable = new QLabel(this);
    }
}
void Monitor::on_stopButton_clicked()
{
    for(size_t i = 0 ; i < Programs.size() ; i++ )
    {
        CustomProcess* &process = Programs[i].Process;

        if(process)
        {
            process->kill();
        }
    }
    ui->startButton->setText("Start");
    Started = false;
}

void Monitor::on_closeButton_clicked()
{
    this->close();
}

std::vector<Program>* Monitor::getPrograms()
{
    return &Programs;
}

void Monitor::on_startButton_clicked()
{
    if(Started)
    {
        on_stopButton_clicked();
        return;
    }

    for(size_t i = 0 ; i < Programs.size()  ; i++ )
    {
        CustomProcess* &process = Programs[i].Process;
        if(process)delete process;
        process = new CustomProcess();
        process->setProcessKey(Programs[i].Pkey);
        QLabel* label = nullptr;
        findLabel(Programs[i].Pkey,label);
        label->setText(Programs[i].Label);

        bool result =connect (process, SIGNAL(readyReadStandardOutput()), this, SLOT(PrintOutPut()));
        Q_ASSERT(result);
        result =connect (process, SIGNAL(readyReadStandardError()), this, SLOT(PrintError()));
        Q_ASSERT(result);
        result =connect (process, SIGNAL(finished(int)), this, SLOT(processFinished(int)));
        Q_ASSERT(result);

        process->start(Programs[i].Command, Programs[i].Arguments);
        if(!process->waitForStarted(100))
        {
            QMessageBox msg;
            msg.setText("Error: Can not start process!!!");
            msg.exec();
        }
    }

    Started = true;
     ui->startButton->setText("Stop");

}

void Monitor::resizeEvent(QResizeEvent *event)
{
    if(std::abs(event->oldSize().width()) > 1 || std::abs(event->oldSize().height())>1 )
    {
        QRect gridRect = ui->gridWidget->geometry();
        gridRect.setWidth(this->geometry().width()-20);
        gridRect.setHeight(this->geometry().height()-50);
        ui->gridWidget->setGeometry(gridRect);
        QWidget::resizeEvent(event);
    }
}

Program& Monitor::getProgram(ProcessKey key)
{
    auto program = std::find_if(Programs.begin(),
                                Programs.end(),
                                [key](Program& program){return program.Process->getProcessKey() == key;});

    if(program != Programs.end())
        return *program;

    Q_ASSERT(false);

    return *program;
}
