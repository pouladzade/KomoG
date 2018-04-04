#include "config.h"
#include<qfile.h>
#include<qtextstream.h>
#include<qjsonobject.h>
#include<qjsonarray.h>
#include<qjsondocument.h>
#include<QDir>
#include<QMessageBox>
#include<QCoreApplication>

Config* Config::JsonConfig = nullptr;
ConfigDataModel Config::DataModel;

Config::Config()
{


}

Config *Config::getInstance()
{
    if(JsonConfig == nullptr)
        JsonConfig = new Config();

    return JsonConfig;
}

bool Config::LoadConfig()
{    
    QFile loadFile(JASON_CONFIG_PATH);

    if (!loadFile.open(QIODevice::ReadOnly))
    {
        QMessageBox::question(nullptr,
                            "Error",
                            "Can not find config.json in bin directory!",
                            QMessageBox::Close);
        return false;
    }
    DataModel.apps_config.reserve(3);
    DataModel.clis_conifg.reserve(2);
    QByteArray jsonData = loadFile.readAll();

    QJsonDocument loadDoc(QJsonDocument::fromJson(jsonData));

    QJsonObject obj = loadDoc.object();

    if (obj.contains("os") && obj["os"].isString())
            DataModel.os = obj["os"].toString();

    if (obj.contains("shell_name") && obj["shell_name"].isString())
            DataModel.shell_name = obj["shell_name"].toString();

    if (obj.contains("process_id_command") && obj["process_id_command"].isString())
            DataModel.process_id_command = obj["process_id_command"].toString();

    if (obj.contains("log_viewer_command") && obj["log_viewer_command"].isString())
            DataModel.log_viewer_command = obj["log_viewer_command"].toString();

    if (obj.contains("log_viewer_params") && obj["log_viewer_params"].isArray()) {
        QJsonArray params = obj["log_viewer_params"].toArray();
        for (size_t i = 0; i < params.size(); ++i)
            DataModel.log_viewer_params << params[i].toString();
    }

    if (obj.contains("programs") && obj["programs"].isArray()) {
        QJsonArray apps = obj["programs"].toArray();

        for (size_t i = 0; i < apps.size(); ++i)
        {
            Apps program;
            QJsonObject appObj = apps[i].toObject();

            if (appObj.contains("name") && appObj["name"].isString())
                    program.name = appObj["name"].toString();

            if (appObj.contains("process_name") && appObj["process_name"].isString())
                    program.process_name = appObj["process_name"].toString();

            if (appObj.contains("program_path") && appObj["program_path"].isString())
                    program.program_path = appObj["program_path"].toString().replace("$HOME",QDir::homePath());

            if (appObj.contains("description") && appObj["description"].isString())
                    program.description = appObj["description"].toString();

            if (appObj.contains("monitoring_script") && appObj["monitoring_script"].isString())
                    program.monitoring_script = appObj["monitoring_script"].toString().replace("$HOME",QDir::homePath());

            if (appObj.contains("working_dir") && appObj["working_dir"].isString())
                    program.working_dir = appObj["working_dir"].toString().replace("$HOME",QDir::homePath());

            if (appObj.contains("install_script") && appObj["install_script"].isString())
                    program.install_script = appObj["install_script"].toString().replace("$HOME",QDir::homePath());

            if (appObj.contains("uninstall_script") && appObj["uninstall_script"].isString())
                    program.uninstall_script = appObj["uninstall_script"].toString().replace("$HOME",QDir::homePath());

            if (appObj.contains("kill_script") && appObj["kill_script"].isString())
                    program.kill_script = appObj["kill_script"].toString().replace("$HOME",QDir::homePath());

            if (appObj.contains("log_1") && appObj["log_1"].isString())
                    program.log_1 = appObj["log_1"].toString().replace("$HOME",QDir::homePath());

            if (appObj.contains("log_2") && appObj["log_2"].isString())
                    program.log_2 = appObj["log_2"].toString().replace("$HOME",QDir::homePath());

            if (appObj.contains("log_3") && appObj["log_3"].isString())
                    program.log_3 = appObj["log_3"].toString().replace("$HOME",QDir::homePath());

            if (appObj.contains("log_4") && appObj["log_4"].isString())
                    program.log_4 = appObj["log_4"].toString().replace("$HOME",QDir::homePath());

            DataModel.apps_config.push_back(program);

        }
    }

    if (obj.contains("CLI_Command") && obj["CLI_Command"].isArray()) {
        QJsonArray clis = obj["CLI_Command"].toArray();

        for (size_t i = 0; i < clis.size(); ++i)
        {
            CLI cli_app;
            QJsonObject cliObj = clis[i].toObject();

            if (cliObj.contains("name") && cliObj["name"].isString())
                    cli_app.name = cliObj["name"].toString();


            if (cliObj.contains("program_path") && cliObj["program_path"].isString()){
                    cli_app.program_path = cliObj["program_path"].toString().replace("$HOME",QDir::homePath());
            }

            if (cliObj.contains("commands_file") && cliObj["commands_file"].isString()){
                    cli_app.commands_file = cliObj["commands_file"].toString().replace("$HOME",QDir::homePath());
            }

            if (cliObj.contains("description") && cliObj["description"].isString())
                    cli_app.description = cliObj["description"].toString();

            if (cliObj.contains("install_script") && cliObj["install_script"].isString()){
                    cli_app.install_script = cliObj["install_script"].toString().replace("$HOME",QDir::homePath());
            }

            if (cliObj.contains("uninstall_script") && cliObj["uninstall_script"].isString()){
                    cli_app.uninstall_script = cliObj["uninstall_script"].toString().replace("$HOME",QDir::homePath());
            }

            if (cliObj.contains("kill_script") && cliObj["kill_script"].isString()){
                    cli_app.kill_script = cliObj["kill_script"].toString().replace("$HOME",QDir::homePath());
            }

            DataModel.clis_conifg.push_back(cli_app);
        }
    }

    return true;
}
