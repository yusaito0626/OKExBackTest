

#include <QApplication>

#include <windows.h>

#include "mainwindow.h"
#include "GlobalVariables.h"


void GetAppPathW(std::string& Path); 
void ReadConfig(std::string configfilepath);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    std::string configfile = "";
    GetAppPathW(configfile);
    ReadConfig(configfile);
    w.logfile = std::ofstream(GlobalVariables::OKExBacktest_GUI::outputFilePath + "\\OKExBacktest.log");
    w.UpdateSettingTab();

    return a.exec();
}

void GetAppPathW(std::string& Path)
{
    char FullPath[MAX_PATH];
    char Drive[MAX_PATH];
    char Dir[MAX_PATH];
    char AppPath[MAX_PATH];
    ::GetModuleFileNameA(NULL, FullPath, MAX_PATH);
    _splitpath_s(FullPath, Drive, MAX_PATH, Dir, MAX_PATH, NULL, 0, NULL, 0);
    _makepath_s(AppPath, Drive, Dir, NULL, NULL);
    Path = AppPath;
}

void ReadConfig(std::string configfilepath)
{
    std::string ConfigFullPath = configfilepath + "OKExBacktest.ini";
    std::ifstream ifs(ConfigFullPath);
    std::string line;
    std::string::size_type pos = std::string::npos;
    while (std::getline(ifs, line))
    {
        pos = line.find('=');
        if (pos != std::string::npos)
        {
            std::string variable;
            std::string value;
            variable = line.substr(0, pos);
            value = line.substr(pos + 1);

            if (variable == "calandarFile")
            {
                GlobalVariables::OKExBacktest_GUI::calandarFile = value;
            }
            else if (variable == "masterFilePath")
            {
                GlobalVariables::OKExBacktest_GUI::masterFilePath = value;
            }
            else if (variable == "outputFilePath")
            {
                GlobalVariables::OKExBacktest_GUI::outputFilePath = value;
            }
            else if (variable == "feedFilePath")
            {
                GlobalVariables::OKExBacktest_GUI::feedFilePath = value;
            }
            else if (variable == "paramFile")
            {
                GlobalVariables::OKExBacktest_GUI::paramFile = value;
            }
        }
    }
}

