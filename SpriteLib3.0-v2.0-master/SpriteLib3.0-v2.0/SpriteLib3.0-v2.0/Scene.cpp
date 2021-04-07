#include "Scene.h"
#include "Utilities.h"


Scene::Scene(std::string name)
{
	m_physicsWorld = new b2World(m_gravity);
	m_name = name;
}

void Scene::Unload()
{
	if (m_sceneReg != nullptr)
	{
		delete m_sceneReg;
		m_sceneReg = nullptr;
	}

	if (m_physicsWorld != nullptr)
	{
		delete m_physicsWorld;
		m_physicsWorld = nullptr;
	}
}

void Scene::InitScene(float windowWidth, float windowHeight)
{
	//Default scene class init just includes a camera so stuff doesn't immediately crash
	printf("You shouldn't be running this, while initializing this Scene Type.\n");

	//Dynamically allocates the register
	if (m_sceneReg == nullptr)
	{
		m_sceneReg = new entt::registry;
	}

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//Setup Main Camera entity
	{
		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		//ECS::AttachComponent<HorizontalScroll>(entity);
		vec4 temp = vec4(-75.f, 75.f, -75.f, 75.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);
	}

	//Setup new Entity
	{
		//Creates entity
		auto entity = ECS::CreateEntity();
		m_helloWorldSign = entity;

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Sets up components
		std::string fileName = "HelloWorld.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 100, 60);
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.5f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 0.f, 0.f));
	}
}

void Scene::Update()
{
	auto& tempSpr = m_sceneReg->get<Sprite>(m_helloWorldSign);
	
	tempSpr.SetTransparency((0.5 * sin(Timer::time * 3.f)) + 0.5f);
}

void Scene::GUI()
{
	//Does nothin

}

void Scene::AdjustScrollOffset()
{
	float maxSizeX = ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetOrthoSize().y;
	float maxSizeY = ECS::GetComponent<Camera>(MainEntities::MainCamera()).GetOrthoSize().w;

	float playerHalfSize = ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).GetWidth() / 2.f;

	ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetOffset((maxSizeX * BackEnd::GetAspectRatio()) - playerHalfSize);
	ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetOffset(maxSizeY - playerHalfSize);
}

void Scene::CreateCameraEntity(bool mainCamera, float windowWidth, float windowHeight, float left, float right, float bottom, float top, 
									float zNear, float zFar, float aspectRatio, bool vertScroll, bool horizScroll)
{
	//Setup main camera
	{
		//Creates Camera Entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, false);

		ECS::AttachComponent<Camera>(entity);
		if (horizScroll)
		{
			ECS::AttachComponent<HorizontalScroll>(entity);
		}
		if (vertScroll)
		{
			ECS::AttachComponent<VerticalScroll>(entity);
		}

		vec4 temp = vec4(left, right, bottom, top);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, zNear, zFar);

		if (horizScroll)
		{
			ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		}
		if (vertScroll)
		{
			ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		}
	}
}

//Projectile setup

unsigned Scene::CreateSeedProjectile(float posX, float posY) //Setup for the watermelon's seed projectile
{
	auto entity = ECS::CreateEntity();
	//auto player = ECS::GetComponent<PlayerFacing>(MainEntities::MainPlayer());

	//Adding components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<ProjectileCollision>(entity);

	//Setting up components
	std::string fileName = "watermelon_projectile.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 10, 5);
	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(posX, posY, 10.f));

	auto& seedSpr = ECS::GetComponent<Sprite>(entity);
	auto& seedPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 5.f;

	b2Body* seedBody;
	b2BodyDef seedDef;
	seedDef.type = b2_dynamicBody;
	float seedForce = 32000;

	if (ECS::GetComponent<PlayerFacing>(MainEntities::MainPlayer()).isFacingRight == true)
	{
		seedDef.position.Set(posX + 10, posY);
	}
	else
	{
		seedDef.position.Set(posX - 10, posY);
	}

	seedBody = m_physicsWorld->CreateBody(&seedDef);

	seedPhsBody = PhysicsBody(entity, seedBody, float(seedSpr.GetWidth() - shrinkX), float (seedSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PROJECTILE, ENEMY | OBJECTS | ENVIRONMENT | GROUND, 0.f, 0.f); //Makes a circle body

	seedBody->SetFixedRotation(true);
	seedPhsBody.SetRotationAngleDeg(0.f);
	seedPhsBody.SetColor(vec4(0.f, 20.f, 10.f, 0.f));
	seedPhsBody.SetGravityScale(0.2f);
	if (ECS::GetComponent<PlayerFacing>(MainEntities::MainPlayer()).isFacingRight == true)
	{
		seedBody->ApplyLinearImpulseToCenter(b2Vec2(980000.f, 0.f), true);
	}
	else
	{
		seedBody->ApplyLinearImpulseToCenter(b2Vec2(-980000.f, 0.f), true);
	}
	return entity;
}

unsigned Scene::CreateJuiceProjectile(float posX, float posY) //Setup for the apple's juice projectile
{
	auto entity = ECS::CreateEntity();
	//auto player = ECS::GetComponent<PlayerFacing>(MainEntities::MainPlayer());

	//Adding components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<ProjectileCollision>(entity);

	//Setting up components
	std::string fileName = "juiceprojectile.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 20, 5);
	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(posX, posY, 10.f));

	auto& juiceSpr = ECS::GetComponent<Sprite>(entity);
	auto& juicePhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 5.f;

	b2Body* juiceBody;
	b2BodyDef juiceDef;
	juiceDef.type = b2_dynamicBody;
	float juiceForce = 32000;

	if (ECS::GetComponent<PlayerFacing>(MainEntities::MainPlayer()).isFacingRight == true)
	{
		juiceDef.position.Set(posX + 10, posY);
	}
	else
	{
		juiceDef.position.Set(posX - 10, posY);
	}

	juiceBody = m_physicsWorld->CreateBody(&juiceDef);

	juicePhsBody = PhysicsBody(entity, juiceBody, float(juiceSpr.GetWidth() - shrinkX), float (juiceSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, FRIENDLY, ENEMY | OBJECTS | ENVIRONMENT | GROUND, 0.f, 0.f); //Makes a circle body

	juiceBody->SetFixedRotation(true);
	juicePhsBody.SetRotationAngleDeg(0.f);
	juicePhsBody.SetColor(vec4(0.f, 20.f, 10.f, 0.f));
	juicePhsBody.SetGravityScale(0.05f);
	if (ECS::GetComponent<PlayerFacing>(MainEntities::MainPlayer()).isFacingRight == true)
	{
		juiceBody->ApplyLinearImpulseToCenter(b2Vec2(980000.f, 0.f), true);
	}
	else
	{
		juiceBody->ApplyLinearImpulseToCenter(b2Vec2(-980000.f, 0.f), true);
		//ECS::GetComponent<Transform>(entity).SetRotationAngleZ(180.f);
	}
	return entity;
}

unsigned Scene::CreatePeelProjectile(float posX, float posY) //Setup for the banana's peel projectile
{
	auto entity = ECS::CreateEntity();
	//auto player = ECS::GetComponent<PlayerFacing>(MainEntities::MainPlayer());

	//Adding components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<ProjectileCollision>(entity);

	//Setting up components
	std::string fileName = "banana_peel.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 18);
	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(posX, posY, 10.f));

	auto& peelSpr = ECS::GetComponent<Sprite>(entity);
	auto& peelPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 10.f;

	b2Body* peelBody;
	b2BodyDef peelDef;
	peelDef.type = b2_dynamicBody;
	float peelForce = 32000;

	if (ECS::GetComponent<PlayerFacing>(MainEntities::MainPlayer()).isFacingRight == true)
	{
		peelDef.position.Set(posX + 10, posY);
	}
	else
	{
		peelDef.position.Set(posX - 10, posY);
	}

	peelBody = m_physicsWorld->CreateBody(&peelDef);

	peelPhsBody = PhysicsBody(entity, peelBody, float(peelSpr.GetWidth() - shrinkX), float(peelSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, MINE, ENEMY | OBJECTS | ENVIRONMENT | GROUND, 0.f, 0.f); //Makes a circle body

	peelBody->SetFixedRotation(true);
	peelPhsBody.SetRotationAngleDeg(0.f);
	peelPhsBody.SetColor(vec4(0.f, 20.f, 10.f, 0.f));
	peelPhsBody.SetGravityScale(0.9f);
	if (ECS::GetComponent<PlayerFacing>(MainEntities::MainPlayer()).isFacingRight == true)
	{
		peelBody->ApplyLinearImpulseToCenter(b2Vec2(980000.f, 980000.f), true);
	}
	else
	{
		peelBody->ApplyLinearImpulseToCenter(b2Vec2(-980000.f, 980000.f), true);
	}
	return entity;
}

unsigned Scene::CreateCookieEnemy(float posX, float posY)
{
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	ECS::AttachComponent<EnemyStats>(entity);
	//ECS::AttachComponent<Trigger*>(entity);

	//Sets up components
	std::string fileName = "evil_cookie_static.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 25);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(posX, posY, 3.f));
	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	ECS::GetComponent<EnemyStats>(entity).health = 3;

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(float32(posX), float32(posY));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(25.f - shrinkX), float(25.f - shrinkY), vec2(0.f, 0.f), false, ENEMY, PLAYER | GROUND | PROJECTILE | FRIENDLY);
	tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));

	return entity;
}

unsigned Scene::CreateIcecreamEnemy(float posX, float posY)
{
	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);
	ECS::AttachComponent<PhysicsBody>(entity);
	//ECS::AttachComponent<Trigger*>(entity);

	//Sets up components
	std::string fileName = "evil_cookie_static.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 25);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(posX, posY, 3.f));
	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
	//ECS::GetComponent<Trigger*>(entity) = new EnemyTrigger();
	//ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
	//ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(entity);
	//EnemyTrigger* temp = (EnemyTrigger*)ECS::GetComponent<Trigger*>(entity);

	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

	float shrinkX = 0.f;
	float shrinkY = 0.f;
	b2Body* tempBody;
	b2BodyDef tempDef;
	tempDef.type = b2_dynamicBody;
	tempDef.position.Set(float32(posX), float32(posY));

	tempBody = m_physicsWorld->CreateBody(&tempDef);

	tempPhsBody = PhysicsBody(entity, tempBody, float(25.f - shrinkX), float(25.f - shrinkY), vec2(0.f, 0.f), false, ENEMY, PLAYER | GROUND | PROJECTILE | FRIENDLY);
	tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));

	return entity;
}

entt::registry* Scene::GetScene() const
{
	return m_sceneReg;
}

void Scene::SetScene(entt::registry& scene)
{
	m_sceneReg = &scene;
}

vec4 Scene::GetClearColor() const
{
	return m_clearColor;
}

void Scene::SetClearColor(vec4 clearColor)
{
	m_clearColor = clearColor;
}

std::string Scene::GetName() const
{
	return m_name;
}

void Scene::SetName(std::string name)
{
	m_name = name;
}

b2Vec2 Scene::GetGravity() const
{
	return m_gravity;
}

void Scene::SetGravity(b2Vec2 grav)
{
	m_gravity = grav;
}

b2World & Scene::GetPhysicsWorld()
{
	return *m_physicsWorld;
}

void Scene::SetWindowSize(float windowWidth, float windowHeight)
{
	//TODO: Find new way to get the main camera
	auto& tempCam = m_sceneReg->get<Camera>(MainEntities::MainCamera());
	
	tempCam.SetWindowSize(vec2(windowWidth, windowHeight));
	tempCam.Orthographic(float(windowWidth / windowHeight), tempCam.GetOrthoSize().x, tempCam.GetOrthoSize().y,
															tempCam.GetOrthoSize().z, tempCam.GetOrthoSize().w,
															tempCam.GetNear(), tempCam.GetFar());
}
