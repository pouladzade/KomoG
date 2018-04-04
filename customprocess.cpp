#include "customprocess.h"

CustomProcess::CustomProcess(QObject *parent) : QProcess(parent)
{

}

CustomProcess::~CustomProcess()
{

}

ProcessKey CustomProcess::getProcessKey() const
{
    return Pkey;
}

void CustomProcess::setProcessKey(ProcessKey key)
{
    Pkey = key;
}
