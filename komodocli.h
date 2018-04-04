#ifndef KOMODOCLI_H
#define KOMODOCLI_H

#include <QObject>
#include<vector>
#include<string.h>
#include"clicommand.h"

using namespace std;


class KomodoCli
{
public:
    KomodoCli();
    bool loadCommands(QString path);
    auto getCommands() -> std::vector<CliCommand>&;

protected:
    QStringList extractCommands(QString text);
    bool loadJsonFile(QString path);
private:
    std::vector<CliCommand> Commands;
};

#endif // KOMODOCLI_H
