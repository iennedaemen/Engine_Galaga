#include "MiniginPCH.h"
#include "Zako.h"
#include "SpriteComponent.h"
#include "ColliderComponent.h"
#include "Laser.h"
#include "Time.h"
#include "ScreenInfo.h"

void Zako::Initialize()
{
	std::shared_ptr<SpriteComponent> pSpriteComp = std::make_shared<SpriteComponent>(52.0f, 40.0f, 1, 1);
	AddComponent(pSpriteComp);
	pSpriteComp->SetTexture("Zako.png");
	pSpriteComp->IsStatic(true);
	m_Rect = { m_Rect.x, m_Rect.y, 52, 40 };
	GetTransform()->SetScale(0.4f);

	m_pLaser = std::shared_ptr<GameObject>(std::make_shared<Laser>(false));
	Add(m_pLaser);
	m_pLaser->SetPosition(-100, 0);
}

void Zako::Update()
{
	// ACTIONS
	if (m_EnumState == State::Idle)
	{
		m_ActionTimer += Time::GetInstance().m_ElapsedSec;

		if (m_ActionTimer >= m_ActionTime)
		{
			m_ActionTimer = 0.0f;
			m_ActionTime = float(std::rand() % 10 + 15);

			if (m_NextAction) m_DoShootRun = true;
			else m_DoCrashRun = true;
		}
	}

	// LASER
	if (m_pLaser->GetRect().y > ScreenInfo::GetInstance().screenheigth + 20)
	{
		m_pLaser->m_Rect.x = -100;
		std::shared_ptr<Laser> dLaser = std::dynamic_pointer_cast<Laser> (m_pLaser);
		dLaser->m_IsActive = false;
	}

	// STATE
	std::shared_ptr<ZakoState> newState = nullptr;
	newState = m_pState->HandleState(*this);
	if (newState != nullptr)
		m_pState = newState;
	if (m_pState)
		m_pState->Update(*this);

	if (m_EnumState == State::Idle)
		SetPosition(m_IdlePos.x, m_IdlePos.y);
}

void Zako::SetNextAction(bool doShootRun)
{
	m_NextAction = doShootRun;
}