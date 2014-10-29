#include "CActor.h"

#include <iostream>

CActor::CActor(const char* sFile)
{
    std::fstream stream(sFile, std::fstream::in | std::fstream::binary);
    bValid = construct(stream);
    stream.close();
}

CActor::CActor(std::istream &stream)
{
    bValid = construct(stream);
}

bool CActor::construct(std::istream &stream)
{
    uint16_t wSig;
    stream.read((char*)&wSig, 2);
    if (wSig != 0x4341)//AC
    {
        return false;
    }

    uint16_t wActCount;
    stream.read((char*)&wVersion , 2);//Get version
    stream.read((char*)&wActCount, 2);//Get Number of Actions
    stream.seekg(10, stream.cur);//Skip 10 bytes of reserved area

    //Get all the action data
    vActions.reserve(wActCount);
    for (uint16_t i = 0; i < wActCount; i++)
    {
        Action* pAction = new Action;
        fetchAction(stream, pAction);
        vActions.push_back(pAction);
    }

    //Get All the Sounds
    if (wVersion >= 0x0201)
    {
        uint32_t dwSoundCount;
        stream.read((char*)&(dwSoundCount), 4);
        vSounds.reserve(dwSoundCount);
        for (uint32_t i = 0; i < dwSoundCount; i++)
        {
            char* sSoundFile = (char*)malloc(40);
            stream.read(sSoundFile, 40);
            vSounds.push_back(sSoundFile);
        }
    }

    //Get the Delay values
    if (wVersion >= 0x0202)
    {
        uint16_t wActCount = vActions.size();
        for (uint16_t i = 0; i < wActCount; i++)
        {
            Action* pAction = vActions.at(i);
            stream.read((char*)&(pAction->fDelay), 4);
        }
    }
    return true;
}

CActor::~CActor()
{
}

void CActor::fetchAction(std::istream &stream, CActor::Action* pAction)
{
    //Get the Frames
    uint32_t dwFrameCount;
    stream.read((char*)&dwFrameCount, 4);
    pAction->vFrames.reserve(dwFrameCount);
    for (uint32_t i = 0; i < dwFrameCount; i++)
    {
        Frame* pFrame = new Frame;
        fetchFrame(stream, pFrame);
        pAction->vFrames.push_back(pFrame);
    }
    //Set the delay for older versions
    if (wVersion < 0x0202)
    {
        pAction->fDelay = 4.0;
    }
}

void CActor::fetchFrame(std::istream &stream, CActor::Frame* pFrame)
{
    stream.seekg(32, stream.cur);
    //Get the Layers
    uint32_t dwLayerCount;
    stream.read((char*)&dwLayerCount, 4);
    pFrame->vLayers.reserve(dwLayerCount);
    for (uint32_t i = 0; i < dwLayerCount; i++)
    {
        Layer* pLayer = new Layer;
        fetchLayer(stream, pLayer);
        pFrame->vLayers.push_back(pLayer);
    }
    //Get the sound index
    if (wVersion >= 0x0200)
    {
        stream.read((char*)&(pFrame->lSoundIndex), 4);
    }
    else
    {
        pFrame->lSoundIndex = -1;
    }
    //Get the positions
    if (wVersion >= 0x0203)
    {
        uint32_t dwPosCount;
        stream.read((char*)&dwPosCount, 4);
        pFrame->vPositions.reserve(dwPosCount);
        for (uint32_t i = 0; i < dwPosCount; i++)
        {
            Pos* pPos = new Pos;
            stream.read((char*)pPos, sizeof(Pos));
            pFrame->vPositions.push_back(pPos);
        }
    }
}

void CActor::fetchLayer(std::istream &stream, CActor::Layer* pLayer)
{
    stream.read((char*)&(pLayer->dwX), 4);
    stream.read((char*)&(pLayer->dwY), 4);
    stream.read((char*)&(pLayer->dwSprNum), 4);
    stream.read((char*)&(pLayer->dwMirror), 4);
    if (wVersion >= 0x0200)
    {
        uint8_t rgba[4];
        stream.read((char*)rgba, 4);
        pLayer->color.r = rgba[0];
        pLayer->color.g = rgba[1];
        pLayer->color.b = rgba[2];
        pLayer->color.a = rgba[3];
        stream.read((char*)&(pLayer->fScaleX), 4);
        if (wVersion >= 0x0204)
        {
            stream.read((char*)&(pLayer->fScaleY), 4);
        }
        else
        {
            pLayer->fScaleY = pLayer->fScaleX;
        }
        stream.read((char*)&(pLayer->dwAngle), 4);
        stream.read((char*)&(pLayer->dwSprType), 4);
        if (wVersion >= 0x0205)
        {
            stream.read((char*)&(pLayer->dwWidth), 4);
            stream.read((char*)&(pLayer->dwHeight), 4);
        }
        else
        {
            pLayer->dwWidth = 0;
            pLayer->dwHeight = 0;
        }
    }
    else
    {
        pLayer->color = sf::Color::White;
        pLayer->fScaleX = 1.0;
        pLayer->fScaleY = 1.0;
        pLayer->dwAngle = 0;
        pLayer->dwSprType = 0;
        pLayer->dwWidth = 0;
        pLayer->dwHeight = 0;
    }
}

CActor::Action* CActor::GetAction(uint16_t wAct)
{
    return vActions.at(wAct);
}

CActor::Frame* CActor::GetFrame(uint16_t wAct, uint32_t wFrame)
{
    return GetAction(wAct)->vFrames.at(wFrame);
}

CActor::Layer* CActor::GetLayer(uint16_t wAct, uint32_t wFrame, uint32_t wLayer)
{
    return GetFrame(wAct, wFrame)->vLayers.at(wLayer);
}

CActor::Pos* CActor::GetPos(uint16_t wAct, uint32_t wFrame, uint32_t wPos)
{
    return GetFrame(wAct, wFrame)->vPositions.at(wPos);
}

char* CActor::GetSound(uint32_t dwSound)
{
    return vSounds.at(dwSound);
}

bool CActor::IsValid()
{
    return bValid;
}

uint16_t CActor::GetActionCount()
{
    return vActions.size();
}

uint32_t CActor::GetSoundCount()
{
    return vSounds.size();
}
