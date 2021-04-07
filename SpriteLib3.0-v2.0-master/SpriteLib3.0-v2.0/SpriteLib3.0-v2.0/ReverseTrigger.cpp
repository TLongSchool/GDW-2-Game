#include "ReverseTrigger.h"
#include "ECS.h"
#include "Utilities.h"
#include "CartCraze.h"

void ReverseTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void ReverseTrigger::OnEnter()
{
	Trigger::enemyTurn();
}

void ReverseTrigger::OnExit()
{
	Trigger::OnExit();
}
