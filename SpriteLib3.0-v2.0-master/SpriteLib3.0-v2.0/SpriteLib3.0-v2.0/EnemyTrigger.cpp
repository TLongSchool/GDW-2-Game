#include "EnemyTrigger.h"
#include "ECS.h"
#include "Utilities.h"

void EnemyTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void EnemyTrigger::OnEnter()
{
	Trigger::OnEnter();
	for (int i = 0; i < m_targetEntities.size(); i++)
	{
		MessageBox(NULL, "You have run out of lives!", "Game Over...", MB_OK | MB_ICONINFORMATION);
		exit(0);
	}
}

void EnemyTrigger::OnExit()
{
	Trigger::OnExit();
}
