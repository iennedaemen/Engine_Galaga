#pragma once
#include "BaseComponent.h"
#include <SDL.h>
#include "TextComponent.h"

class FpsComponent final: public BaseComponent
{
public:
	FpsComponent(std::string font, int size, SDL_Color color);

protected:
	virtual void Update() override;
	virtual void Render() override {};
	virtual void Initialize() override;

private:
	int m_Fps = 0;
	std::shared_ptr<TextComponent> m_TextComp;
};

