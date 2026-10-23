#include "main.h"


int main(){

   std::cout<<"1. Creating EventManger\n"<<std::flush;
   EventManager eventManager;

   std::cout<<"2. Creating Renderer\n"<<std::flush;
   RaylibRenderer renderer(1280,720, "Dynamic Maze");
   eventManager.subscribe<WallTelegraphEvent>(renderer);

   std::cout<<"3. Creating UIManager\n"<<std::flush;
   UIManager uiManager;

   std::cout<<"4. Creating generator\n"<<std::flush;
   RecursiveBacktrackerGenerator generator;
   PrimsGenerator primGenerator;

   //Maze Post Processoring
   MazeBraider gaunletBraider(0.65f);
   MazeBraider darkBraider(0.5f);
   MazeBraider dynamicBraider(0.25f);

   std::cout<<"5. Creating checker\n"<<std::flush;
   BFSSolvabilityChecker checker;

   std::cout<<"6. Creating mutator\n"<<std::flush;
   GridMazeMutator mutator(checker, eventManager);

   std::cout<<"7. Creating scorers\n"<<std::flush;
   DynamicMazeScoreCalculator dynamicScorer;
   DarkMazeScoreCalculator darkScorer;
   GauntletScoreCalculator gauntletScorer;

   std::cout<<"8. Creating modes\n"<<std::flush;
   DynamicMazeMode dynamicMode(uiManager, mutator, dynamicScorer, eventManager, generator, dynamicBraider);
   DarkMazeMode darkMode(uiManager,mutator, darkScorer, eventManager, generator, darkBraider);

// GauntletMode
   Manhattan manhattan;
   AStarPathfinder finder(manhattan);
   GauntletMode gauntletMode(uiManager ,mutator, gauntletScorer, eventManager,primGenerator, finder, gaunletBraider);

   MainMenuMode mainMenu(uiManager);
   SettingsMode settings(uiManager);

   std::cout<<"9. Starting gameloop\n"<<std::flush;
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
