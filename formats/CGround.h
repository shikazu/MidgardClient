#ifndef _CGROUND_H
#define _CGROUND_H
#include <vector>
#include "../common/FileStream.h"

class CGround///Handler for GND files
{
	public:
		struct Cell
		{
			float fHeight[4];//lower number means higher ground
			int32_t lTopSurface;//All three are IDs. If none for respective surface, value will be -1
			int32_t lFrontSurface;
			int32_t lRightSurface;
		};

		struct Surface
		{
			float u[4];
			float v[4];
			uint16_t wTexture;
			uint16_t wLightMap;
			uint8_t pColor[4];
		};

		struct Lightmap
		{
			uint8_t cBrightness[8*8];//a
			uint8_t cIntensity[8*8][3];//r,g,b
		};

		CGround(sf::String sFile);
		CGround(FileStream &flstream);
		virtual ~CGround();

		const char* GetTexture(uint32_t dwIndex) const;
		const Lightmap& GetLightmap(uint32_t dwIndex) const;
		const Surface& GetSurface(uint32_t dwIndex) const;
		const Cell& GetCell(uint32_t dwX, uint32_t dwY) const;

		const uint32_t GetTextureCount() const;
		const uint32_t GetLightmapCount() const;
		const uint32_t GetSurfaceCount() const;
		const uint32_t GetWidth() const;//Cell count is just product of width & height so not adding that
		const uint32_t GetHeight() const;
		const bool IsValid() const;

	private:
		bool bValid;
		uint16_t wVersion;
		uint32_t dwWidth;
		uint32_t dwHeight;
		float fZoom;
		std::vector<char*> vTextures;
		std::vector<Lightmap> vLightmaps;
		std::vector<Surface> vSurfaces;
		std::vector<Cell> vCells;

		void construct(FileStream &flstream);
		bool fetchLightmaps(FileStream &flstream, uint32_t dwLightmapCount);
		bool genLightmaps(FileStream &flstream, uint32_t dwLightmapCount);
};

#endif//_CGROUND_H
