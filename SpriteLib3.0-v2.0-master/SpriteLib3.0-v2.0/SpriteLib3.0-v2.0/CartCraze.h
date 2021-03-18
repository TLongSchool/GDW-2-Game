#pragma once

#include "Scene.h"
#include "CartCrazeListener.h"
#include "PlayerFacing.h"
#include "PlayerStats.h"
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

protected:
	bool m_firstWindow = false;
	bool m_secondWindow = false;

	std::string m_fileInput;

	CartCrazeListener listener;

	int seedEntity = -1;
	int peelEntity = -1;
	int juiceEntity = -1;
	int healthEntity = -1;

	int puzzleWall1;
	int puzzleWall2;
};
