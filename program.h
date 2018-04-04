#ifndef PROGRAM_H
#define PROGRAM_H
#include"customprocess.h"

class Program
{
public:
    Program();
    explicit Program(QString text, QString comm, QStringList args, ProcessKey key);
    explicit Program(QString text, QString comm, QStringList args);
    ~Program();

    CustomProcess* Process;

    QString  Command;

    QStringList Arguments;

    QString Label;

    ProcessKey Pkey;

};

#endif // PROGRAM_H
