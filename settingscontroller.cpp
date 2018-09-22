#include "settingscontroller.h"

settingsController::settingsController()
{
    pControl = new pathController();

    // Connections

    // pathController folderpaths related..
    connect(pControl,&pathController::listUpdated,this,&settingsController::sendPathToMainWindow);
    connect(pControl,&pathController::updateList,this,&settingsController::sendPathsToMainWindow);
    connect(pControl,&pathController::listUpdated,this,&settingsController::processPath);
    connect(pControl,&pathController::updateList,this,&settingsController::processPaths);
}
