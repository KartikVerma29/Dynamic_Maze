#include "main.h"
#include "maze/postprocess/MazeBraider.h"

int main(){

   cout<<"1. Creating EventManger\n"<<flush;
   EventManager eventManager;

   cout<<"2. Creating Renderer\n"<<flush;
   RaylibRenderer renderer(1280,720, "Dynamic Maze");

   cout<<"3. Creating UIManager\n"<<flush;
   UIManager uiManager(eventManager);

   cout<<"4. Creating generator\n"<<flush;
   RecursiveBacktrackerGenerator generator;
   PrimsGenerator primGenerator;

   //Maze Post Processoring
   MazeBraider gaunletBraider(0.75f);
   MazeBraider darkBraider(0.5f);
   MazeBraider dynamicBraider(0.25f);

   cout<<"5. Creating checker\n"<<flush;
   BFSSolvabilityChecker checker;

   cout<<"6. Creating mutator\n"<<flush;
   GridMazeMutator mutator(checker, eventManager);

   cout<<"7. Creating scorers\n"<<flush;
   DynamicMazeScoreCalculator dynamicScorer;
   DarkMazeScoreCalculator darkScorer;
   GauntletScoreCalculator gauntletScorer;

   cout<<"8. Creating modes\n"<<flush;
   DynamicMazeMode dynamicMode(uiManager, mutator, dynamicScorer, eventManager, generator, dynamicBraider);
   DarkMazeMode darkMode(uiManager,mutator, darkScorer, eventManager, generator, darkBraider);

// GauntletMode
   Manhattan manhattan;
   AStarPathfinder finder(manhattan);
   GauntletMode gauntletMode(uiManager ,mutator, gauntletScorer, eventManager,primGenerator, finder, gaunletBraider);

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
