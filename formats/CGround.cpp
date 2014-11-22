#include "CGround.h"
#include "../common/Globals.h"

CGround::CGround(sf::String sFile)
{
	FileStream flstream;
	GetPipe().getFileStream(sFile, flstream);
	construct(flstream);
}

CGround::CGround(FileStream &flstream)
{
	construct(flstream);
}

void CGround::construct(FileStream &flstream)
{
	uint32_t wSig;
	flstream.read(&wSig, 4);
	if (wSig != 0x4E475247)//GRGN
	{
		bValid = false;
		return;
	}

	wVersion = (flstream.readByte() << 8 ) | flstream.readByte();
	if (wVersion != 0x0106)
	{
		bValid = false;
		return;
	}
	flstream.read(&dwWidth, 4);
	flstream.read(&dwHeight, 4);
	flstream.read(&fZoom, 4);

	//Read Textures
	uint32_t dwTextureCount, dwTextureNameLen;
	flstream.read(&dwTextureCount, 4);
	flstream.read(&dwTextureNameLen, 4);
	vTextures.reserve(dwTextureCount);
	for (uint32_t i = 0; i < dwTextureCount; i++)
	{
		char* sTexture = new char[dwTextureNameLen + 1];
		sTexture[dwTextureNameLen] = 0;
		flstream.read(sTexture, dwTextureNameLen);
		vTextures.push_back(sTexture);
	}

	//Fetch/Generate Lightmaps
	uint32_t dwLightmapCount;
	flstream.read(&dwLightmapCount, 4);
	if (wVersion >= 0x0107)
	{
		bValid = fetchLightmaps(flstream, dwLightmapCount);
	}
	else
	{
		bValid = genLightmaps(flstream, dwLightmapCount);
	}

	if (!bValid)
	{
		return;
	}

	//Read surfaces
	uint32_t dwSurfaceCount;
	flstream.read(&dwSurfaceCount, 4);
	vSurfaces.reserve(dwSurfaceCount);
	for (uint32_t i = 0; i < dwSurfaceCount; i++)
	{
		Surface surface;
		flstream.read(&surface, sizeof(Surface));
		vSurfaces.push_back(std::move(surface));
	}

	//Read Cells
	uint32_t dwCellCount = dwWidth * dwHeight;
	vCells.reserve(dwCellCount);
	for (uint32_t i = 0; i < dwCellCount; i++)
	{
		Cell cell;
		if (wVersion >= 0x0107)
		{
			flstream.read(&cell, sizeof(Cell));
		}
		else//Surface IDs are 16 bit
		{
			uint16_t wTopSurface, wFrontSurface, wRightSurface;
			flstream.read(cell.fHeight, sizeof(cell.fHeight));
			flstream.read(&wTopSurface, 2);
			flstream.read(&wFrontSurface, 2);
			flstream.read(&wRightSurface, 2);

			cell.lTopSurface   = wTopSurface;
			cell.lFrontSurface = wFrontSurface;
			cell.lRightSurface = wRightSurface;
		}
		vCells.push_back(std::move(cell));
	}
	//bValid should already be true from earlier Lightmap creation
}

CGround::~CGround()
{
	for (uint32_t i = 0; i < vTextures.size(); i++)
	{
		delete[] vTextures.at(i);
	}
	/*
	for (uint32_t i = 0; i < vLightmaps.size(); i++)
	{
		delete[] vLightmaps.at(i);
	}
	for (uint32_t i = 0; i < vSurfaces.size(); i++)
	{
		delete[] vSurfaces.at(i);
	}
	for (uint32_t i = 0; i < vCells.size(); i++)
	{
		delete[] vCells.at(i);
	}
	*/
}

bool CGround::fetchLightmaps(FileStream &flstream, uint32_t dwLightmapCount)
{
	uint32_t dwLMwidth, dwLMHeight, dwLMCells;//Should be 8, 8, 1
	flstream.read(&dwLMwidth, 4);
	flstream.read(&dwLMHeight, 4);
	flstream.read(&dwLMCells, 4);
	if (dwLMwidth != 8 || dwLMHeight != 8 || dwLMCells != 1)
	{
		return false;
	}

	vLightmaps.reserve(dwLightmapCount);
	for (uint32_t i = 0; i < dwLightmapCount; i++)
	{
		Lightmap lm;
		flstream.read(&lm, sizeof(Lightmap));
		vLightmaps.push_back(std::move(lm));
	}
	return true;
}

bool CGround::genLightmaps(FileStream &flstream, uint32_t dwLightmapCount)
{
	uint8_t pLightMapIndices[dwLightmapCount][4];
	flstream.read(pLightMapIndices, dwLightmapCount * 4);

	uint32_t dwColorChannels;
	flstream.read(&dwColorChannels, 4);

	char sColorChannels[dwColorChannels][40];
	flstream.read(sColorChannels, dwColorChannels * 40);

	for (uint32_t i = 0; i < dwLightmapCount; i++)
	{
		uint8_t* pLMIndex = pLightMapIndices[i];
		if (pLMIndex[0] >= dwColorChannels || pLMIndex[1] >= dwColorChannels || pLMIndex[2] >= dwColorChannels || pLMIndex[3] >= dwColorChannels)
		{
			return false;
		}
		char *sChannel_a = sColorChannels[pLMIndex[0]], *sChannel_r = sColorChannels[pLMIndex[1]];
		char *sChannel_g = sColorChannels[pLMIndex[2]], *sChannel_b = sColorChannels[pLMIndex[3]];

		Lightmap lm;
		uint32_t aux = 0;
		for (uint32_t j = 0;  j < 64; j++, aux+=5)
		{
			uint32_t dwLM_i = (j/8) + 8*(j%8);//0-56,1-57,...7-63 where each of the ranges will be seperated by 8
			uint32_t dwLow = aux%8, dwHigh = aux/8;
			uint8_t a, r, g, b;
			a = (sChannel_a[dwHigh] & (0xF8 >> dwLow)) << dwLow;
			r = (sChannel_r[dwHigh] & (0xF8 >> dwLow)) << dwLow;
			g = (sChannel_g[dwHigh] & (0xF8 >> dwLow)) << dwLow;
			b = (sChannel_b[dwHigh] & (0xF8 >> dwLow)) << dwLow;
			if (dwLow >= 4)
			{
				a += ((0xF8 << (8-dwLow)) & sChannel_a[dwHigh+1]) >> (8-dwLow);
				r += ((0xF8 << (8-dwLow)) & sChannel_r[dwHigh+1]) >> (8-dwLow);
				g += ((0xF8 << (8-dwLow)) & sChannel_g[dwHigh+1]) >> (8-dwLow);
				b += ((0xF8 << (8-dwLow)) & sChannel_b[dwHigh+1]) >> (8-dwLow);
			}
			lm.cBrightness[dwLM_i] = a;
			lm.cIntensity[dwLM_i][0] = r;
			lm.cIntensity[dwLM_i][1] = g;
			lm.cIntensity[dwLM_i][2] = b;
		}
		vLightmaps.push_back(std::move(lm));
	}
	return true;
}

const char* CGround::GetTexture(uint32_t dwIndex) const
{
	return vTextures.at(dwIndex);
}

const CGround::Lightmap& CGround::GetLightmap(uint32_t dwIndex) const
{
	return vLightmaps.at(dwIndex);
}

const CGround::Surface& CGround::GetSurface(uint32_t dwIndex) const
{
	return vSurfaces.at(dwIndex);
}

const CGround::Cell& CGround::GetCell(uint32_t dwX, uint32_t dwY) const
{
	return vCells.at(dwX + dwWidth * dwY);
}

const uint32_t CGround::GetTextureCount() const
{
	return vTextures.size();
}

const uint32_t CGround::GetLightmapCount() const
{
	return vLightmaps.size();
}

const uint32_t CGround::GetSurfaceCount() const
{
	return vSurfaces.size();
}

const uint32_t CGround::GetWidth() const
{
	return dwWidth;
}

const uint32_t CGround::GetHeight() const
{
	return dwHeight;
}

const bool CGround::IsValid() const
{
	return bValid;
}
