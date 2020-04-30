#pragma once
#include "BaseComponent.h"
#include <SDL.h>
#include "Font.h"

class TextComponent : public BaseComponent
{
public:
	TextComponent(std::string font, int size = 1, SDL_Color color = { 255,255,255,255 });

	void SetFont(std::string font, int size);
	void SetText(std::string text);
	void SetColor(SDL_Color color);

protected:
	virtual void Update() override;
	virtual void Render() override;
	virtual  void Initialize() override {};


private:
	std::shared_ptr<std::string> m_pText;
	std::shared_ptr<Font> m_pFont;
	std::shared_ptr<Texture2D> m_pTexture;
	SDL_Color m_Color;
};

