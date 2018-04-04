#include "coinissuer.h"
#include "ui_coinissuer.h"
#include<QMessageBox>
#include<QScrollBar>
#include<QResizeEvent>

CoinIssuer::CoinIssuer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CoinIssuer)
{
    ui->setupUi(this);
    IssuerProcess = nullptr;
    IsKomodoRunning = nullptr;

    QRegExpValidator *validator = new QRegExpValidator(this);
    QRegExp regex("((1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})\\.){3,3}(1{0,1}[0-9]{0,2}|2[0-4]{1,1}[0-9]{1,1}|25[0-5]{1,1})");
    validator->setRegExp(regex);
    ui->iplineEdit->setValidator(validator);

}

CoinIssuer::~CoinIssuer()
{
    delete ui;
}

void CoinIssuer::on_startButton_clicked()
{
    if(IssuerProcess)delete IssuerProcess;
    IssuerProcess = new CustomProcess();
    IssuerProcess->setProcessKey(ProcessKey::IDE_PROCESS_ISSUER_START_ISSUING);
    QString command = "./komodo/src/komodod";
    QStringList arguments;

    /////////////////////////////////////////////////////////////////////////////////////////////
    // -ac_name=<ASSETCHAIN NAME> -ac_supply=<SUPPLY OF COINS> -addnode=<PUBLIC IP OF VPS #1> -gen
    //
    arguments << "-ac_name="   + ui->coin2textEdit->toPlainText()   <<
                 "-ac_supply=" + ui->supply2textEdit->toPlainText() <<
                 "-addnode="   + ui->iplineEdit->text()             <<
                 "-gen";

    bool result =connect (IssuerProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(PrintOutPut()));
    Q_ASSERT(result);
    result =connect (IssuerProcess, SIGNAL(readyReadStandardError()), this, SLOT(PrintError()));
    Q_ASSERT(result);
    result =connect (IssuerProcess, SIGNAL(finished(int)), this, SLOT(processFinished(int)));
    Q_ASSERT(result);

    IssuerProcess->start(command, arguments);
    if(!IssuerProcess->waitForStarted(100))
    {
        QMessageBox msg;
        msg.setText("Error: Can not start process!!!");
        msg.exec();
    }
}

void CoinIssuer::PrintError()
{
    CustomProcess* process = reinterpret_cast<CustomProcess*>(sender());

    QByteArray byteArray = process->readAllStandardError();
    QStringList strLines = QString(byteArray).split("\n");
    ui->textBrowser->append("\r\n [Error]");
    for (QString line: strLines){
        ui->textBrowser->append("<font color=\"#FBAD35\">" + line + "</font>");
    }

    ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());
}

void CoinIssuer::PrintOutPut()
{
    CustomProcess* process = reinterpret_cast<CustomProcess*>(sender());

    QByteArray byteArray = process->readAllStandardOutput();
    QStringList strLines = QString(byteArray).split("\n");

    for (QString line: strLines){
        ui->textBrowser->append(line);
    }

    ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());
}

void CoinIssuer::processFinished(int exitCode)
{
    CustomProcess* process = reinterpret_cast<CustomProcess*>(sender());

    if(exitCode == 0)
        ui->textBrowser->append("<font color=\"green\">" + getProcessName(process->getProcessKey())+ QString(" finished successfully").arg(exitCode) + "</font>");
    else
        ui->textBrowser->append("<font color=\"red\">" + getProcessName(process->getProcessKey())+ QString(" finished with error code %1").arg(exitCode) + "</font>");

    ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());
}

QString CoinIssuer::getProcessName(ProcessKey key)
{
    switch(key)
    {
    case ProcessKey::IDE_PROCESS_ISSUER_IS_KOMODO_RUNNING:
            return "(Is Komodo Running) ";

    case ProcessKey::IDE_PROCESS_ISSUER_START_ISSUING:
        return "(Coin Creation )";
    }

    return "";
}

void CoinIssuer::on_isKmdRunButton_clicked()
{
    if(IsKomodoRunning)delete IsKomodoRunning;
    IsKomodoRunning = new CustomProcess();
    IsKomodoRunning->setProcessKey(ProcessKey::IDE_PROCESS_ISSUER_IS_KOMODO_RUNNING);
    QString command = "./komodo/src/komodo-cli";
    QStringList arguments;

    /////////////////////////////////////////////////////////////////////////////////////////////
    // -ac_name=<ASSETCHAIN NAME> -ac_supply=<SUPPLY OF COINS> -addnode=<PUBLIC IP OF VPS #1> -gen
    //
    arguments << "getinfo" ;

    bool result =connect (IsKomodoRunning, SIGNAL(readyReadStandardOutput()), this, SLOT(PrintOutPut()));
    Q_ASSERT(result);
    result =connect (IsKomodoRunning, SIGNAL(readyReadStandardError()), this, SLOT(PrintError()));
    Q_ASSERT(result);
    result =connect (IsKomodoRunning, SIGNAL(finished(int)), this, SLOT(processFinished(int)));
    Q_ASSERT(result);

    IsKomodoRunning->start(command, arguments);
    if(!IsKomodoRunning->waitForStarted(100))
    {
        QMessageBox msg;
        msg.setText("Error: Can not start process!!!");
        msg.exec();
    }
}
void CoinIssuer::resizeEvent(QResizeEvent *event)
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
