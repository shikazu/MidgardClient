#include "CActor.h"

CActor::CActor(FileStream &flstream)
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
        Action* pAction = new Action;
        fetchAction(flstream, pAction);
        vActions.push_back(pAction);
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
            Action* pAction = vActions.at(i);
            flstream.read(&(pAction->fDelay), 4);
        }
    }
    bValid = true;
}

CActor::~CActor()
{
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
    for (uint32_t i = 0; i < vSounds.size(); i++)
    {
        delete[] vSounds.at(i);
    }
}

void CActor::fetchAction(FileStream &flstream, CActor::Action* pAction)
{
    //Get the Frames
    uint32_t dwFrameCount;
    flstream.read(&dwFrameCount, 4);
    pAction->vFrames.reserve(dwFrameCount);
    for (uint32_t i = 0; i < dwFrameCount; i++)
    {
        Frame* pFrame = new Frame;
        fetchFrame(flstream, pFrame);
        pAction->vFrames.push_back(pFrame);
    }
    //Set the delay for older versions
    if (wVersion < 0x0202)
    {
        pAction->fDelay = 4.0;
    }
}

void CActor::fetchFrame(FileStream &flstream, CActor::Frame* pFrame)
{
    flstream.seek(32, flstream.CUR);
    //Get the Layers
    uint32_t dwLayerCount;
    flstream.read(&dwLayerCount, 4);
    pFrame->vLayers.reserve(dwLayerCount);
    for (uint32_t i = 0; i < dwLayerCount; i++)
    {
        Layer* pLayer = new Layer;
        fetchLayer(flstream, pLayer);
        pFrame->vLayers.push_back(pLayer);
    }
    //Get the sound index
    if (wVersion >= 0x0200)
    {
        flstream.read(&(pFrame->lSoundIndex), 4);
    }
    else
    {
        pFrame->lSoundIndex = -1;
    }
    //Get the positions
    if (wVersion >= 0x0203)
    {
        uint32_t dwPosCount;
        flstream.read(&dwPosCount, 4);
        pFrame->vPositions.reserve(dwPosCount);
        for (uint32_t i = 0; i < dwPosCount; i++)
        {
            Pos* pPos = new Pos;
            flstream.read(pPos, sizeof(Pos));
            pFrame->vPositions.push_back(pPos);
        }
    }
}

void CActor::fetchLayer(FileStream &flstream, CActor::Layer* pLayer)
{
    flstream.read(&(pLayer->dwX), 4);
    flstream.read(&(pLayer->dwY), 4);
    flstream.read(&(pLayer->dwSprNum), 4);
    flstream.read(&(pLayer->dwMirror), 4);
    if (wVersion >= 0x0200)
    {
        uint8_t rgba[4];
        flstream.read(rgba, 4);
        pLayer->color.r = rgba[0];
        pLayer->color.g = rgba[1];
        pLayer->color.b = rgba[2];
        pLayer->color.a = rgba[3];
        flstream.read(&(pLayer->fScaleX), 4);
        if (wVersion >= 0x0204)
        {
            flstream.read(&(pLayer->fScaleY), 4);
        }
        else
        {
            pLayer->fScaleY = pLayer->fScaleX;
        }
        flstream.read(&(pLayer->dwAngle), 4);
        flstream.read(&(pLayer->dwSprType), 4);
        if (wVersion >= 0x0205)
        {
            flstream.read(&(pLayer->dwWidth), 4);
            flstream.read(&(pLayer->dwHeight), 4);
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
