#include "LadderTrigger.h"
#include "ECS.h"

void LadderTrigger::OnTrigger()
{
	climb = true;
	Trigger::OnTrigger();
}

void LadderTrigger::OnEnter()
{
	Trigger::OnEnter();
	for (int i = 0; i < m_targetEntities.size(); i++)
	{
		climb = true;
	}
}

void LadderTrigger::OnExit()
{
	climb = false;
	Trigger::OnExit();
}
