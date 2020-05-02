#pragma once
#include "BaseComponent.h"
#include <SDL.h>

class Texture2D;

class SpriteComponent final : public BaseComponent
{
public:
	SpriteComponent(float width, float height, int rows, int cols, int framesPerSec, int beginFrame = 0);

	void SetTexture(const std::string& filename);
	void SetTexture(const std::string& filename, float width, float height, int rows, int cols, int fps);
	void SetSpriteSheetTopLeft(float left, float top);
	void IsStatic(bool isStatic);
	void SetCurrentFrame(int frame);
	void SetNrFramesToPlay(int nrFramesToPlay);

protected:
	virtual void Update() override;
	virtual void Render() override;
	virtual  void Initialize() override;

private:
	std::shared_ptr<Texture2D> m_pTexture;
	SDL_Rect m_SrcRect, m_DestRect;
	float m_FrameTime;
	float m_AccumulatedSec;
	float m_SheetWidth, m_SheetHeight;
	float m_SheetLeft, m_SheetTop;
	int m_Cols, m_Rows;
	int m_FramesPerSec;
	int m_CurFrame;
	int m_NrFramesToPlay;
	bool m_IsStatic;
	
	// FUNCTIONS
	void InitDestinationRect();
	void InitSourceRect();
	void UpdateSourceRect();
};
