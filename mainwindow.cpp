#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include<QContextMenuEvent>
#include"monitor.h"
#include"coinissuer.h"
#include <QDir>
#include"program.h"
#include<QScrollBar>
#include<QResizeEvent>
#include<math.h>
#include"cligui.h"
#include"config.h"
#include "utility.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);



    KomodoStarted = false;
    ui->privKeyLineEdit->setEchoMode(QLineEdit::Password);
    ui->privKeyLineEdit->setInputMethodHints(Qt::ImhHiddenText| Qt::ImhNoPredictiveText|Qt::ImhNoAutoUppercase);
    setLayout(ui->gridLayout);

}

void MainWindow::showEvent(QShowEvent* event)
{
    initializePrograms();
}

MainWindow::~MainWindow()
{
    delete ui;
    releasePrograms();

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    for(QWidget *dialog : Dialogs)
    {
        dialog->close();
        Utility::delay(100);
        delete dialog;
    }
    event->accept();
}

void MainWindow::on_actionBarterDEX_triggered()
{
    Monitor* monitor = new Monitor();
    monitor->setWindowTitle(Config::getInstance()->DataModel.apps_config[BARTERDEX_CONFIG_INDEX].name);
    auto programs = monitor->getPrograms();
    programs->push_back(Program(Config::getInstance()->DataModel.apps_config[BARTERDEX_CONFIG_INDEX].log_1,
                               Config::getInstance()->DataModel.log_viewer_command,
                               QStringList(Config::getInstance()->DataModel.log_viewer_params)<< Config::getInstance()->DataModel.apps_config[BARTERDEX_CONFIG_INDEX].log_1,
                               ProcessKey::IDE_PROCESS_MONITORING_UP_LEFT));

    programs->push_back(Program(Config::getInstance()->DataModel.apps_config[BARTERDEX_CONFIG_INDEX].log_2,
                               Config::getInstance()->DataModel.log_viewer_command,
                               QStringList(Config::getInstance()->DataModel.log_viewer_params) <<  Config::getInstance()->DataModel.apps_config[BARTERDEX_CONFIG_INDEX].log_2,
                               ProcessKey::IDE_PROCESS_MONITORING_UP_RIGHT));

    programs->push_back(Program(Config::getInstance()->DataModel.apps_config[BARTERDEX_CONFIG_INDEX].log_3,
                               Config::getInstance()->DataModel.log_viewer_command,
                               QStringList(Config::getInstance()->DataModel.log_viewer_params)<< Config::getInstance()->DataModel.apps_config[BARTERDEX_CONFIG_INDEX].log_3,
                               ProcessKey::IDE_PROCESS_MONITORING_DOWN_LEFT));

    programs->push_back(Program(Config::getInstance()->DataModel.apps_config[BARTERDEX_CONFIG_INDEX].log_4,
                               Config::getInstance()->DataModel.log_viewer_command,
                               QStringList(Config::getInstance()->DataModel.log_viewer_params)<<  Config::getInstance()->DataModel.apps_config[BARTERDEX_CONFIG_INDEX].log_4,
                               ProcessKey::IDE_PROCESS_MONITORING_DOWN_RIGHT));

    //Specially for process monitoring

    programs->push_back(Program("BarterDEX process monitor",
                               Config::getInstance()->DataModel.shell_name,
                               QStringList() << Config::getInstance()->DataModel.apps_config[BARTERDEX_CONFIG_INDEX].monitoring_script,
                               ProcessKey::IDE_PROCESS_MONITORING_PERFORMANCE));

    Dialogs.insert(monitor);
    monitor->show();
}

void MainWindow::on_actionKomodo_triggered()
{

    Monitor* monitor = new Monitor();

    monitor->setWindowTitle(Config::getInstance()->DataModel.apps_config[KOMODO_CONFIG_INDEX].name);
    auto programs = monitor->getPrograms();
    programs->push_back(Program(Config::getInstance()->DataModel.apps_config[KOMODO_CONFIG_INDEX].log_1,
                               Config::getInstance()->DataModel.log_viewer_command,
                               QStringList(Config::getInstance()->DataModel.log_viewer_params)<< Config::getInstance()->DataModel.apps_config[KOMODO_CONFIG_INDEX].log_1 ,
                               ProcessKey::IDE_PROCESS_MONITORING_UP_LEFT));

    programs->push_back(Program(Config::getInstance()->DataModel.apps_config[KOMODO_CONFIG_INDEX].log_2,
                               Config::getInstance()->DataModel.log_viewer_command,
                               QStringList(Config::getInstance()->DataModel.log_viewer_params) << Config::getInstance()->DataModel.apps_config[KOMODO_CONFIG_INDEX].log_2 ,
                               ProcessKey::IDE_PROCESS_MONITORING_UP_RIGHT));

    programs->push_back(Program(Config::getInstance()->DataModel.apps_config[KOMODO_CONFIG_INDEX].log_3,
                               Config::getInstance()->DataModel.log_viewer_command,
                               QStringList(Config::getInstance()->DataModel.log_viewer_params)<< Config::getInstance()->DataModel.apps_config[KOMODO_CONFIG_INDEX].log_3 ,
                               ProcessKey::IDE_PROCESS_MONITORING_DOWN_LEFT));

    programs->push_back(Program(Config::getInstance()->DataModel.apps_config[KOMODO_CONFIG_INDEX].log_4,
                               Config::getInstance()->DataModel.log_viewer_command,
                               QStringList(Config::getInstance()->DataModel.log_viewer_params)<< Config::getInstance()->DataModel.apps_config[KOMODO_CONFIG_INDEX].log_4 ,
                               ProcessKey::IDE_PROCESS_MONITORING_DOWN_RIGHT));

    //Specially for process monitoring

    programs->push_back(Program("Komodo process monitor",
                               Config::getInstance()->DataModel.shell_name,
                               QStringList() << Config::getInstance()->DataModel.apps_config[KOMODO_CONFIG_INDEX].monitoring_script,
                               ProcessKey::IDE_PROCESS_MONITORING_PERFORMANCE));

    Dialogs.insert(monitor);
    monitor->show();

}

void MainWindow::on_actionAgama_triggered()
{
    Monitor* monitor = new Monitor();
    monitor->setWindowTitle("Agama");
    auto programs = monitor->getPrograms();
    programs->push_back(Program(Config::getInstance()->DataModel.apps_config[AGAMA_CONFIG_INDEX].log_1,
                               Config::getInstance()->DataModel.log_viewer_command,
                               QStringList(Config::getInstance()->DataModel.log_viewer_params)<<  Config::getInstance()->DataModel.apps_config[AGAMA_CONFIG_INDEX].log_1,
                               ProcessKey::IDE_PROCESS_MONITORING_UP_LEFT));

    programs->push_back(Program(Config::getInstance()->DataModel.apps_config[AGAMA_CONFIG_INDEX].log_2,
                               Config::getInstance()->DataModel.log_viewer_command,
                               QStringList(Config::getInstance()->DataModel.log_viewer_params)  <<  Config::getInstance()->DataModel.apps_config[AGAMA_CONFIG_INDEX].log_2,
                               ProcessKey::IDE_PROCESS_MONITORING_UP_RIGHT));

    programs->push_back(Program("Agama processes",
                               Config::getInstance()->DataModel.process_id_command,
                               QStringList()  <<  Config::getInstance()->DataModel.apps_config[AGAMA_CONFIG_INDEX].process_name,
                               ProcessKey::IDE_PROCESS_MONITORING_DOWN_LEFT));

    programs->push_back(Program("Komodod processes",
                               Config::getInstance()->DataModel.process_id_command,
                               QStringList()  << Config::getInstance()->DataModel.apps_config[KOMODO_CONFIG_INDEX].process_name  ,
                               ProcessKey::IDE_PROCESS_MONITORING_DOWN_RIGHT));

    //Specially for process monitoring

    programs->push_back(Program("Agama process monitor",
                               Config::getInstance()->DataModel.shell_name,
                               QStringList() << Config::getInstance()->DataModel.apps_config[AGAMA_CONFIG_INDEX].monitoring_script,
                               ProcessKey::IDE_PROCESS_MONITORING_PERFORMANCE));

    Dialogs.insert(monitor);
    monitor->show();
}

void MainWindow::on_actionJumblr_triggered()
{
    //TODO Ahmad
}

void MainWindow::on_actionCoinIssuer_triggered()
{
    CoinIssuer* issuer = new CoinIssuer();

    Dialogs.insert(issuer);
    issuer->show();
}

void MainWindow::on_startButton_clicked()
{
    if(!Utility::fileExists(Config::getInstance()->DataModel.apps_config[KOMODO_CONFIG_INDEX].program_path))
    {
        QMessageBox::question(nullptr,
                              "Error",
                              "Can not find" + Config::getInstance()->DataModel.apps_config[KOMODO_CONFIG_INDEX].program_path,
                              QMessageBox::Close);
    }
    if(KomodoStarted)
    {
        if(execute(ProcessKey::IDE_PROCESS_KOMODOD_STOP))
        {
            KomodoStarted = false;
            ui->startButton->setText("Start Komodo");
        }
    }
    else
    {
        if(execute(ProcessKey::IDE_PROCESS_KOMODOD_START))
        {
            KomodoStarted = true;
            ui->startButton->setText("Stop Komodo");
        }
    }

}

void MainWindow::on_getInfoButton_clicked()
{
    execute(ProcessKey::IDE_PROCESS_GET_INFO);
}

void MainWindow::on_getPubKeyButton_clicked()
{
    if(ui->addressLineEdit->text().isEmpty())
    {
        QMessageBox msg;
        msg.setText("Please enter your address first!!");
        msg.exec();
        return;
    }

    Program* program = getProgram(ProcessKey::IDE_PROCESS_GET_PUBLIC_KEY);
    Q_ASSERT(program);

    program->Arguments = QStringList() << "validateaddress" << ui->addressLineEdit->text();

    execute(ProcessKey::IDE_PROCESS_GET_PUBLIC_KEY);
}

void MainWindow::on_getPrivKeyButton_clicked()
{
    if(ui->addressLineEdit->text().isEmpty())
    {
        QMessageBox msg;
        msg.setText("Please enter your wallet address first!!");
        msg.exec();
        return;
    }

    Program* program = getProgram(ProcessKey::IDE_PROCESS_GET_PRIVATE_KEY);
    Q_ASSERT(program);

    program->Arguments = QStringList() << "dumpprivkey" << ui->addressLineEdit->text();

    execute(ProcessKey::IDE_PROCESS_GET_PRIVATE_KEY);
}

void MainWindow::on_insertPrivKeyButton_clicked()
{
    if(ui->privKeyLineEdit->text().isEmpty())
    {
        QMessageBox msg;
        msg.setText("Please enter your private key first!!");
        msg.exec();
        return;
    }

    Program* program = getProgram(ProcessKey::IDE_PROCESS_INSERT_PRIVATE_KEY);
    Q_ASSERT(program);

    program->Arguments = QStringList() << "importprivkey" << ui->privKeyLineEdit->text();

    execute(ProcessKey::IDE_PROCESS_INSERT_PRIVATE_KEY);
}

Program* MainWindow::getProgram(ProcessKey key)
{
    auto program = std::find_if(Programs.begin(),Programs.end(),[key](Program* program){return program->Process->getProcessKey() == key;});
    if(program != Programs.end())
        return *program;

    return nullptr;
}

Program* MainWindow::removeProcess(ProcessKey key)
{
    auto program = std::find_if(Programs.begin(),Programs.end(),[key](Program* program){return program->Process->getProcessKey() == key;});
    if(program != Programs.end())
    {
       Programs.erase(program);
    }

    return nullptr;
}

bool MainWindow::execute(ProcessKey key)
{
    Program* program = getProgram(key);
    Q_ASSERT(program);
    if(!program)return false;

    if(program->Process)delete program->Process;
    program->Process = new CustomProcess();
    program->Process->setProcessKey(key);

    QTextBrowser* textBrowser = nullptr;
    findTextBrowser(key, textBrowser);
    if(!textBrowser)
        return false;
    textBrowser->append(program->Label + "...   ");

    bool result =connect (program->Process, SIGNAL(readyReadStandardOutput()), this, SLOT(PrintOutPut()));
    Q_ASSERT(result);
    result =connect (program->Process, SIGNAL(readyReadStandardError()), this, SLOT(PrintError()));
    Q_ASSERT(result);
    result =connect (program->Process, SIGNAL(finished(int)), this, SLOT(processFinished(int)));
    Q_ASSERT(result);

    program->Process->start(program->Command, program->Arguments);
    if(!program->Process->waitForStarted(100) || !result)
    {
        QMessageBox msg;
        msg.setText("Error: Can not start process!!!");
        msg.exec();
        return false;
    }
    return true;
}
void MainWindow::processFinished(int exitCode)
{

    CustomProcess* process = reinterpret_cast<CustomProcess*>(sender());
    QTextBrowser* textBrowser = nullptr;
    findTextBrowser(process->getProcessKey(), textBrowser);
    if(!textBrowser)
        return;
    Program* program = getProgram(static_cast<ProcessKey>(process->getProcessKey()));


    if(exitCode == 0)
        textBrowser->append("<font color=\"green\">" + program->Label+ QString(" finished successfully").arg(exitCode) + "</font>");
    else
        textBrowser->append("<font color=\"red\">" + program->Label+ QString(" finished with error code %1").arg(exitCode) + "</font>");

    if(process->getProcessKey() == ProcessKey::IDE_PROCESS_INSTALL_KOMODO)
    {
        installing(false,exitCode);
    }

    textBrowser->verticalScrollBar()->setValue(textBrowser->verticalScrollBar()->maximum());

}

void MainWindow::installing(bool isInstalling, int exitCode)
{
    if(isInstalling)
    {
        ui->textBrowser2->append("don't close the software! \nInstalling komodo . . . . .");
        this->setEnabled(false);

    }
    else
    {
        this->setEnabled(true);

        if(exitCode == 0)
            ui->textBrowser2->append("Komodo successfully installed!");
        else
            ui->textBrowser2->append("Installation terminated due some errors!!");
    }
}
void MainWindow::PrintError()
{
    CustomProcess* process = reinterpret_cast<CustomProcess*>(sender());
    QTextBrowser* textBrowser = nullptr;
    findTextBrowser(process->getProcessKey(), textBrowser);
    if(!textBrowser)
        return;

    QByteArray byteArray = process->readAllStandardError();
    QStringList strLines = QString(byteArray).split("\n");
    for (QString line: strLines){
        textBrowser->append("<font color=\"#FBAD35\">" + line + "</font>");
    }

    textBrowser->verticalScrollBar()->setValue(textBrowser->verticalScrollBar()->maximum());
}

void MainWindow::PrintOutPut()
{
    CustomProcess* process = reinterpret_cast<CustomProcess*>(sender());
    QTextBrowser* textBrowser = nullptr;
    findTextBrowser(process->getProcessKey(), textBrowser);
    if(!textBrowser)
        return;

    QByteArray byteArray = process->readAllStandardOutput();
    QStringList strLines = QString(byteArray).split("\n");

    for (QString line: strLines){
        textBrowser->append(line);
        textBrowser->verticalScrollBar()->setValue(textBrowser->verticalScrollBar()->maximum());
    }
    textBrowser->verticalScrollBar()->setValue(textBrowser->verticalScrollBar()->maximum());
}

void MainWindow::findTextBrowser(ProcessKey processKey, QTextBrowser* &textBrowser)
{
    if(processKey == ProcessKey::IDE_PROCESS_KOMODOD_START ||
       processKey == ProcessKey::IDE_PROCESS_KOMODOD_STOP )

        textBrowser = ui->textBrowser2;
    else
        textBrowser = ui->textBrowser;
}

void MainWindow::initializePrograms()
{
    Program* program = new Program("Komodo start",
                                   Config::getInstance()->DataModel.apps_config[KOMODO_CONFIG_INDEX].program_path,
                                   QStringList());

    program->Process = new CustomProcess();
    program->Process->setProcessKey(ProcessKey::IDE_PROCESS_KOMODOD_START);
    Programs.push_back(program);

    program = new Program("Komodo stop",
                                   Config::getInstance()->DataModel.clis_conifg[KOMODO_CLI_CONFIG_INDEX].program_path,
                                   QStringList()  << "stop" );
    program->Process = new CustomProcess();
    program->Process->setProcessKey(ProcessKey::IDE_PROCESS_KOMODOD_STOP);
    Programs.push_back(program);


    program = new Program("getting info",
                                   Config::getInstance()->DataModel.clis_conifg[KOMODO_CLI_CONFIG_INDEX].program_path,
                                   QStringList()  << "getinfo");
    program->Process = new CustomProcess();
    program->Process->setProcessKey(ProcessKey::IDE_PROCESS_GET_INFO);
    Programs.push_back(program);


    program = new Program("getting private key",
                                   Config::getInstance()->DataModel.clis_conifg[KOMODO_CLI_CONFIG_INDEX].program_path,
                                   QStringList());
    program->Process = new CustomProcess();
    program->Process->setProcessKey(ProcessKey::IDE_PROCESS_GET_PRIVATE_KEY);
    Programs.push_back(program);


    program = new Program("getting public key",
                                   Config::getInstance()->DataModel.clis_conifg[KOMODO_CLI_CONFIG_INDEX].program_path,
                                   QStringList());
    program->Process = new CustomProcess();
    program->Process->setProcessKey(ProcessKey::IDE_PROCESS_GET_PUBLIC_KEY);
    Programs.push_back(program);

    program = new Program("Inserting private key",
                                   Config::getInstance()->DataModel.clis_conifg[KOMODO_CLI_CONFIG_INDEX].program_path,
                                   QStringList());
    program->Process = new CustomProcess();
    program->Process->setProcessKey(ProcessKey::IDE_PROCESS_INSERT_PRIVATE_KEY);
    Programs.push_back(program);

    program = new Program("Installing Komodo",
                                   Config::getInstance()->DataModel.shell_name,
                                   QStringList() << Config::getInstance()->DataModel.apps_config[KOMODO_CONFIG_INDEX].install_script);

    program->Process = new CustomProcess();
    program->Process->setProcessKey(ProcessKey::IDE_PROCESS_INSTALL_KOMODO);
    Programs.push_back(program);

    program = new Program("UnInstalling Komodo",
                                   Config::getInstance()->DataModel.shell_name,
                                   QStringList() << Config::getInstance()->DataModel.apps_config[KOMODO_CONFIG_INDEX].uninstall_script);

    program->Process = new CustomProcess();
    program->Process->setProcessKey(ProcessKey::IDE_PROCESS_UNINSTALL_KOMODO);
    Programs.push_back(program);

    program = new Program("Cleaning Komodo",
                                   Config::getInstance()->DataModel.shell_name,
                                   QStringList() << "./bin/clean");

    program->Process = new CustomProcess();
    program->Process->setProcessKey(ProcessKey::IDE_PROCESS_CLEAN_KOMODO);
    Programs.push_back(program);

    program = new Program("Kill Komodo",
                                   Config::getInstance()->DataModel.shell_name,
                                   QStringList() << Config::getInstance()->DataModel.apps_config[KOMODO_CONFIG_INDEX].kill_script);

    program->Process = new CustomProcess();
    program->Process->setProcessKey(ProcessKey::IDE_PROCESS_KILL_KOMODO);
    Programs.push_back(program);

}

void MainWindow::releasePrograms()
{
    for(auto program : Programs)
        if(program)
        {
            if(program->Process)
                program->Process->kill();

            delete program;            
            program = nullptr;
        }
}

void MainWindow::resizeEvent(QResizeEvent *event)
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

void MainWindow::on_KillButton_clicked()
{
    execute(ProcessKey::IDE_PROCESS_KILL_KOMODO);
}

void MainWindow::on_actionInstall_triggered()
{
    if(execute(ProcessKey::IDE_PROCESS_INSTALL_KOMODO))
    {
        installing(true);
    }
}

void MainWindow::on_actionUninstall_triggered()
{    
    if(Utility::showMessage("Are you sure you want to Uninstall Komodo?"))
    execute(ProcessKey::IDE_PROCESS_UNINSTALL_KOMODO);
}

void MainWindow::on_actionCleanKomodo_triggered()
{
    if(Utility::showMessage("Are you sure you want to delete all the komodo's data?"))
        execute(ProcessKey::IDE_PROCESS_CLEAN_KOMODO);
}

void MainWindow::on_actionGetInfo_triggered()
{
    execute(ProcessKey::IDE_PROCESS_GET_INFO);
}



void MainWindow::on_actionKomodo_CLI_triggered()
{
    CliGui* komodoCliGui = new CliGui();
    komodoCliGui->setProgramName(Config::getInstance()->DataModel.clis_conifg[KOMODO_CLI_CONFIG_INDEX].program_path);
    komodoCliGui->setCommandFilePath(Config::getInstance()->DataModel.clis_conifg[KOMODO_CLI_CONFIG_INDEX].commands_file);
    komodoCliGui->show();
    Dialogs.insert(komodoCliGui);

}

void MainWindow::on_actionMarketMaker_triggered()
{
    CliGui* marketMaker = new CliGui();
    marketMaker->setProgramName(Config::getInstance()->DataModel.clis_conifg[MARKETMAKER_CONFIG_INDEX].program_path);
    marketMaker->setCommandFilePath(Config::getInstance()->DataModel.clis_conifg[MARKETMAKER_CONFIG_INDEX].commands_file);
    marketMaker->show();
    Dialogs.insert(marketMaker);
}


