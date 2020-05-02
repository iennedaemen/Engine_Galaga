#include "MiniginPCH.h"
#include "SpriteComponent.h"
#include "Renderer.h"
#include "Time.h"
#include "ResourceManager.h"


SpriteComponent::SpriteComponent(float width, float height, int rows, int cols, int framesPerSec, int beginFrame)
	: m_pTexture{nullptr}
	, m_SrcRect{}
	, m_DestRect{}
	, m_FrameTime{}
	, m_AccumulatedSec{}
	, m_SheetWidth{width}
	, m_SheetHeight{height}
	, m_SheetLeft{}
	, m_SheetTop{}
	, m_Cols{ cols }
	, m_Rows{rows}
	, m_FramesPerSec{framesPerSec}
	, m_CurFrame{beginFrame}
	, m_NrFramesToPlay{m_Rows}
	, m_IsStatic{false}
{
	Initialize();
}

void SpriteComponent::Initialize()
{
	m_FrameTime = 1.0f / float(m_FramesPerSec);

	InitSourceRect();
	InitDestinationRect();
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
		}
	}

}

void SpriteComponent::Render()
{
	m_DestRect.x = int(GetGameObject()->GetTransform().GetPosition().x);
	m_DestRect.y = int(GetGameObject()->GetTransform().GetPosition().y);
	Renderer::GetInstance().RenderTexture(*m_pTexture, m_DestRect, m_SrcRect);
}

void SpriteComponent::InitDestinationRect()
{
	m_DestRect.w = int(m_SheetWidth / float(m_Rows));
	m_DestRect.h = int(m_SheetHeight / float(m_Cols));

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

void SpriteComponent::SetTexture(const std::string& filename)
{
	m_pTexture.reset();
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
}

void SpriteComponent::SetTexture(const std::string& filename, float width, float height, int rows, int cols, int fps)
{
	m_pTexture = nullptr;
	m_pTexture = ResourceManager::GetInstance().LoadTexture(filename);
	m_SheetWidth = width;
	m_SheetHeight = height;
	m_Rows = rows;
	m_Cols = cols;
	m_FramesPerSec = fps;

	Initialize();
}

void SpriteComponent::IsStatic(bool isStatic)
{
	m_IsStatic = isStatic;
}

void SpriteComponent::SetCurrentFrame(int frame)
{
	m_CurFrame = frame;
}

void SpriteComponent::SetNrFramesToPlay(int nrFramesToPlay)
{
	m_NrFramesToPlay = nrFramesToPlay;
}

