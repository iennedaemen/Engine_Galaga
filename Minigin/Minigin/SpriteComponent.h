#pragma once
#include "BaseComponent.h"
#include <SDL.h>
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)

class Texture2D;

class SpriteComponent final : public BaseComponent
{
public:
	SpriteComponent(float width, float height, int rows, int cols, int beginFrame = 0);

	void SetTexture(const std::string& filename);
	void SetTexture(const std::string& filename, float width, float height, int rows, int cols);
	void SetSpriteSheetTopLeft(float left, float top);
	glm::vec2 GetSpriteSheetTopLeft();
	void IsStatic(bool isStatic);
	void SetCurrentFrame(int frame);
	void SetNrFramesToPlay(int nrFramesToPlay);

	void SetPlayAnimOnce(bool playAnimOnce)
	{
		m_PlayOnce = playAnimOnce;
	}

	bool IsAnimPlayed()
	{
		return m_PlayedAnim;
	}
	void SetIsAnimPlayed(bool isPlayed)
	{
		m_PlayedAnim = isPlayed;
	}

protected:
	virtual void Update() override;
	virtual void Render() override;
	virtual  void Initialize() override;

private:
	std::shared_ptr<Texture2D> m_pTexture;
	SDL_Rect m_SrcRect;
	float m_FrameTime;
	float m_AccumulatedSec;
	float m_SheetWidth, m_SheetHeight;
	float m_SheetLeft, m_SheetTop;
	int m_Cols, m_Rows;
	int m_CurFrame;
	int m_NrFramesToPlay;
	bool m_IsStatic;
	bool m_PlayOnce = false;
	bool m_PlayedAnim = false;
	
	// FUNCTIONS
	void InitDestinationRect();
	void InitSourceRect();
	void UpdateSourceRect();
};
