#pragma once
#include "BaseComponent.h"

class Texture2D;

class RenderComponent : public BaseComponent
{
public:
	RenderComponent();

	void SetTexture(const std::string& filename, float width, float height);
	void SetTexture(const std::string& filename);

protected:
	virtual void Update() override {};
	virtual void Render() override;
	virtual  void Initialize() override {};

private:
	std::shared_ptr<Texture2D> m_pTexture;
	float m_Width, m_Height;
};

