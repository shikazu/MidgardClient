#ifndef _CACTOR_H
#define _CACTOR_H
#include <SFML/Graphics.hpp>
#include <vector>
#include "../common/FileStream.h"

class CActor ///Handler for ACT files
{
    public:
        struct Layer
        {
            uint32_t dwX;
            uint32_t dwY;
            uint32_t dwSprNum;
            uint32_t dwMirror;
            sf::Color color;
            float fScaleX;
            float fScaleY;
            uint32_t dwAngle;
            uint32_t dwSprType;
            uint32_t dwWidth;
            uint32_t dwHeight;
        };
        struct Pos
        {
            uint32_t dwReserved;
            uint32_t dwX;
            uint32_t dwY;
            uint32_t dwAttrib;
        };
        struct Frame
        {
            std::vector<CActor::Layer*> vLayers;
            std::vector<CActor::Pos*> vPositions;
            int32_t lSoundIndex;
        };
        struct Action
        {
            std::vector<CActor::Frame*> vFrames;
            float fDelay;
        };

        CActor(FileStream &flstream);
        virtual ~CActor();

        CActor::Pos*    GetPos(uint16_t wAct, uint32_t wFrame, uint32_t wPos);
        CActor::Layer*  GetLayer(uint16_t wAct, uint32_t wFrame, uint32_t wLayer);
        CActor::Frame*  GetFrame(uint16_t wAct, uint32_t wFrame);
        CActor::Action* GetAction(uint16_t wAct);
        char* GetSound(uint32_t dwSound);

        uint16_t GetActionCount();
        uint32_t GetSoundCount();
        bool IsValid();

    private:
        bool bValid;
        uint16_t wVersion;
        std::vector<CActor::Action*> vActions;
        std::vector<char*> vSounds;

        void fetchAction(FileStream &flstream, CActor::Action* pAction);
        void fetchFrame(FileStream &flstream, CActor::Frame* pFrame);
        void fetchLayer(FileStream &flstream, CActor::Layer* pLayer);
};

#endif//_CACTOR_H
