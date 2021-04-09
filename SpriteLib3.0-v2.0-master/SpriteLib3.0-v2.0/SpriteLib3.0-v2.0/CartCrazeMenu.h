#pragma once

#include "Scene.h"
#include <string>

class CartCrazeMenu : public Scene
{
public:
	CartCrazeMenu(std::string name);

	void InitScene(float windowWidth, float windowHeight) override;

	void Update() override;

	//void GUI() override;

	//void GUIWindowUI();
	//void GUIWindowOne();
	//void GUIWindowTwo();


	//Input overrides
	void KeyboardHold() override;
	void KeyboardDown() override;
	void KeyboardUp() override;


protected:
	//bool m_firstWindow = false;
	//bool m_secondWindow = false;

	std::string m_fileInput;
};