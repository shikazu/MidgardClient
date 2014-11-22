#include "SpriteRenderer.h"
#include <iostream>

SpriteRenderer::SpriteRenderer(CSprite *pSprite, CActor *pActor):sf::Drawable()
{
	sprite = pSprite;
	actor = pActor;
	wAct = 0;
	dwFrame = 0;
	bUpdateGeometry = true;
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::SetAction(uint16_t wAct, uint32_t dwFrame)
{
	this->wAct = wAct;
	this->dwFrame = dwFrame;
	bUpdateGeometry = true;
}

void SpriteRenderer::UpdateFrame(bool bForward)
{
	if (bForward)
	{
		dwFrame++;
		if (dwFrame >= actor->GetFrameCount(wAct))
		{
			dwFrame = 0;
		}
	}
	else
	{
		if (dwFrame == 0)
		{
			dwFrame = actor->GetFrameCount(wAct)-1;
		}
		else
		{
			dwFrame--;
		}
	}
	bUpdateGeometry = true;
}

CSprite* SpriteRenderer::GetSprite()
{
	return sprite;
}

CActor* SpriteRenderer::GetActor()
{
	return actor;
}

void SpriteRenderer::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (bUpdateGeometry)
	{
		updateTransforms();
		bUpdateGeometry = false;
	}

	uint32_t dwLayerCount = vTransforms.size();
	sf::Transform refTransform = states.transform;

	for (uint32_t i = 0; i < dwLayerCount; i++)
	{
		CActor::Layer layer = actor->GetLayer(wAct, dwFrame, i);
		if (layer.lSprNum < 0) continue;
		sf::VertexArray va(sf::Quads, 4);
		const sf::Texture* pTexture = sprite->GetTexture(layer.lSprNum);
		sf::Vector2u vSize = pTexture->getSize();

		va[0] = sf::Vertex(sf::Vector2f(0      ,       0), layer.color, sf::Vector2f(0      ,       0));
		va[1] = sf::Vertex(sf::Vector2f(vSize.x,       0), layer.color, sf::Vector2f(vSize.x,       0));
		va[2] = sf::Vertex(sf::Vector2f(vSize.x, vSize.y), layer.color, sf::Vector2f(vSize.x, vSize.y));
		va[3] = sf::Vertex(sf::Vector2f(0      , vSize.y), layer.color, sf::Vector2f(0      , vSize.y));

		states.texture = pTexture;
		states.transform = vTransforms.at(i) * refTransform;
		states.blendMode = sf::BlendMode::BlendAlpha;

		target.draw(va, states);
	}
}

const uint16_t SpriteRenderer::GetActIndex() const
{
	return wAct;
}

const uint32_t SpriteRenderer::GetFrameIndex() const
{
	return dwFrame;
}

const uint32_t SpriteRenderer::GetFrameCount() const
{
	return actor->GetFrameCount(wAct);
}

const float SpriteRenderer::GetDelay() const
{
	return actor->GetAction(wAct).fDelay;
}

void SpriteRenderer::updateTransforms() const
{
	uint32_t dwLayerCount = actor->GetLayerCount(wAct, dwFrame);
	vTransforms.clear();
	vTransforms.reserve(dwLayerCount);

	for (uint32_t i = 0; i < dwLayerCount; i++)
	{
		sf::Transform tr;
		CActor::Layer layer = actor->GetLayer(wAct, dwFrame, i);
		if (layer.lSprNum >= 0)
		{
			sf::Vector2u vCenter = sprite->GetTexture(layer.lSprNum)->getSize()/(uint32_t)2;
			int32_t x = layer.x - vCenter.x;
			int32_t y = layer.y - vCenter.y;

			tr.scale(layer.fScaleX, layer.fScaleY, vCenter.x, vCenter.y);
			tr.rotate(layer.dwAngle, vCenter.x, vCenter.y);
			tr.translate(x, y);
		}
		vTransforms.push_back(std::move(tr));
	}
}
