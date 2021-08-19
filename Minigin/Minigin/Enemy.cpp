#include "MiniginPCH.h"
#include "Enemy.h"
#include "GameObject.h"
#include "Laser.h"

void Enemy::ShootLaser(std::shared_ptr<GameObject> obj)
{
	std::shared_ptr<Laser> dLaser = std::dynamic_pointer_cast<Laser> (m_pLaser);
	if (!dLaser->IsActive())
	{
		dLaser->SetActive(true);
		m_pLaser->SetPosition(float(obj->m_Rect.x + obj->m_Rect.w / 2 - m_pLaser->m_Rect.w / 2), float(obj->m_Rect.y));
		return;
	}
}