

#include <QApplication>

#include <windows.h>

#include "mainwindow_temp.h"


void GetAppPathW(std::string& Path); 
void ReadConfig(std::string configfilepath);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    std::string configfile = "";
    GetAppPathW(configfile);

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

}

