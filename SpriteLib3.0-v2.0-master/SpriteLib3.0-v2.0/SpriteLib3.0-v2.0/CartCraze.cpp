#include "CartCraze.h"
#include "Utilities.h"
#include "Trigger.h"
#include "LadderTrigger.h"
#include <random>

CartCraze::CartCraze(std::string name)
	: Scene(name)
{
	//No gravity this is a top down scene
	m_gravity = b2Vec2(0.f, -128.f);
	m_physicsWorld->SetGravity(m_gravity);

	m_physicsWorld->SetContactListener(&listener);
}

//Background
void backgroundPic(int x)
{
	/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/

	//Creates entity
	auto entity = ECS::CreateEntity();

	//Add components
	ECS::AttachComponent<Sprite>(entity);
	ECS::AttachComponent<Transform>(entity);

	//Set up the components
	std::string fileName = "CartCrazeBackground.png";
	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 820, 600);
	ECS::GetComponent<Sprite>(entity).SetTransparency(0.9);
	ECS::GetComponent<Transform>(entity).SetPosition(vec3(x, 20.f, 0.f));
}


void CartCraze::InitScene(float windowWidth, float windowHeight)
{
	//Dynamically allocates the register
	m_sceneReg = new entt::registry;

	//Attach the register
	ECS::AttachRegister(m_sceneReg);

	//Sets up aspect ratio for the camera
	float aspectRatio = windowWidth / windowHeight;

	//Setup MainCamera Entity
	{
		/*Scene::CreateCamera(m_sceneReg, vec4(-75.f, 75.f, -75.f, 75.f), -100.f, 100.f, windowWidth, windowHeight, true, true);*/

		//Creates Camera entity
		auto entity = ECS::CreateEntity();
		ECS::SetIsMainCamera(entity, true);

		//Creates new orthographic camera
		ECS::AttachComponent<Camera>(entity);
		ECS::AttachComponent<HorizontalScroll>(entity);
		ECS::AttachComponent<VerticalScroll>(entity);

		vec4 temp = vec4(-200.f, 200.f, -200.f, 200.f);
		ECS::GetComponent<Camera>(entity).SetOrthoSize(temp);
		ECS::GetComponent<Camera>(entity).SetWindowSize(vec2(float(windowWidth), float(windowHeight)));
		ECS::GetComponent<Camera>(entity).Orthographic(aspectRatio, temp.x, temp.y, temp.z, temp.w, -100.f, 100.f);

		//Attaches the camera to vert and horiz scrolls
		ECS::GetComponent<HorizontalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
		ECS::GetComponent<VerticalScroll>(entity).SetCam(&ECS::GetComponent<Camera>(entity));
	}

	//Generate the background
	{
		int xBackDistRight = 410;
		int xBackDistLeft = -410;

		for (int i = 0; i < 25; i++) {

			backgroundPic(xBackDistRight);
			xBackDistRight += 820;

		}

		for (int i = 0; i < 25; i++) {

			backgroundPic(xBackDistLeft);
			xBackDistLeft -= 820;

		}
	}

	//Main Menu
	{
		//Main menu camera target
		{
			auto entity = ECS::CreateEntity();
			mainMenuCamera = entity;

			ECS::AttachComponent<Transform>(entity);

			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-20000.f, 0.f, 0.f));
		}

		//Title logo
		{
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);


			//Set up components
			std::string fileName = "title_logo.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 280, 120);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-20000.f, 120.f, 1.f));
		}

		//Start Game Button
		{
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<ButtonSelect>(entity);


			//Set up components
			std::string fileName = "Start_Button.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 150, 60);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-20000.f, -10.f, 1.f));
			ECS::GetComponent<ButtonSelect>(entity).isSelected = true;

			mainMenuButtons.push_back(entity);
		}

		//Controls button
		{	
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<ButtonSelect>(entity);


			//Set up components
			std::string fileName = "Controls_Button.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 150, 60);
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.75);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-20000.f, -80.f, 1.f));

			mainMenuButtons.push_back(entity);
		}

		//Exit button
		{
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<ButtonSelect>(entity);


			//Set up components
			std::string fileName = "Exit_Button.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 150, 60);
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.75);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-20000.f, -150.f, 1.f));

			mainMenuButtons.push_back(entity);
		}

		
	}

	//Character Select Menu
	{
		//Character select menu camera target
		{
			auto entity = ECS::CreateEntity();
			characterMenuCamera = entity;

			ECS::AttachComponent<Transform>(entity);

			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-30000.f, 0.f, 0.f));
		}

		//Banana select button
		{
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<ButtonSelect>(entity);


			//Set up components
			std::string fileName = "Banana_Description.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 350, 150);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-30000.f, -100.f, 1.f));
			ECS::GetComponent<ButtonSelect>(entity).isSelected = true;

			characterSelectButtons.push_back(entity);
		}

		//Banana select character sprite
		{
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<ButtonSelect>(entity);


			//Set up components
			std::string fileName = "banana.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 130, 130);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-30200.f, 90.f, 1.f));
			ECS::GetComponent<ButtonSelect>(entity).isSelected = true;

			characterSelectButtons.push_back(entity);
		}

		//Apple select button
		{
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<ButtonSelect>(entity);


			//Set up components
			std::string fileName = "Apple_Description.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 350, 150);
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-30000.f, -100.f, 1.f));

			characterSelectButtons.push_back(entity);
		}

		//Apple select character sprite
		{
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<ButtonSelect>(entity);


			//Set up components
			std::string fileName = "apple.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 130, 130);
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.75f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-30000.f, 90.f, 1.f));

			characterSelectButtons.push_back(entity);
		}

		//Watermelon select button
		{
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<ButtonSelect>(entity);


			//Set up components
			std::string fileName = "Watermelon_Description.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 350, 150);
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-30000.f, -100.f, 1.f));

			characterSelectButtons.push_back(entity);
		}

		//Watermelon select character sprite
		{
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<ButtonSelect>(entity);


			//Set up components
			std::string fileName = "watermelon.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 130, 130);
			ECS::GetComponent<Sprite>(entity).SetTransparency(0.75);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-29800.f, 90.f, 1.f));

			characterSelectButtons.push_back(entity);
		}
	}

	//Controls Menu
	{
		//Controls menu camera target
		{
			auto entity = ECS::CreateEntity();
			controlsMenuCamera = entity;

			ECS::AttachComponent<Transform>(entity);

			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-40000.f, 0.f, 0.f));
		}

		//Controls menu entity
		{
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			//Set up components
			std::string fileName = "controlswide.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 850.f, 430.f);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-40000, 0.f, 0.f));
		}
	}

	//Victory Screen
	{
		//Victory screen camera target
		{
			auto entity = ECS::CreateEntity();
			victoryScreenCamera = entity;

			ECS::AttachComponent<Transform>(entity);

			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-40000.f, 0.f, 0.f));
		}

		//Victory screen entity
		{
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			//Set up components
			std::string fileName = "controlswide.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 850.f, 430.f);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-50000, 0.f, 0.f));
		}
	}

	//Game Over Screen
	{
		//Game over camera target
		{
			auto entity = ECS::CreateEntity();
			gameOverCamera = entity;

			ECS::AttachComponent<Transform>(entity);

			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-40000.f, 0.f, 0.f));
		}

		//Game over entity
		{
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);

			//Set up components
			std::string fileName = "controlswide.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 850.f, 430.f);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(-40000, 0.f, 0.f));
		}
	}


	//Main Game
	{

		//Player entity
		{
			/*Scene::CreatePhysicsSprite(m_sceneReg, "LinkStandby", 80, 60, 1.f, vec3(0.f, 30.f, 2.f), b2_dynamicBody, 0.f, 0.f, true, true)*/

			auto entity = ECS::CreateEntity();
			ECS::SetIsMainPlayer(entity, true);

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);
			ECS::AttachComponent<CanJump>(entity);
			ECS::AttachComponent<PlayerFacing>(entity);
			ECS::AttachComponent<PlayerStats>(entity);
			ECS::AttachComponent<CanClimb>(entity);

			//Sets up the components
			std::string fileName = "apple.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 35, 35);
			ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(0.f, 30.f, 2.f));
			ECS::GetComponent<PlayerStats>(entity).checkPointPos = b2Vec2(0.f, 30.f);

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;

			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_dynamicBody;
			tempDef.position.Set(float32(0.f), float32(30.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			//tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);
			tempPhsBody = PhysicsBody(entity, tempBody, float((tempSpr.GetHeight() - shrinkY) / 2.f), vec2(0.f, 0.f), false, PLAYER, ENVIRONMENT | ENEMY | OBJECTS | PICKUP | TRIGGER | HEXAGON, 0.5f, 3.f);
			//std::vector<b2Vec2> points = {b2Vec2(-tempSpr.GetWidth()/2.f, -tempSpr.GetHeight()/2.f), b2Vec2(tempSpr.GetWidth()/2.f, -tempSpr.GetHeight()/2.f), b2Vec2(0.f, tempSpr.GetHeight()/2.f)};
			//tempPhsBody = PhysicsBody(entity, BodyType::TRIANGLE, tempBody, points, vec2(0.f, 0.f), false, PLAYER, ENEMY | OBJECTS | PICKUP | TRIGGER, 0.5f, 3.f);

			tempPhsBody.SetRotationAngleDeg(0.f);
			tempPhsBody.SetFixedRotation(true);
			tempPhsBody.SetColor(vec4(1.f, 0.f, 1.f, 0.3f));
			tempPhsBody.SetGravityScale(1.4f);

		}

		//Player health bar
		{
			//Heart 1
			{
				auto entity = ECS::CreateEntity();

				//Add Components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				std::string fileName = "heartfull.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50);
				ECS::GetComponent<Transform>(entity).SetPosition(-300, 180, 10);

				healthSpriteStorage.push_back(entity);
			}
			//Heart 2
			{
				auto entity = ECS::CreateEntity();

				//Add Components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				std::string fileName = "heartfull.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50);
				ECS::GetComponent<Transform>(entity).SetPosition(-250, 180, 10);

				healthSpriteStorage.push_back(entity);
			}		
			//Heart 3
			{
				auto entity = ECS::CreateEntity();

				//Add Components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				std::string fileName = "heartfull.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50);
				ECS::GetComponent<Transform>(entity).SetPosition(-300, 180, 10);

				healthSpriteStorage.push_back(entity);
			}
			//Heart 4
			{
				auto entity = ECS::CreateEntity();

				//Add Components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				std::string fileName = "heartfull.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50);
				ECS::GetComponent<Transform>(entity).SetPosition(-300, 180, 10);

				healthSpriteStorage.push_back(entity);
			}
			//Heart 5
			{
				auto entity = ECS::CreateEntity();

				//Add Components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				std::string fileName = "heartfull.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50);
				ECS::GetComponent<Transform>(entity).SetPosition(-300, 180, 10);

				healthSpriteStorage.push_back(entity);
			}
			//Heart 1
			{
				auto entity = ECS::CreateEntity();

				//Add Components
				ECS::AttachComponent<Sprite>(entity);
				ECS::AttachComponent<Transform>(entity);

				std::string fileName = "heartfull.png";
				ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 50);
				ECS::GetComponent<Transform>(entity).SetPosition(-300, 180, 10);

				healthSpriteStorage.push_back(entity);
			}
		}

		//Beginning Platform
		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 220, 10);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(30.f), float32(-10.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Setup ladder trigger
		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);
			ECS::AttachComponent<Trigger*>(entity);

			//Sets up components
			std::string fileName = "boxSprite.jpg";
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 80.f));
			ECS::GetComponent<Trigger*>(entity) = new LadderTrigger();
			ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
			ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(climb);
			LadderTrigger* temp = (LadderTrigger*)ECS::GetComponent<Trigger*>(entity);

			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(295.f), float32(120.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(40.f - shrinkX), float(100.f - shrinkY), vec2(0.f, 0.f), true, TRIGGER, PLAYER | OBJECTS);
			tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		}

		//Beginning Platform - First Jump Platform
		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 20);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(95.f), float32(5.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Beginning Platform - Second Jump Platform
		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 50, 8);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(175.f), float32(35.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Second Platform
		{
			//Creates entitys
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "boxSprite.jpg";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 400, 10);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(430.f), float32(-10.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON), 0, 1000.f;
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Second Platform - Top Platform
		{
			//Creates entitys
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 300, 10);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(400.f), float32(60.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Second Platform - Wall
		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 70, 40);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(610.f), float32(30.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENVIRONMENT, PLAYER | OBJECTS | ENEMY | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
			tempPhsBody.SetRotationAngleDeg(90.f);
		}

		//Third Platform
		{
			//Creates entitys
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 320, 10);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(835.f), float32(-10.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Third Platform - First Jump Platform
		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 7);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(770.f), float32(25.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Third Platform - Second Jump Platform
		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 7);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(820.f), float32(35.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Third Platform - Third Jump Platform
		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 7);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(880.f), float32(60.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Third Platform - Fourth Jump Platform
		{
			//Creates entitys
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 70, 10);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(960.f), float32(80.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Item Platform
		{
			//Creates entitys
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 250, 10);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(865.f), float32(350.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Fourth Platform
		{
			//Creates entitys
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 500, 10);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(1370.f), float32(-10.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Fourth Platform - Checkpoint Platform
		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 7);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(1370.f), float32(-2.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Fourth Platform - First Jump Platform
		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 7);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(1530.f), float32(20.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Fourth Platform - Second Jump Platform
		{
			//Creates entitys
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 300, 6);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(1740.f), float32(50.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
				float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Fifth Platform
		{
			//Creates entitys
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 450, 10);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(2000.f), float32(-10.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
				float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Fifth Platform - Wall
		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 110, 7);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(2100.f), float32(50.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, ENVIRONMENT, OBJECTS | ENEMY | HEXAGON); //Removed PLAYER collision
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));
			tempPhsBody.SetRotationAngleDeg(90.f);
		}

		//Fifth Platform - Top Left Platform On Top Of Ladder
		{
			//Creates entitys
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 6);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(2050.f), float32(105.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
				float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Fifth Platform - Platform To Sixth Platform
		{
			//Creates entitys
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 6);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(2197.f), float32(105.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
				float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Sixth Platform
		{
			//Creates entitys
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 210, 10);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(2480.f), float32(-10.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
				float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Sixth Platform - Jump Platform
		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 40, 30);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(2545.f), float32(10.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
				float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Last Platform
		{
			//Creates entitys
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 320, 10);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(2850.f), float32(-10.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
				float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Last Platform - First Jump Platform
		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 7);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(2740.f), float32(12.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
				float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Last Platform - Second Jump Platform
		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 7);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(2800.f), float32(30.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
				float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Last Platform - Third Jump Platform
		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 7);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(2860.f), float32(50.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
				float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Last Platform - End Level Platform
		{
			//Creates entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Sets up components
			std::string fileName = "platform_mk_ii.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 7);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(2950.f), float32(-2.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);

			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
				float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, GROUND, PLAYER | ENEMY | OBJECTS | HEXAGON);
			tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

		}

		//Cart Finish Line
		{
			//Create entity
			auto entity = ECS::CreateEntity();

			//Add components
			ECS::AttachComponent<Sprite>(entity);
			ECS::AttachComponent<Transform>(entity);
			ECS::AttachComponent<PhysicsBody>(entity);

			//Set up components
			std::string fileName = "cart.png";
			ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 200, 190);
			ECS::GetComponent<Transform>(entity).SetPosition(vec3(3100.f, 0.f, 2.f));

			auto& tempSpr = ECS::GetComponent<Sprite>(entity);
			auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

			float shrinkX = 0.f;
			float shrinkY = 0.f;
			b2Body* tempBody;
			b2BodyDef tempDef;
			tempDef.type = b2_staticBody;
			tempDef.position.Set(float32(3100.f), float32(-120.f));

			tempBody = m_physicsWorld->CreateBody(&tempDef);
			tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX), float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, FINISH, PLAYER);
			tempPhsBody.SetColor(vec4(20.f, 1.f, 19.f, 5.f));

		}

		//Enemy 1
		unsigned enemy1 = CreateCookieEnemy(500.f, 72.f, false);
		enemyStorage.push_back(enemy1);

		////Enemy 1
		//{
		//	//Creates entity
		//	auto entity = ECS::CreateEntity();

		//	//Add components
		//	ECS::AttachComponent<Sprite>(entity);
		//	ECS::AttachComponent<Transform>(entity);
		//	ECS::AttachComponent<PhysicsBody>(entity);
		//	//ECS::AttachComponent<Trigger*>(entity);

		//	//Sets up components
		//	std::string fileName = "evil_cookie_static.png";
		//	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 25);
		//	ECS::GetComponent<Transform>(entity).SetPosition(vec3(35.f, -8.f, 3.f));
		//	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		//	//ECS::GetComponent<Trigger*>(entity) = new EnemyTrigger();
		//	//ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		//	//ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(entity);
		//	//EnemyTrigger* temp = (EnemyTrigger*)ECS::GetComponent<Trigger*>(entity);

		//	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		//	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//	float shrinkX = 0.f;
		//	float shrinkY = 0.f;
		//	b2Body* tempBody;
		//	b2BodyDef tempDef;
		//	tempDef.type = b2_staticBody;
		//	tempDef.position.Set(float32(500.f), float32(76.f));

		//	tempBody = m_physicsWorld->CreateBody(&tempDef);

		//	tempPhsBody = PhysicsBody(entity, tempBody, float(25.f - shrinkX), float(25.f - shrinkY), vec2(0.f, 0.f), false, ENEMY, PLAYER);
		//	tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		//}

		////Enemy 2
		//{
		//	//Creates entity
		//	auto entity = ECS::CreateEntity();

		//	//Add components
		//	ECS::AttachComponent<Sprite>(entity);
		//	ECS::AttachComponent<Transform>(entity);
		//	ECS::AttachComponent<PhysicsBody>(entity);
		//	//ECS::AttachComponent<Trigger*>(entity);

		//	//Sets up components
		//	std::string fileName = "evil_cookie_static.png";
		//	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 25);
		//	ECS::GetComponent<Transform>(entity).SetPosition(vec3(35.f, -8.f, 3.f));
		//	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		//	//ECS::GetComponent<Trigger*>(entity) = new EnemyTrigger();
		//	//ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		//	//ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(entity);
		//	//EnemyTrigger* temp = (EnemyTrigger*)ECS::GetComponent<Trigger*>(entity);

		//	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		//	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//	float shrinkX = 0.f;
		//	float shrinkY = 0.f;
		//	b2Body* tempBody;
		//	b2BodyDef tempDef;
		//	tempDef.type = b2_staticBody;
		//	tempDef.position.Set(float32(500.f), float32(6.f));

		//	tempBody = m_physicsWorld->CreateBody(&tempDef);

		//	tempPhsBody = PhysicsBody(entity, tempBody, float(25.f - shrinkX), float(25.f - shrinkY), vec2(0.f, 0.f), false, ENEMY, PLAYER);
		//	tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		//}

		////Enemy 3
		//{
		//	//Creates entity
		//	auto entity = ECS::CreateEntity();

		//	//Add components
		//	ECS::AttachComponent<Sprite>(entity);
		//	ECS::AttachComponent<Transform>(entity);
		//	ECS::AttachComponent<PhysicsBody>(entity);
		//	//ECS::AttachComponent<Trigger*>(entity);

		//	//Sets up components
		//	std::string fileName = "evil_cookie_static.png";
		//	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 25, 25);
		//	ECS::GetComponent<Transform>(entity).SetPosition(vec3(35.f, -8.f, 3.f));
		//	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		//	//ECS::GetComponent<Trigger*>(entity) = new EnemyTrigger();
		//	//ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		//	//ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(entity);
		//	//EnemyTrigger* temp = (EnemyTrigger*)ECS::GetComponent<Trigger*>(entity);

		//	auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		//	auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		//	float shrinkX = 0.f;
		//	float shrinkY = 0.f;
		//	b2Body* tempBody;
		//	b2BodyDef tempDef;
		//	tempDef.type = b2_staticBody;
		//	tempDef.position.Set(float32(420.f), float32(6.f));

		//	tempBody = m_physicsWorld->CreateBody(&tempDef);

		//	tempPhsBody = PhysicsBody(entity, tempBody, float(25.f - shrinkX), float(25.f - shrinkY), vec2(0.f, 0.f), false, ENEMY, PLAYER);
		//	tempPhsBody.SetColor(vec4(1.f, 0.f, 0.f, 0.3f));
		//}

	//Turn trigger 1 
	{
		//Creates entity
		auto entity = ECS::CreateEntity();

		//Add components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);
		ECS::AttachComponent<PhysicsBody>(entity);
		ECS::AttachComponent<Trigger*>(entity);

		//Sets up components
		std::string fileName = "greybox.jpg";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 30, 30);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(30.f, -20.f, 2.f));
		ECS::GetComponent<Sprite>(entity).SetTransparency(0.f);
		ECS::GetComponent<Trigger*>(entity) = new ReverseTrigger();
		ECS::GetComponent<Trigger*>(entity)->SetTriggerEntity(entity);
		ECS::GetComponent<Trigger*>(entity)->AddTargetEntity(entity);
		ReverseTrigger* temp = (ReverseTrigger*)ECS::GetComponent<Trigger*>(entity);

		auto& tempSpr = ECS::GetComponent<Sprite>(entity);
		auto& tempPhsBody = ECS::GetComponent<PhysicsBody>(entity);

		float shrinkX = 0.f;
		float shrinkY = 0.f;
		b2Body* tempBody;
		b2BodyDef tempDef;
		tempDef.type = b2_staticBody;
		tempDef.position.Set(float32(230.f), float32(76.f));

		tempBody = m_physicsWorld->CreateBody(&tempDef);

		tempPhsBody = PhysicsBody(entity, tempBody, float(tempSpr.GetWidth() - shrinkX),
			float(tempSpr.GetHeight() - shrinkY), vec2(0.f, 0.f), false, TRIGGER, ENEMY);
		tempPhsBody.SetColor(vec4(0.f, 1.f, 0.f, 0.3f));

	}
		ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
		ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
	}
}

void CartCraze::Update()
{
	if (mainMenu == true)
	{
		ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(mainMenuCamera));
		ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(mainMenuCamera));
	}

	if (controlsMenu == true)
	{
		ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(controlsMenuCamera));
		ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(controlsMenuCamera));
	}

	if (characterMenu == true)
	{
		ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(characterMenuCamera));
		ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(characterMenuCamera));
	}

	if (gameOver == true)
	{
		ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(gameOverCamera));
		ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(gameOverCamera));
	}

	if (ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).playerWon == true)
	{
		ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(victoryScreenCamera));
		ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(victoryScreenCamera));
	}

	if (gameRun == true)
	{
		ECS::GetComponent<HorizontalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
		ECS::GetComponent<VerticalScroll>(MainEntities::MainCamera()).SetFocus(&ECS::GetComponent<Transform>(MainEntities::MainPlayer()));
		//Checking for collided projectiles (seeds, juice, peels)
		{
			for (int i = 0; i < projectileStorage.size(); i++)
			{
				if (ECS::GetComponent<ProjectileCollision>(projectileStorage[i]).hasCollided == true)
				{
					PhysicsBody::m_bodiesToDelete.push_back(projectileStorage[i]);
					projectileStorage.erase(projectileStorage.begin() + i);
				}
			}
		}

		//Checking banana peel stuck status
		for (int i = 0; i < projectileStorage.size(); i++)
		{
			if (ECS::GetComponent<ProjectileCollision>(projectileStorage[i]).isStuck == true)
			{
				ECS::GetComponent<PhysicsBody>(projectileStorage[i]).SetVelocity(vec3(0.f, 0.f, 0.f));
			}
		}


		//Checking for more than 3 banana peels
		{
			//if ((ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isBanana == true) && (projectileStorage.capacity()>3))
			//{
			//PhysicsBody::m_bodiesToDelete.push_back(projectileStorage[1]);
			//projectileStorage.erase(projectileStorage.begin());
			//}
		}

		//Enemy health/deletion check
		{
			for (int i = 0; i < enemyStorage.size(); i++)
			{
				if (ECS::GetComponent<EnemyStats>(enemyStorage[i]).health <= 0)
				{
					PhysicsBody::m_bodiesToDelete.push_back(enemyStorage[i]);
					enemyStorage.erase(enemyStorage.begin() + i);
				}
			}
		}

		//Enemy stun check
		{
			for (int i = 0; i < enemyStorage.size(); i++)
			{
				if (ECS::GetComponent<EnemyStats>(enemyStorage[i]).isStunned == true)
				{
					int stunTime = 3;
					int stunnedAt = Timer::time;
					ECS::GetComponent<PhysicsBody>(enemyStorage[i]).SetVelocity(vec3(0.f, 0.f, 0.f));
					ECS::GetComponent<Sprite>(enemyStorage[i]).SetTransparency(0.5f);
					if (Timer::time >= (stunnedAt + stunTime))
					{
						ECS::GetComponent<EnemyStats>(enemyStorage[i]).isStunned = false;
						ECS::GetComponent<Sprite>(enemyStorage[i]).SetTransparency(1.f);
						stunnedAt = 0;
					}
				}
			}
		}

		//Player stun check
		{

		}

		//Player health check
		{
			if (ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isWaterMelon)
			{

			}


			if (ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isApple)
			{

			}


			if (ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).health <= 0)
			{
				gameOver = true;
			}
		}
	}

	//Fall check (checks if player has fallen past a certain point, damages them and sends them back to their checkpoint if so.
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	b2Vec2 playerPosition = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetPosition();
	if (playerPosition.y < -150)
	{
		ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).health -= 1;
		player.SetPosition(ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).checkPointPos);
		player.SetVelocity(vec3(0, 0, 0));
	}


}

void CartCraze::GUI()
{
	GUIWindowUI();

	if (m_firstWindow)
	{
		GUIWindowOne();
	}

	if (m_secondWindow)
	{
		GUIWindowTwo();
	}
}

void CartCraze::GUIWindowUI()
{
	ImGui::Begin("Test");

	ImGui::Checkbox("Enable First Window", &m_firstWindow);
	ImGui::SameLine();
	if (ImGui::Button("Toggle Second Window", ImVec2(180.f, 15.f)))
	{
		m_secondWindow = !m_secondWindow;
	}
	
	ImGui::End();
}

void CartCraze::GUIWindowOne()
{
	//Window begin
	ImGui::Begin("Side Docked Window");
	//is the buffer initialized
	static bool init = false;

	//Buffers
	const unsigned int BUF_LEN = 512;
	static char buf[BUF_LEN];
	//Image load
	static std::string imageLoad = "LinkStandby.png";
	//World gravity slider
	float gravity[2] = { m_physicsWorld->GetGravity().x, m_physicsWorld->GetGravity().y };

	if (!init)
	{
		memset(buf, 0, BUF_LEN);
		init = true;
	}
	m_physicsWorld->SetAllowSleeping(true);

	//ImGui content
	if (ImGui::TreeNode("Vignette Effect"))
	{
		if (EffectManager::GetVignetteHandle() != -1)
		{
			if (ImGui::TreeNode("Properties"))
			{
				VignetteEffect* vig = (VignetteEffect*)EffectManager::GetEffect(EffectManager::GetVignetteHandle());
				float innerRad = vig->GetInnerRadius();
				float outerRad = vig->GetOuterRadius();
				float opacity = vig->GetOpacity();

				if (ImGui::SliderFloat("Inner Radius", &innerRad, 0.f, outerRad))
				{
					vig->SetInnerRadius(innerRad);
				}
				if (ImGui::SliderFloat("Outer Radius", &outerRad, innerRad, 1.f))
				{
					vig->SetOuterRadius(outerRad);
				}
				if (ImGui::SliderFloat("Opacity", &opacity, 0.f, 1.f))
				{
					vig->SetOpacity(opacity);
				}

				ImGui::TreePop();
			}

			if (ImGui::Button("Remove Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::RemoveEffect(EffectManager::GetVignetteHandle());
			}
		}
		else
		{
			if (ImGui::Button("Apply Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::CreateEffect(Vignette, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
			}
		}


		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Sepia Effect"))
	{
		if (EffectManager::GetSepiaHandle() != -1)
		{
			if (ImGui::TreeNode("Properties"))
			{
				SepiaEffect* sep = (SepiaEffect*)EffectManager::GetEffect(EffectManager::GetSepiaHandle());
				float intensity = sep->GetIntensity();

				if (ImGui::SliderFloat("Inner Radius", &intensity, 0.f, 1.f))
				{
					sep->SetIntensity(intensity);
				}

				ImGui::TreePop();
			}

			if (ImGui::Button("Remove Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::RemoveEffect(EffectManager::GetSepiaHandle());
			}
		}
		else
		{
			if (ImGui::Button("Apply Effect", ImVec2(100.f, 30.f)))
			{
				EffectManager::CreateEffect(Sepia, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
			}
		}


		ImGui::TreePop();
	}

	ImGui::Separator();
	
	ImGui::Text("Editing World Variables!");
	if (ImGui::SliderFloat2("World Gravity", gravity, -1000.f, 1000.f, "%.2f"))
	{
		m_physicsWorld->SetGravity(b2Vec2(gravity[0], gravity[1]));
		m_physicsWorld->SetAllowSleeping(false);
	}

	ImGui::Separator();

	ImGui::Text("Displaying image dynamically");
	ImGui::InputText("Input filename for image", buf, BUF_LEN, ImGuiInputTextFlags_CharsNoBlank);

	if (ImGui::Button("Confirm File", ImVec2(100.f, 30.f)))
	{
		imageLoad = buf;
	}

	ImGui::Image((void*)(intptr_t)TextureManager::FindTexture(imageLoad)->GetID(), ImVec2(150.f, 150.f), ImVec2(0, 1), ImVec2(1, 0));

	ImGui::Separator();
	
	ImGui::Text("Editing Colors");
	ImGui::ColorPicker4("Scene Background Color", &m_clearColor.x);

	ImGui::End();
}

void CartCraze::GUIWindowTwo()
{
	//Second docked window
	ImGui::Begin("Second Window");
	//Image load
	static std::string imageLoad1 = "FunnyButton.png";
	static std::string imageLoad2 = "LikeButton.png";
	static std::string hahaPressed = "";
	static std::string likePressed = "";

	ImGui::BeginTabBar("Tab Bar Test");

	if (ImGui::BeginTabItem("Tab 1"))
	{
		ImGui::Text("You are within Tab 1");

		if (ImGui::ImageButton((void*)(intptr_t)TextureManager::FindTexture(imageLoad1)->GetID(), ImVec2(100.f, 100.f), ImVec2(0,1), ImVec2(1,0)))
		{
			hahaPressed = "You shouldn't have pressed that...";
		}

		ImGui::Text("%s", hahaPressed.c_str());

		ImGui::EndTabItem();
	}

	if (ImGui::BeginTabItem("Tab 2"))
	{
		ImGui::Text("You are within Tab 2");

		if (ImGui::ImageButton((void*)(intptr_t)TextureManager::FindTexture(imageLoad2)->GetID(), ImVec2(100.f, 100.f), ImVec2(0, 1), ImVec2(1, 0)))
		{
			likePressed = "LIKED!!!";
		}

		ImGui::Text("%s", likePressed.c_str());

		ImGui::EndTabItem();
	}
	
	ImGui::EndTabBar();

	ImGui::End();
}



void CartCraze::KeyboardHold()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	auto& canClimb = ECS::GetComponent<CanClimb>(MainEntities::MainPlayer());

	float playerSpeed = ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).speed;
	float speed = 2.f;
	b2Vec2 vel = b2Vec2(0.f, 0.f);

	if (gameRun == true)
	{
		if (Input::GetKey(Key::A))
		{
			player.GetBody()->ApplyForceToCenter(b2Vec2(-playerSpeed * speed, 0.f), true);
			ECS::GetComponent<PlayerFacing>(MainEntities::MainPlayer()).isFacingRight = false;

		}
		if (Input::GetKey(Key::D))
		{
			player.GetBody()->ApplyForceToCenter(b2Vec2(playerSpeed * speed, 0.f), true);
			ECS::GetComponent<PlayerFacing>(MainEntities::MainPlayer()).isFacingRight = true;
		}

		if (Input::GetKey(Key::W))
		{
			if (canClimb.m_canClimb)
			{
				player.GetBody()->ApplyForceToCenter(b2Vec2(0.f * speed, 90000000.f), true);
			}
		}
		if (Input::GetKey(Key::S))
		{

		}
	}
}

void CartCraze::KeyboardDown()
{
	auto& player = ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer());
	auto& canJump = ECS::GetComponent<CanJump>(MainEntities::MainPlayer());
	std::string fileName = "LinkStandby.png";

	if (mainMenu == true)
	{
		if (Input::GetKeyDown(Key::W) || Input::GetKeyDown(Key::UpArrow))
		{
			if (ECS::GetComponent<ButtonSelect>(mainMenuButtons[0]).isSelected == true)
			{
				ECS::GetComponent<Sprite>(mainMenuButtons[0]).SetTransparency(0.75);
				ECS::GetComponent<ButtonSelect>(mainMenuButtons[2]).isSelected = true;
				ECS::GetComponent<Sprite>(mainMenuButtons[2]).SetTransparency(1.f);
				ECS::GetComponent<ButtonSelect>(mainMenuButtons[0]).isSelected = false;
			}
			else if (ECS::GetComponent<ButtonSelect>(mainMenuButtons[1]).isSelected == true)
			{
				ECS::GetComponent<Sprite>(mainMenuButtons[1]).SetTransparency(0.75);
				ECS::GetComponent<ButtonSelect>(mainMenuButtons[0]).isSelected = true;
				ECS::GetComponent<Sprite>(mainMenuButtons[0]).SetTransparency(1.f);
				ECS::GetComponent<ButtonSelect>(mainMenuButtons[1]).isSelected = false;

			}
			else if (ECS::GetComponent<ButtonSelect>(mainMenuButtons[2]).isSelected == true)
			{
				ECS::GetComponent<Sprite>(mainMenuButtons[2]).SetTransparency(0.75);
				ECS::GetComponent<ButtonSelect>(mainMenuButtons[1]).isSelected = true;
				ECS::GetComponent<Sprite>(mainMenuButtons[1]).SetTransparency(1.f);
				ECS::GetComponent<ButtonSelect>(mainMenuButtons[2]).isSelected = false;
			}
		}
		if (Input::GetKeyDown(Key::S) || Input::GetKeyDown(Key::DownArrow))
		{
			if (ECS::GetComponent<ButtonSelect>(mainMenuButtons[0]).isSelected == true)
			{
				ECS::GetComponent<ButtonSelect>(mainMenuButtons[0]).isSelected = false;
				ECS::GetComponent<Sprite>(mainMenuButtons[0]).SetTransparency(0.75);
				ECS::GetComponent<ButtonSelect>(mainMenuButtons[1]).isSelected = true;
				ECS::GetComponent<Sprite>(mainMenuButtons[1]).SetTransparency(1.f);
			}
			else if (ECS::GetComponent<ButtonSelect>(mainMenuButtons[1]).isSelected == true)
			{
				ECS::GetComponent<ButtonSelect>(mainMenuButtons[1]).isSelected = false;
				ECS::GetComponent<Sprite>(mainMenuButtons[1]).SetTransparency(0.75);
				ECS::GetComponent<ButtonSelect>(mainMenuButtons[2]).isSelected = true;
				ECS::GetComponent<Sprite>(mainMenuButtons[2]).SetTransparency(1.f);
			}
			else if (ECS::GetComponent<ButtonSelect>(mainMenuButtons[2]).isSelected == true)
			{
				ECS::GetComponent<ButtonSelect>(mainMenuButtons[2]).isSelected = false;
				ECS::GetComponent<Sprite>(mainMenuButtons[2]).SetTransparency(0.75);
				ECS::GetComponent<ButtonSelect>(mainMenuButtons[0]).isSelected = true;
				ECS::GetComponent<Sprite>(mainMenuButtons[0]).SetTransparency(1.f);
			}
		}

		if (Input::GetKeyDown(Key::Enter))
		{
			if (mainMenu == true)
			{
				if (ECS::GetComponent<ButtonSelect>(mainMenuButtons[0]).isSelected == true) 
				{
					gameRun = false;
					mainMenu = false;
					characterMenu = true;
					controlsMenu = false;
					gameWin = false;
					gameOver = false;
				}
				else if (ECS::GetComponent<ButtonSelect>(mainMenuButtons[1]).isSelected == true)
				{
					gameRun = false;
					mainMenu = false;
					characterMenu = false;
					controlsMenu = true;
					gameWin = false;
					gameOver = false;
				}
				else if (ECS::GetComponent<ButtonSelect>(mainMenuButtons[2]).isSelected == true)
				{
					exit(0);
				}
			}
		}
	}

	else if (controlsMenu == true)
	{
		if (Input::GetKeyDown(Key::Enter))
		{
			gameRun = false;
			mainMenu = true;
			characterMenu = false;
			controlsMenu = false;
			gameWin = false;
			gameOver = false;
		}
	}

	else if (characterMenu == true)
	{
		if (Input::GetKeyDown(Key::D) || Input::GetKeyDown(Key::RightArrow))
		{
			if (ECS::GetComponent<ButtonSelect>(characterSelectButtons[0]).isSelected == true)
			{
				ECS::GetComponent<ButtonSelect>(characterSelectButtons[2]).isSelected = true;
				ECS::GetComponent<Sprite>(characterSelectButtons[2]).SetTransparency(1.f);
				ECS::GetComponent<Sprite>(characterSelectButtons[3]).SetTransparency(1.f);
				ECS::GetComponent<ButtonSelect>(characterSelectButtons[0]).isSelected = false;
				ECS::GetComponent<Sprite>(characterSelectButtons[0]).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(characterSelectButtons[1]).SetTransparency(0.75f);
			}
			else if (ECS::GetComponent<ButtonSelect>(characterSelectButtons[2]).isSelected == true)
			{
				ECS::GetComponent<ButtonSelect>(characterSelectButtons[4]).isSelected = true;
				ECS::GetComponent<Sprite>(characterSelectButtons[4]).SetTransparency(1.f);
				ECS::GetComponent<Sprite>(characterSelectButtons[5]).SetTransparency(1.f);
				ECS::GetComponent<ButtonSelect>(characterSelectButtons[2]).isSelected = false;
				ECS::GetComponent<Sprite>(characterSelectButtons[2]).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(characterSelectButtons[3]).SetTransparency(0.75f);
			}
			else if (ECS::GetComponent<ButtonSelect>(characterSelectButtons[4]).isSelected == true)
			{
				ECS::GetComponent<ButtonSelect>(characterSelectButtons[0]).isSelected = true;
				ECS::GetComponent<Sprite>(characterSelectButtons[0]).SetTransparency(1.f);
				ECS::GetComponent<Sprite>(characterSelectButtons[1]).SetTransparency(1.f);
				ECS::GetComponent<ButtonSelect>(characterSelectButtons[4]).isSelected = false;
				ECS::GetComponent<Sprite>(characterSelectButtons[4]).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(characterSelectButtons[5]).SetTransparency(0.75f);
			}
		}

		if (Input::GetKeyDown(Key::A) || Input::GetKeyDown(Key::LeftArrow))
		{
			if (ECS::GetComponent<ButtonSelect>(characterSelectButtons[0]).isSelected == true)
			{
				ECS::GetComponent<ButtonSelect>(characterSelectButtons[4]).isSelected = true;
				ECS::GetComponent<Sprite>(characterSelectButtons[4]).SetTransparency(1.f);
				ECS::GetComponent<Sprite>(characterSelectButtons[5]).SetTransparency(1.f);
				ECS::GetComponent<ButtonSelect>(characterSelectButtons[0]).isSelected = false;
				ECS::GetComponent<Sprite>(characterSelectButtons[0]).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(characterSelectButtons[1]).SetTransparency(0.75f);
			}
			else if (ECS::GetComponent<ButtonSelect>(characterSelectButtons[2]).isSelected == true)
			{
				ECS::GetComponent<ButtonSelect>(characterSelectButtons[0]).isSelected = true;
				ECS::GetComponent<Sprite>(characterSelectButtons[0]).SetTransparency(1.f);
				ECS::GetComponent<Sprite>(characterSelectButtons[1]).SetTransparency(1.f);
				ECS::GetComponent<ButtonSelect>(characterSelectButtons[2]).isSelected = false;
				ECS::GetComponent<Sprite>(characterSelectButtons[2]).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(characterSelectButtons[3]).SetTransparency(0.75f);
			}
			else if (ECS::GetComponent<ButtonSelect>(characterSelectButtons[4]).isSelected == true)
			{
				ECS::GetComponent<ButtonSelect>(characterSelectButtons[2]).isSelected = true;
				ECS::GetComponent<Sprite>(characterSelectButtons[2]).SetTransparency(1.f);
				ECS::GetComponent<Sprite>(characterSelectButtons[3]).SetTransparency(1.f);
				ECS::GetComponent<ButtonSelect>(characterSelectButtons[4]).isSelected = false;
				ECS::GetComponent<Sprite>(characterSelectButtons[4]).SetTransparency(0.f);
				ECS::GetComponent<Sprite>(characterSelectButtons[5]).SetTransparency(0.75f);
			}
		}

		if (Input::GetKeyDown(Key::Enter))
		{
			if (ECS::GetComponent<ButtonSelect>(characterSelectButtons[0]).isSelected == true)
			{
				ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isWaterMelon = false;
				ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isApple = false;
				ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isBanana = true;
				ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).health = 2;
				ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).speed = 450000;
				fileName = "banana.png";
				ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).LoadSprite(fileName, 35, 35);
				//ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).SetGravityScale(0.8f);
				gameRun = true;
				mainMenu = false;
				characterMenu = false;
				controlsMenu = false;
				gameWin = false;
				gameOver = false;
			}
			else if (ECS::GetComponent<ButtonSelect>(characterSelectButtons[2]).isSelected == true)
			{
				ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isWaterMelon = false;
				ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isBanana = false;
				ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isApple = true;
				ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).health = 3;
				ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).speed = 350000;
				fileName = "apple.png";
				ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).LoadSprite(fileName, 35, 35);
				gameRun = true;
				mainMenu = false;
				characterMenu = false;
				controlsMenu = false;
				gameWin = false;
				gameOver = false;
			}
			else if (ECS::GetComponent<ButtonSelect>(characterSelectButtons[4]).isSelected == true)
			{
				ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isBanana = false;
				ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isApple = false;
				ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isWaterMelon = true;
				ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).health = 5;
				ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).speed = 200000;
				fileName = "watermelon.png";
				ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).LoadSprite(fileName, 35, 35);
				ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).SetMass((ECS::GetComponent<PhysicsBody>(MainEntities::MainPlayer()).GetMass())*0.6);
				gameRun = true;
				mainMenu = false;
				characterMenu = false;
				controlsMenu = false;
				gameWin = false;
				gameOver = false;
			}
		}
	}

	if (Input::GetKeyDown(Key::T))
	{
		PhysicsBody::SetDraw(!PhysicsBody::GetDraw());
	}
	if (canJump.m_canJump)
	{
		if (Input::GetKeyDown(Key::Space))
		{
			if (gameRun == true)
			{
				player.GetBody()->ApplyLinearImpulseToCenter(b2Vec2(0.f, 350000.f), true);
				canJump.m_canJump = false;
			}
		}
	}

	if (Input::GetKeyDown(Key::E))
	{
		if (gameRun == true)
		{
			if (ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isWaterMelon == true)
			{
				seedEntity = Scene::CreateSeedProjectile(player.GetBody()->GetPosition().x, player.GetBody()->GetPosition().y);
				projectileStorage.push_back(seedEntity);
			}
			else if (ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isApple == true)
			{
				juiceEntity = Scene::CreateJuiceProjectile(player.GetBody()->GetPosition().x, player.GetBody()->GetPosition().y);
				projectileStorage.push_back(juiceEntity);
			}
			else if (ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isBanana == true)
			{
				peelEntity = Scene::CreatePeelProjectile(player.GetBody()->GetPosition().x, player.GetBody()->GetPosition().y);
				projectileStorage.push_back(peelEntity);
			}
		}
	}
	//if (Input::GetKeyDown(Key::G))
	//{
	//	if (gameRun == true)
	//	{
	//		//Set up player stats and sprite and switches the character with a keypress; temp solution until a better character selection system can be made.
	//		if (ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isWaterMelon == true)
	//		{
	//			ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isWaterMelon = false;
	//			ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isApple = true;
	//			ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).health = 3;
	//			ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).speed = 350000;
	//			fileName = "AppleTempSprite.jpg";
	//			ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).LoadSprite(fileName, 35, 35);
	//		}
	//		else if (ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isApple == true)
	//		{
	//			ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isApple = false;
	//			ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isBanana = true;
	//			ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).health = 2;
	//			ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).speed = 450000;
	//			fileName = "BananaTempSprite.jpg";
	//			ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).LoadSprite(fileName, 35, 35);
	//		}
	//		else if (ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isBanana == true)
	//		{
	//			ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isBanana = false;
	//			ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).isWaterMelon = true;
	//			ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).health = 5;
	//			ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).speed = 200000;
	//			fileName = "watermelon.png";
	//			ECS::GetComponent<Sprite>(MainEntities::MainPlayer()).LoadSprite(fileName, 35, 35);
	//		}
	//	}
	//}
	if (Input::GetKeyDown(Key::Escape))
	{
		exit(0);
	}

	if (Input::GetKeyDown(Key::M))
	{
		gameRun = false;
		mainMenu = true;
		controlsMenu = false;
		characterMenu = false;
		gameWin = false;
		gameOver = false;
	}
	if (Input::GetKeyDown(Key::J))
	{
		gameRun = false;
		mainMenu = false;
		controlsMenu = false;
		characterMenu = false;
		ECS::GetComponent<PlayerStats>(MainEntities::MainPlayer()).playerWon == true;
		gameOver = false;
	}
	if (Input::GetKeyDown(Key::K))
	{
		gameRun = false;
		mainMenu = false;
		controlsMenu = false;
		characterMenu = false;
		gameWin = false;
		gameOver = true;
	}
}

void CartCraze::KeyboardUp()
{
	

}
