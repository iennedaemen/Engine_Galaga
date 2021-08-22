#include "MiniginPCH.h"
#include "Boss.h"
#include "SpriteComponent.h"
#include "ColliderComponent.h"
#include "Laser.h"
#include "Time.h"
#include "ScreenInfo.h"
#include "Beam.h"

void Boss::Initialize()
{
	std::shared_ptr<SpriteComponent> pSpriteComp = std::make_shared<SpriteComponent>(120.0f, 64.0f, 2, 1);
	AddComponent(pSpriteComp);
	pSpriteComp->SetTexture("Boss.png");
	pSpriteComp->IsStatic(true);
	pSpriteComp->SetSpriteSheetTopLeft(0, 0);
	m_Rect = { m_Rect.x, m_Rect.y, 30, 32 };

	m_pBeam = std::shared_ptr<GameObject>(std::make_shared<Beam>());
	Add(m_pBeam);
	m_pBeam->SetPosition(float(m_Rect.x + m_Rect.w / 2), float(m_Rect.y));

	m_pLaser = std::shared_ptr<GameObject>(std::make_shared<Laser>(false));
	Add(m_pLaser);
	m_pLaser->SetPosition(-100, 0);

}

void Boss::Update()
{
	// LIVES
	if (m_IsHit)
	{
		m_Lives--;
		m_IsHit = false;
	}

	if (m_Lives == 1)
		GetComponent<SpriteComponent>()->SetCurrentFrame(1);

	// ACTIONS
	if (m_EnumState == State::Idle)
	{
		m_ActionTimer += Time::GetInstance().m_ElapsedSec;

		if (m_ActionTimer >= m_ActionTime)
		{
			m_ActionTimer = 0.0f;
			m_ActionTime = float(std::rand() % 10 + 15);

			if (m_NextAction) m_DoShootRun = true;
			else m_DoBeamRun = true;
		}
	}

	// LASER
	if (m_pLaser->GetRect().y > ScreenInfo::GetInstance().screenheigth + 20)
	{
		m_pLaser->m_Rect.x = -100;
		std::shared_ptr<Laser> dLaser = std::dynamic_pointer_cast<Laser> (m_pLaser);
		dLaser->m_IsActive = false;
	}

	// BEAM
	std::shared_ptr<Beam> dBeam = std::dynamic_pointer_cast<Beam>(m_pBeam);
	if (dBeam->m_IsActive)
		m_pBeam->SetPosition(float(m_Rect.x - 20), float(m_Rect.y + 40));

	// STATE
	std::shared_ptr<BossState> newState = nullptr;
	newState = m_pState->handleInput(*this);
	if (newState != nullptr)
		m_pState = newState;
	if (m_pState)
		m_pState->update(*this);

	if (m_EnumState == State::Idle)
		SetPosition(m_IdlePos.x, m_IdlePos.y);
}

const std::shared_ptr<GameObject> Boss::GetBeam()
{
	return m_pBeam;
}

void Boss::SetNextAction(bool doShootRun)
{
	m_NextAction = doShootRun;
}