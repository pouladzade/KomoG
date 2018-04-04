#ifndef COINISSUER_H
#define COINISSUER_H

#include <QWidget>
#include<QProcess>

#include"customprocess.h"

namespace Ui {
class CoinIssuer;
}

class CoinIssuer : public QWidget
{
    Q_OBJECT

public:
    explicit CoinIssuer(QWidget *parent = 0);
    ~CoinIssuer();

protected:
    QString getProcessName(ProcessKey key);

    void resizeEvent(QResizeEvent *event);
private slots:
    void on_startButton_clicked();
    void on_isKmdRunButton_clicked();
    void PrintOutPut();
    void PrintError();
    void processFinished(int exitCode);

private:
    Ui::CoinIssuer *ui;
    CustomProcess *IssuerProcess;
    CustomProcess *IsKomodoRunning;
};

#endif // COINISSUER_H
