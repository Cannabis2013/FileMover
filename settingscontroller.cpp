#include "settingscontroller.h"

settingsController::settingsController()
{
    pControl = new pathController();

    // Connections

}

void settingsController::insertPath(QString path)
{
    emit processPath(path);
    pControl->insertPath(path);
}

void settingsController::insertPaths(QStringList paths)
{
    emit processPaths(paths);
    pControl->insertPaths(paths);
}
