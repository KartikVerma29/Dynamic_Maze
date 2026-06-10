#include "maze/generation/RecursiveBacktrackerGenerator.h"
#include "maze/mutation/GridMazeMutator.h"
#include "maze/solvability/BFSSolvabilityChecker.h"
#include "modes/DarkMazeMode.h"
#include "modes/DynamicMazeMode.h"
#include "modes/GauntletMode.h"
#include "modes/IAppState.h"
#include "scoring/DynamicMazeScoreCalculator.h"
#include "scoring/DarkMazeScoreCalculator.h"
#include "scoring/GauntletScoreCalculator.h"
#include "renderer/RaylibRenderer.h"
#include "ui/UIManager.h"
#include <bits/stdc++.h>
#include "modes/MainMenuMode.h"
#include "modes/SettingsMode.h"
#include "loop/GameLoop.h"

using namespace std;

#include "events/EventManager.h"

int main(){

   cout<<"1. Creating EventManger\n"<<flush;
   EventManager eventManager;

   cout<<"2. Creating Renderer\n"<<flush;
   RaylibRenderer renderer(1080,720, "Dynamic Maze");

   cout<<"3. Creating UIManager\n"<<flush;
   UIManager uiManager(eventManager);

   cout<<"4. Creating generator\n"<<flush;
   RecursiveBacktrackerGenerator generator;

   cout<<"5. Creating checker\n"<<flush;
   BFSSolvabilityChecker checker;

   cout<<"6. Creating mutator\n"<<flush;
   GridMazeMutator mutator(checker, eventManager);

   cout<<"7. Creating scorers\n"<<flush;
   DynamicMazeScoreCalculator dynamicScorer;
   DarkMazeScoreCalculator darkScorer;
   GauntletScoreCalculator gauntletScorer;

   cout<<"8. Creating modes\n"<<flush;
   DynamicMazeMode dynamicMode(uiManager, mutator, dynamicScorer, eventManager, generator);
   DarkMazeMode darkMode(uiManager,mutator, darkScorer, eventManager, generator);
   GauntletMode gauntletMode(uiManager ,mutator, gauntletScorer, eventManager, generator);
   MainMenuMode mainMenu(uiManager);
   SettingsMode settings(uiManager);

   cout<<"9. Starting gameloop\n"<<flush;
   GameLoop gameLoop(renderer, eventManager, uiManager);
   gameLoop.getModeManager().registerState(AppStateType::MAINMENU, &mainMenu);
   gameLoop.getModeManager().registerState(AppStateType::DYNAMIC, &dynamicMode);
   gameLoop.getModeManager().registerState(AppStateType::DARK, &darkMode);
   gameLoop.getModeManager().registerState(AppStateType::GAUNTLET, &gauntletMode);
   gameLoop.getModeManager().registerState(AppStateType::SETTINGS, &settings);

   gameLoop.getModeManager().transitionTo(AppStateType::MAINMENU);
   gameLoop.run();

   return 0;
}
