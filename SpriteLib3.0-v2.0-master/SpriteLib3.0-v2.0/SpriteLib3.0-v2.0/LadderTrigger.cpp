#include "LadderTrigger.h"
#include "ECS.h"
#include "CartCraze.h"
#include "Input.h"

void LadderTrigger::OnTrigger()
{
	insideTrigger = true;
	Trigger::OnTrigger();
}

void LadderTrigger::OnEnter()
{
	insideTrigger = true;
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	float speed = 2.f;

	Trigger::OnEnter();
	player.GetBody()->ApplyForceToCenter(b2Vec2(0.f * speed, 900000000.f), true);

	if (insideTrigger && Input::GetKey(Key::W))
	{
		player.GetBody()->ApplyForceToCenter(b2Vec2(0.f * speed, 900000000.f), true);
	}
}

void LadderTrigger::OnExit()
{
	insideTrigger = false;
	Trigger::OnExit();
}
