#include "program.h"

Program::Program()
{
    Process = nullptr;
}

Program::Program(QString text, QString comm, QStringList args, ProcessKey key)
{
    Process = nullptr;
    Command = comm;
    Arguments = args;
    Label = text;
    Pkey  = key;
}

Program::Program(QString text, QString comm, QStringList args)
{
    Process = nullptr;
    Command = comm;
    Arguments = args;
    Label = text;
}

Program::~Program()
{
    if(Process)
    {
        Process->kill();
        delete Process;
        Process = nullptr;
    }
}
