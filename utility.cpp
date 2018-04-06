#include "utility.h"
#include<QTime>
#include<QCoreApplication>
#include<QMessageBox>
#include<QFileInfo>

Utility::Utility()
{

}

void Utility::delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

bool Utility::showMessage(QString message)
{
      return QMessageBox::question(nullptr,
                                    "Warning",
                                    message,
                                    QMessageBox::Yes|QMessageBox::No)
                                    == QMessageBox::Yes;
}


bool Utility::fileExists(QString path) {
    QFileInfo check_file(path);
    if (check_file.exists() && check_file.isFile())
    {
        return true;
    }
    else
    {
        return false;
    }
}
