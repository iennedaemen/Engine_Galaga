#include "MiniginPCH.h"
#include "TestScene.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "FpsComponent.h"

TestScene::TestScene()
	: Scene("TestScene")
{
	
}


void TestScene::Initialize()
{
	auto go = std::make_shared<GameObject>();
	std::shared_ptr<RenderComponent> pRenderComp = std::make_shared<RenderComponent>();
	go->AddComponent(pRenderComp);
	pRenderComp->SetTexture("background.jpg");
	Add(go);

	go = std::make_shared<GameObject>();
	pRenderComp = std::make_shared<RenderComponent>();
	go->AddComponent(pRenderComp);
	pRenderComp->SetTexture("logo.png");
	Add(go);
	go->SetPosition(216, 180);

	go = std::make_shared<GameObject>();
	std::shared_ptr<TextComponent> pTextComp = std::make_shared<TextComponent>("Lingua.otf", 36);
	go->AddComponent(pTextComp);
	pTextComp->SetText("Programming 4 Assignment");
	Add(go);
	go->SetPosition(80, 20);

	go = std::make_shared<GameObject>();
	std::shared_ptr<FpsComponent> pFpsComp = std::make_shared<FpsComponent>(FpsComponent("Lingua.otf", 20, { 255, 255, 0, 255 }));
	go->AddComponent(pFpsComp);
	Add(go);
	go->SetPosition(10, 10);
}

void TestScene::Update()
{
	
}

void TestScene::Render() const
{
	
}
