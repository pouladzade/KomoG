#ifndef MONITOR_H
#define MONITOR_H

#include <QWidget>
#include"customprocess.h"
#include"program.h"
#include<set>

class QTextBrowser;
class QLabel;

namespace Ui {
class Monitor;
}

class Monitor : public QWidget
{
    Q_OBJECT

public:
    explicit Monitor(QWidget *parent = 0);
    ~Monitor();

    std::vector<Program>* getPrograms();

protected:
    void resizeEvent(QResizeEvent *event);
    void findTextBrowser(ProcessKey key, QTextBrowser* &textBrowser);
    void findLabel(ProcessKey key, QLabel* &lable);

    Program& getProgram(ProcessKey key);
    void closeEvent(QCloseEvent *event);
protected slots:
    void PrintError();
    void PrintOutPut();

    void processFinished(int exitCode);
private slots:
    void on_startButton_clicked();

    void on_stopButton_clicked();

    void on_closeButton_clicked();

private:
    Ui::Monitor *ui;

    std::vector<Program> Programs;
    std::set<QWidget*> WidgetSet;
    bool Started;

};

#endif // MONITOR_H
