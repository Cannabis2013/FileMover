#include "settingscontroller.h"

settingsController::settingsController()
{
    pControl = new pathController();

    // Connections

    // pathController folderpaths related..
    connect(pControl,&pathController::updateList,this,&settingsController::sendPathToMainWindow);
    connect(pControl,&pathController::updateLists,this,&settingsController::sendPathsToMainWindow);
    connect(pControl,&pathController::updateList,this,&settingsController::processPath);
    connect(pControl,&pathController::updateLists,this,&settingsController::processPaths);
}
