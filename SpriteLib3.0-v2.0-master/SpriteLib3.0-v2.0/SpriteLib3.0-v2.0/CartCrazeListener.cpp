#include "CartCrazeListener.h"
#include "CartCraze.h"
#include "ECS.h"
#include "Utilities.h"

CartCrazeListener::CartCrazeListener()
	: b2ContactListener()
{

}

void CartCrazeListener::BeginContact(b2Contact* contact)
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
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

	//Player and ground contact check, for jumping.
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


	//Player and enemy contact check, for determining if player has taken damage.
	if ((filterA.categoryBits == ENEMY && filterB.categoryBits == PLAYER) || (filterB.categoryBits == ENEMY && filterA.categoryBits == PLAYER))
	{
		if (filterA.categoryBits == PLAYER && ECS::GetComponent<EnemyStats>((int)fixtureB->GetBody()->GetUserData()).isStunned == false)
		{
			ECS::GetComponent<PlayerStats>((int)fixtureA->GetBody()->GetUserData()).health -= 1;
		}
		else if (filterB.categoryBits == PLAYER && ECS::GetComponent<EnemyStats>((int)fixtureA->GetBody()->GetUserData()).isStunned == false)
		{
			ECS::GetComponent<PlayerStats>((int)fixtureB->GetBody()->GetUserData()).health -= 1;
		}
	}

	//Seed projectile collision check. For the watermelon. Yes, it's kinda sorta an odd system.
	if ((filterA.categoryBits == PROJECTILE) || ( filterB.categoryBits == PROJECTILE))
	{
		if (filterA.categoryBits == GROUND)
		{
			ECS::GetComponent<ProjectileCollision>((int)fixtureB->GetBody()->GetUserData()).hasCollided = true;
		}
		else if (filterB.categoryBits == GROUND)
		{
			ECS::GetComponent<ProjectileCollision>((int)fixtureA->GetBody()->GetUserData()).hasCollided = true;
		}
		else if (filterA.categoryBits == ENEMY)
		{
			ECS::GetComponent<ProjectileCollision>((int)fixtureB->GetBody()->GetUserData()).hasCollided = true;
			ECS::GetComponent<EnemyStats>((int)fixtureA->GetBody()->GetUserData()).health -= 1;
		}
		else if (filterB.categoryBits == ENEMY)
		{
			ECS::GetComponent<ProjectileCollision>((int)fixtureA->GetBody()->GetUserData()).hasCollided = true;
			ECS::GetComponent<EnemyStats>((int)fixtureB->GetBody()->GetUserData()).health -= 1;
		}
	}

	//Juice projectile collision check. For the apple.
	if ((filterA.categoryBits == FRIENDLY) || (filterB.categoryBits == FRIENDLY))
	{
		if (filterA.categoryBits == GROUND)
		{
			ECS::GetComponent<ProjectileCollision>((int)fixtureB->GetBody()->GetUserData()).hasCollided = true;
		}
		else if (filterB.categoryBits == GROUND)
		{
			ECS::GetComponent<ProjectileCollision>((int)fixtureA->GetBody()->GetUserData()).hasCollided = true;
		}
		else if (filterA.categoryBits == ENEMY)
		{
			ECS::GetComponent<ProjectileCollision>((int)fixtureB->GetBody()->GetUserData()).hasCollided = true;
			ECS::GetComponent<EnemyStats>((int)fixtureA->GetBody()->GetUserData()).isStunned = true;
		}
		else if (filterB.categoryBits == ENEMY)
		{
			ECS::GetComponent<ProjectileCollision>((int)fixtureA->GetBody()->GetUserData()).hasCollided = true;
			ECS::GetComponent<EnemyStats>((int)fixtureB->GetBody()->GetUserData()).isStunned = true;
		}
	}
	
	//Peel mine collision check. For the banana.
	if ((filterA.categoryBits == MINE) || (filterB.categoryBits == MINE))
	{
		if (filterA.categoryBits == GROUND)
		{
			ECS::GetComponent<ProjectileCollision>((int)fixtureB->GetBody()->GetUserData()).isStuck = true;
		}
		else if (filterB.categoryBits == GROUND)
		{
			ECS::GetComponent<ProjectileCollision>((int)fixtureA->GetBody()->GetUserData()).isStuck = true;
		}
		else if (filterA.categoryBits == ENEMY)
		{
			ECS::GetComponent<ProjectileCollision>((int)fixtureB->GetBody()->GetUserData()).hasCollided = true;
			ECS::GetComponent<EnemyStats>((int)fixtureA->GetBody()->GetUserData()).isStunned = true;
		}
		else if (filterB.categoryBits == ENEMY)
		{
			ECS::GetComponent<ProjectileCollision>((int)fixtureA->GetBody()->GetUserData()).hasCollided = true;
			ECS::GetComponent<EnemyStats>((int)fixtureB->GetBody()->GetUserData()).isStunned = true;
		}
	}

	//Finish line check
	if ((filterA.categoryBits == FINISH) || (filterB.categoryBits == FINISH))
	{
		if (filterA.categoryBits == PLAYER)
		{
			ECS::GetComponent<PlayerStats>((int)fixtureA->GetBody()->GetUserData()).playerWon = true;
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
		}
		else if (sensorB)
		{
			TriggerExit(fixtureB);
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
