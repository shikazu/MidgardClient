#include "GActor.h"

GActor::GActor(const char* sFile)
{
    std::fstream *pFile =  new std::fstream(sFile, std::fstream::in | std::fstream::binary);
    uint16_t wSig;
    pFile->read((char*)&wSig, 2);
    if (wSig == 0x4341)
    {
        uint16_t wActCount;
        pFile->read((char*)&wVersion , 2);//Get version
        pFile->read((char*)&wActCount, 2);//Get Number of Actions
        pFile->seekg(10, pFile->cur);//Skip 10 bytes of reserved area

        //Get all the action data
        for (uint16_t i = 0; i < wActCount; i++)
        {
            Action* pAction = (Action*)malloc(sizeof(Action));
            fetchAction(pFile, pAction);
            vActions.push_back(pAction);
        }

        //Get All the Sounds
        if (wVersion >= 0x0201)
        {
            uint32_t dwSoundCount;
            pFile->read((char*)&(dwSoundCount), 4);
            for (uint32_t i = 0; i < dwSoundCount; i++)
            {
                char* sSoundFile = (char*)malloc(40);
                pFile->read(sSoundFile, 40);
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
                pFile->read((char*)&(pAction->fDelay), 4);
            }
        }

        bValid = true;
    }
    else
    {
        bValid = false;
    }
    pFile->close();
}

GActor::~GActor()
{
}

void GActor::fetchAction(std::fstream* pFile, GActor::Action* pAction)
{
    //Get the Frames
    uint32_t dwFrameCount;
    pFile->read((char*)&dwFrameCount, 4);
    pFile->seekg(32, pFile->cur);
    for (uint32_t i = 0; i < dwFrameCount; i++)
    {
        Frame* pFrame = (Frame*)malloc(sizeof(Frame));
        fetchFrame(pFile, pFrame);
        pAction->vFrames.push_back(pFrame);
    }
    //Set the delay for older versions
    if (wVersion < 0x0202)
    {
        pAction->fDelay = 4.0;
    }
}

void GActor::fetchFrame(std::fstream* pFile, GActor::Frame* pFrame)
{
    //Get the Layers
    uint32_t dwLayerCount;
    pFile->read((char*)&dwLayerCount, 4);
    for (uint32_t i = 0; i < dwLayerCount; i++)
    {
        Layer* pLayer = (Layer*)malloc(sizeof(Layer));
        fetchLayer(pFile, pLayer);
        pFrame->vLayers.push_back(pLayer);
    }
    //Get the sound index
    if (wVersion >= 0x0200)
    {
        pFile->read((char*)&(pFrame->lSoundIndex), 4);
    }
    else
    {
        pFrame->lSoundIndex = -1;
    }
    //Get the positions
    if (wVersion >= 0x0203)
    {
        uint32_t dwPosCount;
        pFile->read((char*)&dwPosCount, 4);
        for (uint32_t i = 0; i < dwPosCount; i++)
        {
            Pos* pPos = (Pos*)malloc(sizeof(Pos));
            pFile->read((char*)pPos, sizeof(Pos));
        }
    }
}

void GActor::fetchLayer(std::fstream* pFile, GActor::Layer* pLayer)
{
    pFile->read((char*)&(pLayer->dwX), 4);
    pFile->read((char*)&(pLayer->dwY), 4);
    pFile->read((char*)&(pLayer->dwSprNum), 4);
    pFile->read((char*)&(pLayer->dwMirror), 4);
    if (wVersion >= 0x0200)
    {
        uint8_t rgba[4];
        pFile->read((char*)rgba, 4);
        pLayer->color.r = rgba[0];
        pLayer->color.g = rgba[1];
        pLayer->color.b = rgba[2];
        pLayer->color.a = rgba[3];
        pFile->read((char*)&(pLayer->fScaleX), 4);
        if (wVersion >= 0x0204)
        {
            pFile->read((char*)&(pLayer->fScaleY), 4);
        }
        else
        {
            pLayer->fScaleY = pLayer->fScaleX;
        }
        pFile->read((char*)&(pLayer->dwAngle), 4);
        pFile->read((char*)&(pLayer->dwSprType), 4);
        if (wVersion >= 0x0205)
        {
            pFile->read((char*)&(pLayer->dwWidth), 4);
            pFile->read((char*)&(pLayer->dwHeight), 4);
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

GActor::Action* GActor::GetAction(uint16_t wAct)
{
    return vActions.at(wAct);
}

GActor::Frame* GActor::GetFrame(uint16_t wAct, uint32_t wFrame)
{
    return GetAction(wAct)->vFrames.at(wFrame);
}

GActor::Layer* GActor::GetLayer(uint16_t wAct, uint32_t wFrame, uint32_t wLayer)
{
    return GetFrame(wAct, wFrame)->vLayers.at(wLayer);
}

GActor::Pos* GActor::GetPos(uint16_t wAct, uint32_t wFrame, uint32_t wPos)
{
    return GetFrame(wAct, wFrame)->vPositions.at(wPos);
}

char* GActor::GetSound(uint32_t dwSound)
{
    return vSounds.at(dwSound);
}
