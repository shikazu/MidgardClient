#include "CActor.h"
#include "../common/Globals.h"

CActor::CActor(sf::String sFile)
{
	FileStream flstream;
	GetPipe().getFileStream(sFile, flstream);
	construct(flstream);
}

CActor::CActor(FileStream &flstream)
{
	construct(flstream);
}

void CActor::construct(FileStream &flstream)
{
	uint16_t wSig;
	flstream.read(&wSig, 2);
	if (wSig != 0x4341)//AC
	{
		bValid = false;
		return;
	}

	uint16_t wActCount;
	flstream.read(&wVersion , 2);//Get version
	flstream.read(&wActCount, 2);//Get Number of Actions
	flstream.seek(10, flstream.CUR);//Skip 10 bytes of reserved area

	//Get all the action data
	vActions.reserve(wActCount);
	for (uint16_t i = 0; i < wActCount; i++)
	{
		Action action;
		fetchAction(flstream, action);
		vActions.push_back(std::move(action));
	}

	//Get All the Sounds
	if (wVersion >= 0x0201)
	{
		uint32_t dwSoundCount;
		flstream.read(&dwSoundCount, 4);
		vSounds.reserve(dwSoundCount);
		for (uint32_t i = 0; i < dwSoundCount; i++)
		{
			char* sSoundFile = new char[40];
			flstream.read(sSoundFile, 40);
			vSounds.push_back(sSoundFile);
		}
	}

	//Get the Delay values
	if (wVersion >= 0x0202)
	{
		uint16_t wActCount = vActions.size();
		for (uint16_t i = 0; i < wActCount; i++)
		{
			flstream.read(&(vActions.at(i).fDelay), 4);
		}
	}
	bValid = true;
}

CActor::~CActor()
{
	/*
	for (uint32_t i = 0; i < vActions.size(); i++)
	{
		Action* pAction = vActions.at(i);
		for (uint32_t j = 0; j < pAction->vFrames.size(); j++)
		{
			Frame* pFrame = pAction->vFrames.at(j);
			for (uint32_t k = 0; k < pFrame->vLayers.size(); k++)
			{
				Layer* pLayer = pFrame->vLayers.at(k);
				delete[] pLayer;
			}
			for (uint32_t k = 0; k < pFrame->vPositions.size(); k++)
			{
				Pos* pPosition = pFrame->vPositions.at(k);
				delete[] pPosition;
			}
			delete[] pFrame;
		}
		delete[] pAction;
	}
	*/
	for (uint32_t i = 0; i < vSounds.size(); i++)
	{
		delete[] vSounds.at(i);
	}
}

void CActor::fetchAction(FileStream &flstream, CActor::Action& action)
{
	//Get the Frames
	uint32_t dwFrameCount;
	flstream.read(&dwFrameCount, 4);
	action.vFrames.reserve(dwFrameCount);
	for (uint32_t i = 0; i < dwFrameCount; i++)
	{
		Frame frame;
		fetchFrame(flstream, frame);
		action.vFrames.push_back(std::move(frame));
	}
	//Set the delay for older versions
	if (wVersion < 0x0202)
	{
		action.fDelay = 4.0;
	}
}

void CActor::fetchFrame(FileStream &flstream, CActor::Frame &frame)
{
	flstream.seek(32, flstream.CUR);
	//Get the Layers
	uint32_t dwLayerCount;
	flstream.read(&dwLayerCount, 4);
	frame.vLayers.reserve(dwLayerCount);
	for (uint32_t i = 0; i < dwLayerCount; i++)
	{
		Layer layer;
		fetchLayer(flstream, layer);
		frame.vLayers.push_back(std::move(layer));
	}
	//Get the sound index
	if (wVersion >= 0x0200)
	{
		flstream.read(&(frame.lSoundIndex), 4);
	}
	else
	{
		frame.lSoundIndex = -1;
	}
	//Get the positions
	if (wVersion >= 0x0203)
	{
		uint32_t dwPosCount;
		flstream.read(&dwPosCount, 4);
		frame.vPositions.reserve(dwPosCount);
		for (uint32_t i = 0; i < dwPosCount; i++)
		{
			Pos pos;
			flstream.read(&pos, sizeof(Pos));
			frame.vPositions.push_back(std::move(pos));
		}
	}
}

void CActor::fetchLayer(FileStream &flstream, CActor::Layer &layer)
{
	flstream.read(&(layer.x), 4);
	flstream.read(&(layer.y), 4);
	flstream.read(&(layer.lSprNum), 4);
	uint32_t dwMirror;
	flstream.read(&(dwMirror), 4);
	if (wVersion >= 0x0200)
	{
		layer.color.r = flstream.readByte();
		layer.color.g = flstream.readByte();
		layer.color.b = flstream.readByte();
		layer.color.a = flstream.readByte();
		flstream.read(&(layer.fScaleX), 4);
		if (wVersion >= 0x0204)
		{
			flstream.read(&(layer.fScaleY), 4);
		}
		else
		{
			layer.fScaleY = layer.fScaleX;
		}
		if (dwMirror != 0)
		{
			layer.fScaleX = -layer.fScaleX;//Flip
		}
		flstream.read(&(layer.dwAngle), 4);
		flstream.read(&(layer.dwSprType), 4);
		if (wVersion >= 0x0205)
		{
			flstream.read(&(layer.dwWidth), 4);
			flstream.read(&(layer.dwHeight), 4);
		}
		else
		{
			layer.dwWidth = 0;
			layer.dwHeight = 0;
		}
	}
	else
	{
		layer.color = sf::Color::White;
		layer.fScaleX = 1.0;
		layer.fScaleY = 1.0;
		layer.dwAngle = 0;
		layer.dwSprType = 0;
		layer.dwWidth = 0;
		layer.dwHeight = 0;
	}
}

const CActor::Action& CActor::GetAction(uint16_t wAct) const
{
	return vActions.at(wAct);
}

const CActor::Frame& CActor::GetFrame(uint16_t wAct, uint32_t dwFrame) const
{
	return vActions.at(wAct).vFrames.at(dwFrame);
}

const CActor::Layer& CActor::GetLayer(uint16_t wAct, uint32_t dwFrame, uint32_t dwLayer) const
{
	return vActions.at(wAct).vFrames.at(dwFrame).vLayers.at(dwLayer);
}

const CActor::Pos& CActor::GetPos(uint16_t wAct, uint32_t dwFrame, uint32_t dwPos) const
{
	return vActions.at(wAct).vFrames.at(dwFrame).vPositions.at(dwPos);
}

const char* CActor::GetSound(uint32_t dwSound) const
{
	return vSounds.at(dwSound);
}

const bool CActor::IsValid() const
{
	return bValid;
}

const uint16_t CActor::GetActionCount() const
{
	return vActions.size();
}

const uint32_t CActor::GetFrameCount(uint16_t wAct) const
{
	return vActions.at(wAct).vFrames.size();
}

const uint32_t CActor::GetLayerCount(uint16_t wAct, uint32_t dwFrame) const
{
	return vActions.at(wAct).vFrames.at(dwFrame).vLayers.size();
}

const uint32_t CActor::GetSoundCount() const
{
	return vSounds.size();
}
