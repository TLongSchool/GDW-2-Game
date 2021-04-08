#include "LadderTrigger.h"
#include "ECS.h"
#include "CartCraze.h"

void LadderTrigger::OnTrigger()
{
	Trigger::OnTrigger();
}

void LadderTrigger::OnEnter()
{
	auto& canClimb = ECS::GetComponent<CanClimb>(MainEntities::MainPlayer());

	canClimb.m_canClimb = true;

	Trigger::OnEnter();
}

void LadderTrigger::OnExit()
{
	auto& canClimb = ECS::GetComponent<CanClimb>(MainEntities::MainPlayer());

	canClimb.m_canClimb = false;

	Trigger::OnExit();
}
