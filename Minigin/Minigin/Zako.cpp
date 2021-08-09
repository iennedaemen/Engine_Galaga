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
	pSpriteComp->SetSpriteSheetTopLeft(0, 0);
	m_Rect = { m_Rect.x, m_Rect.y, 20, 15 };

	m_pLaser = std::shared_ptr<GameObject>(std::make_shared<Laser>(false));
	Add(m_pLaser);
	m_pLaser->SetPosition(-100, 0);

}

void Zako::Update()
{
	// SHOOT RUN
	if (m_IsIdle)
	{
		m_ActionTimer += Time::GetInstance().m_ElapsedSec;

		if (m_ActionTimer >= m_ActionTime)
		{
			m_ActionTimer = 0.0f;
			m_ActionTime = float(std::rand() % 10 + 15);

			if (m_NextAction)
				m_DoShootRun = true;
			else m_DoCrashRun = true;

		}
	}

	// LASER

		if (m_pLaser->GetRect().y > ScreenInfo::GetInstance().screenheigth + 20)
		{
			m_pLaser->m_Rect.x = -100;
			std::shared_ptr<Laser> dLaser = std::dynamic_pointer_cast<Laser> (m_pLaser);
			dLaser->SetActive(false);
		}
	

	// STATE
	std::shared_ptr<ZakoState> newState = nullptr;
	newState = m_State->handleInput(*this);
	if (newState != nullptr)
	{
		m_State = newState;
	}
	if (m_State)
		m_State->update(*this);
}

void Zako::ShootLaser()
{
	std::shared_ptr<Laser> dLaser = std::dynamic_pointer_cast<Laser> (m_pLaser);
	if (!dLaser->IsActive())
	{
		dLaser->SetActive(true);
		m_pLaser->SetPosition(float(m_Rect.x + m_Rect.w / 2 - m_pLaser->m_Rect.w / 2), float(m_Rect.y));
		return;
	}
}