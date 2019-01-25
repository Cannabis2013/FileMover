#include "settingscontroller.h"


void settingsController::insertPath(QString path)
{
    mainFolderPaths << path;
}

void settingsController::insertPaths(QStringList paths)
{
    mainFolderPaths << paths;
}
