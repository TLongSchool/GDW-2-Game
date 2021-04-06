#include "MainEntities.h"

int MainEntities::m_mainCamera = 0;
int MainEntities::m_mainPlayer = 0;
int MainEntities::m_mainEnemy = 0;

int MainEntities::MainCamera()
{
	return m_mainCamera;
}

int MainEntities::MainPlayer()
{
	return m_mainPlayer;
}

int MainEntities::MainEnemy()
{
	return m_mainEnemy;
}

void MainEntities::MainCamera(int main)
{
	m_mainCamera = main;
}

void MainEntities::MainPlayer(int main)
{
	m_mainPlayer = main;
}

void MainEntities::MainEnemy(int main)
{
	m_mainEnemy = main;
}