#pragma once

#include "Scene.h"
#include "Game.h"
#include "CartCrazeListener.h"
#include "PlayerFacing.h"
#include "PlayerStats.h"
#include "ButtonSelect.h"
#include <string>

class CartCraze : public Scene
{
public:
    CartCraze(std::string name);

    void InitScene(float windowWidth, float windowHeight) override;

    void Update() override;

    void GUI() override;

    void GUIWindowUI();
    void GUIWindowOne();
    void GUIWindowTwo();


    //Input overrides
    void KeyboardHold() override;
    void KeyboardDown() override;
    void KeyboardUp() override;

    std::vector<int> projectileStorage;
    std::vector<int> mainMenuButtons;
    std::vector<int> characterSelectButtons;
    std::vector<int> enemyStorage;
    std::vector<int> healthSpriteStorage;

    bool gameRun = false;
    bool gameOver = false;
    bool gameWin = false;
    bool mainMenu = true;
    bool characterMenu = false;
    bool controlsMenu = false;

protected:
    bool m_firstWindow = false;
    bool m_secondWindow = false;

    std::string m_fileInput;



    CartCrazeListener listener;

    int seedEntity = -1;
    int peelEntity = -1;
    int juiceEntity = -1;
    int healthEntity = -1;

    //Menu camera target stuff

    unsigned mainMenuCamera = -1;
    unsigned controlsMenuCamera = -1;
    unsigned characterMenuCamera = -1;
    unsigned victoryScreenCamera = -1;
    unsigned gameOverCamera = -1;

    int puzzleWall1;
    int puzzleWall2;
    int climb;
};