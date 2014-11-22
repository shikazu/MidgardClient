#include "CAltitude.h"
#include "../common/Globals.h"

CAltitude::CAltitude(sf::String sFile)
{
	FileStream flstream;
	GetPipe().getFileStream(sFile, flstream);
	construct(flstream);
}

CAltitude::CAltitude(FileStream &flstream)
{
	construct(flstream);
}

void CAltitude::construct(FileStream &flstream)
{
	uint32_t wSig;
	flstream.read(&wSig, 4);
	if (wSig != 0x54415247)//GRAT
	{
		bValid = false;
		return;
	}

	wVersion = (flstream.readByte() << 8 ) | flstream.readByte();
	if (wVersion != 0x0102)
	{
		bValid = false;
		return;
	}
	flstream.read(&dwWidth, 4);
	flstream.read(&dwHeight, 4);
	uint32_t dwCellCount = dwWidth * dwHeight;
	vCells.reserve(dwCellCount);
	for (uint32_t i = 0; i < dwCellCount; i++)
	{
		Cell cell;
		flstream.read(&cell, sizeof(Cell));
		vCells.push_back(std::move(cell));
	}
	bValid = true;
}

CAltitude::~CAltitude()
{
	/*
	for (uint32_t i = 0; i < vCells.size(); i++)
	{
		delete[] vCells.at(i);
	}
	*/
}

const CAltitude::Cell& CAltitude::GetCell(uint32_t dwX, uint32_t dwY) const
{
	return vCells.at(dwX + dwWidth*dwY);
}

const uint32_t CAltitude::GetWidth() const
{
	return dwWidth;
}

const uint32_t CAltitude::GetHeight() const
{
	return dwHeight;
}

const bool CAltitude::IsValid() const
{
	return bValid;
}
