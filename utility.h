#ifndef UTILITY_H
#define UTILITY_H

#include <QObject>

class Utility final
{
public:
    static void delay(int millisecondsToWait);
    static bool showMessage(QString message);
    static bool fileExists(QString path);
private:
    Utility();
};

#endif // UTILITY_H
