#ifndef CLIGUI_H
#define CLIGUI_H

#include <QWidget>
#include<memory>
#include<vector>
#include"komodocli.h"
#include "customprocess.h";

class QLabel;
class QLineEdit;

namespace Ui {
class CliGui;
}

using namespace std;

class CliGui : public QWidget
{
    Q_OBJECT

public:
    explicit CliGui(QWidget *parent = 0);
    ~CliGui();

    QString getProgramName() const;
    void setProgramName(const QString &value);

    QString getCommandFilePath() const;
    void setCommandFilePath(const QString &value);

protected:
    bool loadCommands();
    void showInput(int index, QString text);
    void hideAll(bool hide);

    bool executeCommand(const CliCommand *command);
    void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *event);
    void showEvent(QShowEvent *event);
    void clearAll();
protected slots:
    void PrintError();
    void PrintOutPut();
    void processFinished(int exitCode);
private slots:
    void on_applyButton_clicked();

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::CliGui *ui;
    std::shared_ptr<KomodoCli> Cli;
    std::vector<std::pair<QLabel* ,QLineEdit* >> CommandWidgets;
    CustomProcess* Process;
    CliCommand CurrentCommand;
    QString ProgramName;
    QString CommandFilePath;
};

#endif // CLIGUI_H
