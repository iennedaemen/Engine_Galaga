#include "MiniginPCH.h"
#include "SpriteComponent.h"
#include "Renderer.h"
#include "Time.h"
#include "ResourceManager.h"
#include "ScreenInfo.h"
#include "GameObject.h"


SpriteComponent::SpriteComponent(float width, float height, int rows, int cols, int beginFrame)
	: m_pTexture{nullptr}
	, m_SrcRect{}
	, m_FrameTime{}
	, m_AccumulatedSec{}
	, m_SheetWidth{width}
	, m_SheetHeight{height}
	, m_SheetLeft{}
	, m_SheetTop{}
	, m_Cols{ cols }
	, m_Rows{rows}
	, m_CurFrame{beginFrame}
	, m_NrFramesToPlay{m_Rows}
	, m_IsStatic{false}
{
	Initialize();
}

void SpriteComponent::Initialize()
{
	m_FrameTime = 1.0f / ScreenInfo::GetInstance().framesPerSecond;

	InitSourceRect();
}

void SpriteComponent::Update()
{

	if (!m_IsStatic)
	{
		m_AccumulatedSec += Time::GetInstance().m_ElapsedSec;
		if (m_AccumulatedSec > m_FrameTime)
		{
			++m_CurFrame %= m_NrFramesToPlay;
			m_AccumulatedSec -= m_FrameTime;
			UpdateSourceRect();
			if (m_PlayOnce)
			{
				if (m_CurFrame == m_NrFramesToPlay - 1)
				{
					m_PlayedAnim = true;
				}
			}
		}
	}

}

void SpriteComponent::Render()
{
	float angle = GetGameObject()->GetTransform()->GetRotation();
	Renderer::GetInstance().RenderTexture(*m_pTexture, GetGameObject()->GetRect(), m_SrcRect, angle);
}

void SpriteComponent::InitSourceRect()
{
	m_SrcRect.w = int(m_SheetWidth / float(m_Rows));
	m_SrcRect.h = int(m_SheetHeight / float(m_Cols));
	UpdateSourceRect();
}

void SpriteComponent::UpdateSourceRect()
{
	m_SrcRect.x = int(m_SheetLeft + float(m_CurFrame) * float(m_SrcRect.w));
	m_SrcRect.y = int(m_SheetTop);
}

void SpriteComponent::SetSpriteSheetTopLeft(float left, float top)
{
	m_SheetLeft = m_SheetWidth / float(m_Rows) * left;
	m_SheetTop = m_SheetHeight / float(m_Cols) * top;
	UpdateSourceRect();
}

glm::vec2 SpriteComponent::GetSpriteSheetTopLeft()
{
	return { m_SheetLeft, m_SheetTop };
}

void SpriteComponent::SetTexture(const std::string& filename)
{
	m_pTexture.reset();
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}

void SpriteComponent::SetTexture(const std::string& filename, float width, float height, int rows, int cols)
{
	m_pTexture = nullptr;
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
	m_SheetWidth = width;
	m_SheetHeight = height;
	m_Rows = rows;
	m_Cols = cols;

	Initialize();
}

void SpriteComponent::IsStatic(bool isStatic)
{
	m_IsStatic = isStatic;
}

void SpriteComponent::SetCurrentFrame(int frame)
{
	m_CurFrame = frame;
	UpdateSourceRect();
}

void SpriteComponent::SetNrFramesToPlay(int nrFramesToPlay)
{
	m_NrFramesToPlay = nrFramesToPlay;
}

void SpriteComponent::UpdateRender()
{
	float angle = GetGameObject()->GetTransform()->GetRotation();
	Renderer::GetInstance().RenderTexture(*m_pTexture, GetGameObject()->GetRect(), m_SrcRect, angle);
}