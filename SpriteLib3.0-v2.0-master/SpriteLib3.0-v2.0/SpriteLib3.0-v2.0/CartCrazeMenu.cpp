#include "CartCrazeMenu.h"
#include "Utilities.h"

CartCrazeMenu::CartCrazeMenu(std::string name)
	: Scene(name)
{
	//No gravity this is a menu
	m_gravity = b2Vec2(0.f, 0.f);
	m_physicsWorld->SetGravity(m_gravity);

}

////Background
//void backgroundPic(int x)
//{
//	/*Scene::CreateSprite(m_sceneReg, "HelloWorld.png", 100, 60, 0.5f, vec3(0.f, 0.f, 0.f));*/
//
//	//Creates entity
//	auto entity = ECS::CreateEntity();
//
//	//Add components
//	ECS::AttachComponent<Sprite>(entity);
//	ECS::AttachComponent<Transform>(entity);
//
//	//Set up the components
//	std::string fileName = "HelloWorld.png";
//	ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 820, 600);
//	ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
//	ECS::GetComponent<Transform>(entity).SetPosition(vec3(x, 20.f, 0.f));
//}

void CartCrazeMenu::InitScene(float windowWidth, float windowHeight)
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

	//Set up menu components
	{
		auto entity = ECS::CreateEntity();

		//Attach components
		ECS::AttachComponent<Sprite>(entity);
		ECS::AttachComponent<Transform>(entity);

		//Set up components
		std::string fileName = "Start_Button.png";
		ECS::GetComponent<Sprite>(entity).LoadSprite(fileName, 820, 600);
		ECS::GetComponent<Sprite>(entity).SetTransparency(1.f);
		ECS::GetComponent<Transform>(entity).SetPosition(vec3(20.f, 20.f, 0.f));

	}

	////Generate the background
	//{
	//	int xBackDistRight = 400;
	//	int xBackDistLeft = -400;

	//	for (int i = 0; i < 25; i++) {

	//		backgroundPic(xBackDistRight);
	//		xBackDistRight += 820;

	//	}

	//	for (int i = 0; i < 25; i++) {

	//		backgroundPic(xBackDistLeft);
	//		xBackDistLeft -= 820;

	//	}
	//}
}

void CartCrazeMenu::Update()
{

}

//void CartCrazeMenu::GUI()
//{
//	GUIWindowUI();
//
//	if (m_firstWindow)
//	{
//		GUIWindowOne();
//	}
//
//	if (m_secondWindow)
//	{
//		GUIWindowTwo();
//	}
//}
//
//void CartCrazeMenu::GUIWindowUI()
//{
//	ImGui::Begin("Test");
//
//	ImGui::Checkbox("Enable First Window", &m_firstWindow);
//	ImGui::SameLine();
//	if (ImGui::Button("Toggle Second Window", ImVec2(180.f, 15.f)))
//	{
//		m_secondWindow = !m_secondWindow;
//	}
//
//	ImGui::End();
//}
//
//void CartCrazeMenu::GUIWindowOne()
//{
//	//Window begin
//	ImGui::Begin("Side Docked Window");
//	//is the buffer initialized
//	static bool init = false;
//
//	//Buffers
//	const unsigned int BUF_LEN = 512;
//	static char buf[BUF_LEN];
//	//Image load
//	static std::string imageLoad = "LinkStandby.png";
//	//World gravity slider
//	float gravity[2] = { m_physicsWorld->GetGravity().x, m_physicsWorld->GetGravity().y };
//
//	if (!init)
//	{
//		memset(buf, 0, BUF_LEN);
//		init = true;
//	}
//	m_physicsWorld->SetAllowSleeping(true);
//
//	//ImGui content
//	if (ImGui::TreeNode("Vignette Effect"))
//	{
//		if (EffectManager::GetVignetteHandle() != -1)
//		{
//			if (ImGui::TreeNode("Properties"))
//			{
//				VignetteEffect* vig = (VignetteEffect*)EffectManager::GetEffect(EffectManager::GetVignetteHandle());
//				float innerRad = vig->GetInnerRadius();
//				float outerRad = vig->GetOuterRadius();
//				float opacity = vig->GetOpacity();
//
//				if (ImGui::SliderFloat("Inner Radius", &innerRad, 0.f, outerRad))
//				{
//					vig->SetInnerRadius(innerRad);
//				}
//				if (ImGui::SliderFloat("Outer Radius", &outerRad, innerRad, 1.f))
//				{
//					vig->SetOuterRadius(outerRad);
//				}
//				if (ImGui::SliderFloat("Opacity", &opacity, 0.f, 1.f))
//				{
//					vig->SetOpacity(opacity);
//				}
//
//				ImGui::TreePop();
//			}
//
//			if (ImGui::Button("Remove Effect", ImVec2(100.f, 30.f)))
//			{
//				EffectManager::RemoveEffect(EffectManager::GetVignetteHandle());
//			}
//		}
//		else
//		{
//			if (ImGui::Button("Apply Effect", ImVec2(100.f, 30.f)))
//			{
//				EffectManager::CreateEffect(Vignette, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
//			}
//		}
//
//
//		ImGui::TreePop();
//	}
//
//	if (ImGui::TreeNode("Sepia Effect"))
//	{
//		if (EffectManager::GetSepiaHandle() != -1)
//		{
//			if (ImGui::TreeNode("Properties"))
//			{
//				SepiaEffect* sep = (SepiaEffect*)EffectManager::GetEffect(EffectManager::GetSepiaHandle());
//				float intensity = sep->GetIntensity();
//
//				if (ImGui::SliderFloat("Inner Radius", &intensity, 0.f, 1.f))
//				{
//					sep->SetIntensity(intensity);
//				}
//
//				ImGui::TreePop();
//			}
//
//			if (ImGui::Button("Remove Effect", ImVec2(100.f, 30.f)))
//			{
//				EffectManager::RemoveEffect(EffectManager::GetSepiaHandle());
//			}
//		}
//		else
//		{
//			if (ImGui::Button("Apply Effect", ImVec2(100.f, 30.f)))
//			{
//				EffectManager::CreateEffect(Sepia, BackEnd::GetWindowWidth(), BackEnd::GetWindowHeight());
//			}
//		}
//
//
//		ImGui::TreePop();
//	}
//
//	ImGui::Separator();
//
//	ImGui::Text("Editing World Variables!");
//	if (ImGui::SliderFloat2("World Gravity", gravity, -1000.f, 1000.f, "%.2f"))
//	{
//		m_physicsWorld->SetGravity(b2Vec2(gravity[0], gravity[1]));
//		m_physicsWorld->SetAllowSleeping(false);
//	}
//
//	ImGui::Separator();
//
//	ImGui::Text("Displaying image dynamically");
//	ImGui::InputText("Input filename for image", buf, BUF_LEN, ImGuiInputTextFlags_CharsNoBlank);
//
//	if (ImGui::Button("Confirm File", ImVec2(100.f, 30.f)))
//	{
//		imageLoad = buf;
//	}
//
//	ImGui::Image((void*)(intptr_t)TextureManager::FindTexture(imageLoad)->GetID(), ImVec2(150.f, 150.f), ImVec2(0, 1), ImVec2(1, 0));
//
//	ImGui::Separator();
//
//	ImGui::Text("Editing Colors");
//	ImGui::ColorPicker4("Scene Background Color", &m_clearColor.x);
//
//	ImGui::End();
//}
//
//void CartCrazeMenu::GUIWindowTwo()
//{
//	//Second docked window
//	ImGui::Begin("Second Window");
//	//Image load
//	static std::string imageLoad1 = "FunnyButton.png";
//	static std::string imageLoad2 = "LikeButton.png";
//	static std::string hahaPressed = "";
//	static std::string likePressed = "";
//
//	ImGui::BeginTabBar("Tab Bar Test");
//
//	if (ImGui::BeginTabItem("Tab 1"))
//	{
//		ImGui::Text("You are within Tab 1");
//
//		if (ImGui::ImageButton((void*)(intptr_t)TextureManager::FindTexture(imageLoad1)->GetID(), ImVec2(100.f, 100.f), ImVec2(0, 1), ImVec2(1, 0)))
//		{
//			hahaPressed = "You shouldn't have pressed that...";
//		}
//
//		ImGui::Text("%s", hahaPressed.c_str());
//
//		ImGui::EndTabItem();
//	}
//
//	if (ImGui::BeginTabItem("Tab 2"))
//	{
//		ImGui::Text("You are within Tab 2");
//
//		if (ImGui::ImageButton((void*)(intptr_t)TextureManager::FindTexture(imageLoad2)->GetID(), ImVec2(100.f, 100.f), ImVec2(0, 1), ImVec2(1, 0)))
//		{
//			likePressed = "LIKED!!!";
//		}
//
//		ImGui::Text("%s", likePressed.c_str());
//
//		ImGui::EndTabItem();
//	}
//
//	ImGui::EndTabBar();
//
//	ImGui::End();
//}




void CartCrazeMenu::KeyboardHold()
{

	if (Input::GetKey(Key::A))
	{


	}
	if (Input::GetKey(Key::D))
	{

	}

	if (Input::GetKey(Key::W))
	{

	}
	if (Input::GetKey(Key::S))
	{

	}

}

void CartCrazeMenu::KeyboardDown()
{

}

void CartCrazeMenu::KeyboardUp()
{


}
