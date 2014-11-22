#ifndef _CALTITUDE_H
#define _CALTITUDE_H
#include <vector>
#include "../common/FileStream.h"

class CAltitude///Handler for GAT files
{
	public:
		enum SurfaceType//suffixes -> W means walkable, S means snipable
		{
			LAND_WS = 0,//The LAND prefix is generic - for e.g. It could also mean the air in the case of sky maps
			LAND_S,
			WATER,
			WATER_WS,
			WATER_S,
			CLIFF_S,
			CLIFF
			//Remaining values are unknown (if used at all)
		};

		struct Cell
		{
			float fHeight[4];
			SurfaceType type;
		};

		CAltitude(sf::String sFile);
		CAltitude(FileStream &flstream);
		virtual ~CAltitude();

		const Cell& GetCell(uint32_t dwX, uint32_t dwY) const;
		const uint32_t GetWidth() const;
		const uint32_t GetHeight() const;
		const bool IsValid() const;

	private:
		bool bValid;
		uint16_t wVersion;
		uint32_t dwWidth;
		uint32_t dwHeight;
		std::vector<Cell> vCells;

		void construct(FileStream &flstream);
};

#endif//_CALTITUDE_H
