#ifndef CLICOMMAND_H
#define CLICOMMAND_H


#include <QObject>
#include<vector>
using namespace std;

class CliCommand
{
public:
    CliCommand();
    ~CliCommand();
    QString getCommand() const;
    void setCommand(const QString &value);

    auto getInputs() const -> const std::vector<QString>&;
    void setInput(const QString &value);
    size_t getInputsSize();
private:
    QString Command;
    std::vector<QString> Inputs;
};

#endif // CLICOMMAND_H
