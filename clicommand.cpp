#include "clicommand.h"

CliCommand::CliCommand()
{
    Command = "";
}

CliCommand::~CliCommand()
{

}

QString CliCommand::getCommand() const
{
    return Command;
}

void CliCommand::setCommand(const QString &value)
{
    Command = value;
}

auto CliCommand::getInputs() const ->const std::vector<QString>&
{
    return Inputs;
}

size_t CliCommand::getInputsSize()
{
    return Inputs.size();
}

void CliCommand::setInput(const QString &value)
{
    Inputs.push_back(value);
}
