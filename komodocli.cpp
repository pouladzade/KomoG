#include "komodocli.h"
#include<qfile.h>
#include<qtextstream.h>
#include<qjsonobject.h>
#include<qjsonarray.h>
#include<qjsondocument.h>


KomodoCli::KomodoCli()
{

}
bool KomodoCli::loadCommands(QString path)
{
    QFile inputFile(path);
    if (inputFile.open(QIODevice::ReadOnly))
    {
        Commands.clear();

       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          if(line.isEmpty())
              continue;

          CliCommand command;
          QStringList list = extractCommands(line);
          command.setCommand(list.at(0));
          for(size_t index = 1; index < list.size() ; index++)
              command.setInput(list.at(index));

          Commands.push_back(command);
       }

       inputFile.close();
       return true;
    }

    return false;
}

bool KomodoCli::loadJsonFile(QString path)
{
    QFile loadFile(path);

    if (!loadFile.open(QIODevice::ReadOnly))
    {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));

    QJsonObject json = loadDoc.object();


    if (json.contains("commands") && json["commands"].isArray()) {
        QJsonArray commands = json["commands"].toArray();

        for (int commIndex = 0; commIndex < commands.size(); ++commIndex)
        {
            CliCommand command;

            QJsonObject commObj = commands[commIndex].toObject();

            if (commObj.contains("command") && json["command"].isString())
                    command.setCommand(json["command"].toString());

            if (commObj.contains("inputs") && commObj["inputs"].isArray())
            {
                QJsonArray inputs = commObj["inputs"].toArray();
                for(size_t inIndex = 0 ; inIndex < inputs.size(); ++inIndex)
                {
                    command.setInput(inputs.at(inIndex).toString());
                }
            }
        }
    }
}

auto KomodoCli::getCommands() -> std::vector<CliCommand>&
{
    return Commands;
}


QStringList KomodoCli::extractCommands(QString text )
{
    QStringList list = text.split('#', QString::SkipEmptyParts);
    return list;
}

