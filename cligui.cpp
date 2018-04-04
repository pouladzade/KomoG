#include "cligui.h"
#include "komodocli.h"
#include "ui_cligui.h"
#include<qmessagebox.h>
#include<qscrollbar.h>
#include<QKeyEvent>

CliGui::CliGui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CliGui)
{
    ui->setupUi(this);
    Cli = std::make_shared<KomodoCli>();

    Process = nullptr;

    CommandWidgets.push_back(std::make_pair(ui->label_1,ui->lineEdit_1));
    CommandWidgets.push_back(std::make_pair(ui->label_2,ui->lineEdit_2));
    CommandWidgets.push_back(std::make_pair(ui->label_3,ui->lineEdit_3));
    CommandWidgets.push_back(std::make_pair(ui->label_4,ui->lineEdit_4));
    CommandWidgets.push_back(std::make_pair(ui->label_5,ui->lineEdit_5));
    CommandWidgets.push_back(std::make_pair(ui->label_6,ui->lineEdit_6));
    CommandWidgets.push_back(std::make_pair(ui->label_7,ui->lineEdit_7));
    CommandWidgets.push_back(std::make_pair(ui->label_8,ui->lineEdit_8));
    CommandWidgets.push_back(std::make_pair(ui->label_9,ui->lineEdit_9));
    CommandWidgets.push_back(std::make_pair(ui->label_10,ui->lineEdit_10));

    hideAll(true);
}

void CliGui::showEvent(QShowEvent* event)
{
    loadCommands();

}
CliGui::~CliGui()
{
    delete ui;

    if(Process)
    {
        Process->kill();
        delete Process;
        Process = nullptr;
    }
}

void CliGui::on_applyButton_clicked()
{
    QStringList argList;
    QString programName = getProgramName();
    if(Process)
    {
        Process->kill();
        delete Process;
        Process = nullptr;
    }

    Process = new CustomProcess;

    argList << CurrentCommand.getCommand();
    for(size_t i = 0 ; i < CurrentCommand.getInputsSize() ; i++ )
            argList << CommandWidgets[i].second->text();

    bool result =connect (Process, SIGNAL(readyReadStandardOutput()), this, SLOT(PrintOutPut()));
    Q_ASSERT(result);
    result =connect (Process, SIGNAL(readyReadStandardError()), this, SLOT(PrintError()));
    Q_ASSERT(result);
    result =connect (Process, SIGNAL(finished(int)), this, SLOT(processFinished(int)));
    Q_ASSERT(result);

    Process->start(programName, argList);
    if(!Process->waitForStarted(100) || !result)
    {
        QMessageBox msg;
        msg.setText("Error: Can not start process!!!");
        msg.exec();
    }
}

bool CliGui::loadCommands()
{
    QString path = getCommandFilePath();

    if(!Cli->loadCommands(path))
        return false;

    auto commands = Cli->getCommands();
    CurrentCommand = commands[0];

    for(auto command: commands)
        ui->comboBox->addItem(command.getCommand());

}

void CliGui::on_comboBox_currentIndexChanged(int index)
{
    hideAll(true);
    clearAll();
    auto commands = Cli->getCommands();
    Q_ASSERT(commands.size() > index);

    CurrentCommand = commands.at(index);

    auto inputs = CurrentCommand.getInputs();

    for(int i = 1; i <=inputs.size() ; i++)
        showInput(i-1,inputs[i-1]);
}

QString CliGui::getCommandFilePath() const
{
    return CommandFilePath;
}

void CliGui::setCommandFilePath(const QString &value)
{
    CommandFilePath = value;
}

QString CliGui::getProgramName() const
{
    return ProgramName;
}

void CliGui::setProgramName(const QString &value)
{
    ProgramName = value;
}

void  CliGui::hideAll(bool hide)
{
    for(auto pair : CommandWidgets)
    {
        pair.first->setHidden(hide);
        pair.second->setHidden(hide);
    }
}

void  CliGui::clearAll()
{
    for(auto pair : CommandWidgets)
    {
        pair.first->clear();
        pair.second->clear();
    }
}

void CliGui::showInput(int index , QString text)
{
    if(index  >= 0 && index < CommandWidgets.size())
    {
        CommandWidgets[index].first->setHidden(false);
        CommandWidgets[index].second->setHidden(false);
        CommandWidgets[index].first->setText(text);
    }
}

void CliGui::PrintError()
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

void CliGui::PrintOutPut()
{
    CustomProcess* process = reinterpret_cast<CustomProcess*>(sender());

    QByteArray byteArray = process->readAllStandardOutput();
    QStringList strLines = QString(byteArray).split("\n");

    for (QString line: strLines){
        ui->textBrowser->append(line);
    }

    ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());
}

void CliGui::processFinished(int exitCode)
{
    CustomProcess* process = reinterpret_cast<CustomProcess*>(sender());

    if(exitCode == 0)
        ui->textBrowser->append("<font color=\"green\">[" +process->arguments().at(0) + QString("]  finished successfully").arg(exitCode) + "</font>");
    else
        ui->textBrowser->append("<font color=\"red\">[" + process->arguments().at(0) +QString("]  finished with error code %1").arg(exitCode) + "</font>");

    ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());
}

void CliGui::keyPressEvent(QKeyEvent *event) // definition
{

    event->accept();

    QWidget::keyPressEvent(event);
    if ( (event->key()==Qt::Key_Enter) || (event->key()==Qt::Key_Return) )
    {
        on_applyButton_clicked();
    }
}

void CliGui::resizeEvent(QResizeEvent *event)
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
