#include "MiniginPCH.h"
#include "Minigin.h"
#include <chrono>
#include <thread>
#include "InputManager.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include <SDL.h>
#include "TextObject.h"
#include "GameObject.h"
#include "Scene.h"
#include "Time.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "FpsComponent.h"

using namespace std;
using namespace std::chrono;

void Minigin::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_Window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		640,
		480,
		SDL_WINDOW_OPENGL
	);
	if (m_Window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_Window);
}

/**
 * Code constructing the scene world starts here
 */
void Minigin::LoadGame() const
{
	auto& scene = SceneManager::GetInstance().CreateScene("Demo");

	auto go = std::make_shared<GameObject>();
	std::shared_ptr<RenderComponent> pRenderComp = std::make_shared<RenderComponent>();
	go->AddComponent(pRenderComp);
	pRenderComp->SetTexture("background.jpg");
	scene.Add(go);

	go = std::make_shared<GameObject>();
	pRenderComp = std::make_shared<RenderComponent>();
	go->AddComponent(pRenderComp);
	pRenderComp->SetTexture("logo.png");
	scene.Add(go);
	go->SetPosition(216, 180);

	go = std::make_shared<GameObject>();
	std::shared_ptr<TextComponent> pTextComp = std::make_shared<TextComponent>("Lingua.otf", 36);
	go->AddComponent(pTextComp);
	pTextComp->SetText("Programming 4 Assignment");
	scene.Add(go);
	go->SetPosition(80, 20);

	go = std::make_shared<GameObject>();
	std::shared_ptr<FpsComponent> pFpsComp = std::make_shared<FpsComponent>(FpsComponent("Lingua.otf", 20, {255, 255, 0, 255}));
	go->AddComponent(pFpsComp);
	scene.Add(go);
	go->SetPosition(10, 10);
}

void Minigin::Cleanup()
{
	Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(m_Window);
	m_Window = nullptr;
	SDL_Quit();
}

void Minigin::Run()
{
	Initialize();

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Data/");

	LoadGame();

	{
		auto& renderer = Renderer::GetInstance();
		auto& sceneManager = SceneManager::GetInstance();
		auto& input = InputManager::GetInstance();
		auto lastTime = std::chrono::high_resolution_clock::now();
		float lag = 0.0f;

		bool doContinue = true;
		while (doContinue)
		{
			const auto currentTime = high_resolution_clock::now();
			float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
			lastTime = currentTime;
			lag += deltaTime;
			Time::GetInstance().SetElapsedSec(deltaTime);

			doContinue = input.ProcessInput();
			while (lag >= 0.2f)
			{
				sceneManager.Update();
				lag -= 0.2f;
			}
			renderer.Render();

			auto sleepTime = duration_cast<duration<float>>(currentTime + milliseconds(MsPerFrame) - high_resolution_clock::now());
			this_thread::sleep_for(sleepTime);
		}
	}

	Cleanup();
}
