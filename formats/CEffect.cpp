#include "CEffect.h"
#include "../common/Globals.h"

CEffect::CEffect(sf::String sFile)
{
	FileStream flstream;
	GetPipe().getFileStream(sFile, flstream);
	construct(flstream);
}

CEffect::CEffect(FileStream &flstream)
{
	construct(flstream);
}

void CEffect::construct(FileStream &flstream)
{
	uint32_t wSig;
	flstream.read(&wSig, 4);
	if (wSig != 0x4D525453)//STRM
	{
		bValid = false;
		return;
	}

	uint32_t dwFrameCount, dwFPS, dwLayerCount;//not sure what this frame count is for
	flstream.read(&dwFrameCount, 4);
	flstream.read(&dwFPS, 4);
	flstream.read(&dwLayerCount, 4);
	vLayers.reserve(dwLayerCount);

	flstream.seek(16, flstream.CUR);
	for (uint32_t i = 0; i < dwLayerCount; i++)
	{
		Layer layer;
		fetchLayer(flstream, layer);
		vLayers.push_back(std::move(layer));
	}
	bValid = true;
}

CEffect::~CEffect()
{
	/*
	for (uint32_t i = 0; i < vLayers.size(); i++)
	{
		Layer* pLayer = vLayers.at(i);
		for (uint32_t j = 0; j < pLayer->vFrames.size(); j++)
		{
			Frame* pFrame = pLayer->vFrames.at(j);
			delete[] pFrame;
		}
		for (uint32_t j = 0; j < pLayer->vImages.size(); j++)
		{
			delete[] pLayer->vImages.at(j);
		}
		delete[] pLayer;
	}
	*/
}

void CEffect::fetchLayer(FileStream &flstream, CEffect::Layer &layer)
{
	uint32_t dwImageCount;//Texture count
	flstream.read(&dwImageCount, 4);
	layer.vImages.reserve(dwImageCount);
	for (uint32_t i = 0; i < dwImageCount; i++)
	{
		char* sFile = new char[256];
		flstream.read(sFile, 256);
		layer.vImages.push_back(sFile);
	}

	uint32_t dwFrameCount;//KeyFrame Count
	flstream.read(&dwFrameCount, 4);
	layer.vFrames.reserve(dwFrameCount);
	for (uint32_t i = 0; i < dwFrameCount; i++)
	{
		Frame frame;
		flstream.read(&frame, sizeof(Frame));
		layer.vFrames.push_back(std::move(frame));
	}
}

const CEffect::Layer& CEffect::GetLayer(uint32_t dwLayer) const
{
	return vLayers.at(dwLayer);
}

const uint32_t CEffect::GetLayerCount() const
{
	return vLayers.size();
}

const CEffect::Frame& CEffect::GetFrame(uint32_t dwLayer, uint32_t dwFrame) const
{
	return vLayers.at(dwLayer).vFrames.at(dwFrame);
}

const uint32_t CEffect::GetFrameCount(uint32_t dwLayer) const
{
	return vLayers.at(dwLayer).vFrames.size();
}

const char* CEffect::GetImageName(uint32_t dwLayer, uint32_t dwImage) const
{
	return vLayers.at(dwLayer).vImages.at(dwImage);
}

const uint32_t CEffect::GetImageCount(uint32_t dwLayer) const
{
	return vLayers.at(dwLayer).vImages.size();
}

const bool CEffect::IsValid() const
{
	return bValid;
}
