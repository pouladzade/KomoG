#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "program.h"

#include<QMainWindow>
#include<vector>
#include<QLabel>
#include<QActionGroup>
#include<set>
#include"program.h"

class QTextBrowser;
class CustomProcess;

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void delay(int millisecondsToWait);
    void closeEvent(QCloseEvent *event);
    Program *getProgram(ProcessKey key);
    Program *removeProcess(ProcessKey key);
    bool execute(ProcessKey key);
    void findTextBrowser(ProcessKey processKey, QTextBrowser *&textBrowser);
    void resizeEvent(QResizeEvent *event);
    void installing(bool isInstalling, int exitCode = 0);
    bool showMessage(QString message);
protected slots:
    void PrintOutPut();

    void PrintError();
    void processFinished(int exitCode);

private slots:

    void on_actionBarterDEX_triggered();

    void on_actionKomodo_triggered();

    void on_actionAgama_triggered();

    void on_actionJumblr_triggered();

    void on_actionCoinIssuer_triggered();

    void on_startButton_clicked();

    void on_getInfoButton_clicked();

    void on_getPubKeyButton_clicked();

    void on_getPrivKeyButton_clicked();

    void on_insertPrivKeyButton_clicked();   
    
    void on_KillButton_clicked();

    void on_actionInstall_triggered();

    void on_actionUninstall_triggered();

    void on_actionCleanKomodo_triggered();

    void on_actionGetInfo_triggered();

    void on_actionKomodo_CLI_triggered();

    void on_actionMarketMaker_triggered();

private:
    Ui::MainWindow *ui;
    std::set<QWidget*> Dialogs;
    std::vector<Program*> Programs;
    bool KomodoStarted;
    void initializePrograms();
    void releasePrograms();
};

#endif // MAINWINDOW_H
