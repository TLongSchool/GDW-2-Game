#include "CartCrazeListener.h"

#include "ECS.h"

CartCrazeListener::CartCrazeListener()
	: b2ContactListener()
{

}

void CartCrazeListener::BeginContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	bool sensorA = fixtureA->IsSensor();
	bool sensorB = fixtureB->IsSensor();

	//if neither or both are sensors, will be false
	if ((sensorA ^ sensorB))
	{
		if (sensorA)
		{
			TriggerEnter(fixtureA);
		}
		else if (sensorB)
		{
			TriggerEnter(fixtureB);
		}
	}

	b2Filter filterA = fixtureA->GetFilterData();
	b2Filter filterB = fixtureB->GetFilterData();

	// For player jumping
	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == GROUND) || (filterB.categoryBits == PLAYER && filterA.categoryBits == GROUND))
	{
		if (filterA.categoryBits == PLAYER)
		{
			ECS::GetComponent<CanJump>((int)fixtureA->GetBody()->GetUserData()).m_canJump = true;
		}
		else if (filterB.categoryBits == PLAYER)
		{
			ECS::GetComponent<CanJump>((int)fixtureB->GetBody()->GetUserData()).m_canJump = true;
		}
	}

	// For player ladder climbing
	if ((filterA.categoryBits == PLAYER && filterB.categoryBits == LADDER) || (filterB.categoryBits == PLAYER && filterA.categoryBits == LADDER))
	{
		if (filterA.categoryBits == PLAYER)
		{
			ECS::GetComponent<CanClimb>((int)fixtureA->GetBody()->GetUserData()).m_canClimb = true;
		}
		else if (filterB.categoryBits == PLAYER)
		{
			ECS::GetComponent<CanClimb>((int)fixtureB->GetBody()->GetUserData()).m_canClimb = true;
		}
	}

}

void CartCrazeListener::EndContact(b2Contact* contact)
{
	b2Fixture* fixtureA = contact->GetFixtureA();
	b2Fixture* fixtureB = contact->GetFixtureB();

	bool sensorA = fixtureA->IsSensor();
	bool sensorB = fixtureB->IsSensor();

	//if neither or both are sensors, will be false
	if ((sensorA ^ sensorB))
	{
		if (sensorA)
		{
			TriggerExit(fixtureA);
			ECS::GetComponent<CanClimb>((int)fixtureB->GetBody()->GetUserData()).m_canClimb = false;
		}
		else if (sensorB)
		{
			TriggerExit(fixtureB);
			ECS::GetComponent<CanClimb>((int)fixtureB->GetBody()->GetUserData()).m_canClimb = false;
		}
	}
}

void CartCrazeListener::TriggerEnter(b2Fixture* sensor)
{
	int entity = (int)sensor->GetBody()->GetUserData();

	ECS::GetComponent<Trigger*>(entity)->OnEnter();
}

void CartCrazeListener::TriggerExit(b2Fixture* sensor)
{
	int entity = (int)sensor->GetBody()->GetUserData();

	ECS::GetComponent<Trigger*>(entity)->OnExit();
}
