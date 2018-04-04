#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include<QStringList>
#include <vector>

#define AGAMA_CONFIG_INDEX          0
#define KOMODO_CONFIG_INDEX         1
#define BARTERDEX_CONFIG_INDEX      2
#define KOMODO_CLI_CONFIG_INDEX     0
#define MARKETMAKER_CONFIG_INDEX    1
#define JASON_CONFIG_PATH           "./bin/config.json"

using namespace std;

struct Apps
{
    QString name;
    QString process_name;
    QString program_path;
    QString description;
    QString monitoring_script;
    QString working_dir;
    QString install_script;
    QString uninstall_script;
    QString kill_script;
    QString log_1;
    QString log_2;
    QString log_3;
    QString log_4;
};

struct CLI
{
   QString name;
   QString program_path;
   QString commands_file;
   QString description;
   QString install_script;
   QString uninstall_script;
   QString kill_script;
};

struct ConfigDataModel
{
    QString os;
    QString shell_name;
    QString process_id_command;
    QString log_viewer_command;
    QStringList log_viewer_params;
    std::vector<Apps> apps_config;
    std::vector<CLI>  clis_conifg;

};

class Config
{
private:
    Config();


public:
    static Config* getInstance();
    static Config* JsonConfig;
    static ConfigDataModel DataModel;
    static bool LoadConfig();
        
};

#endif // CONFIG_H
