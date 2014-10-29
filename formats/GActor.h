#ifndef _FORMATS_GACTOR_H
#define _FORMATS_GACTOR_H
#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>

class GActor//Handler for ACT files
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
            std::vector<GActor::Layer*> vLayers;
            std::vector<GActor::Pos*> vPositions;
            int32_t lSoundIndex;
        };
        struct Action
        {
            std::vector<GActor::Frame*> vFrames;
            float fDelay;
        };

        GActor(const char* sFile);
        GActor(std::istream &stream);
        virtual ~GActor();
        bool CheckValid();
        GActor::Pos*    GetPos(uint16_t wAct, uint32_t wFrame, uint32_t wPos);
        GActor::Layer*  GetLayer(uint16_t wAct, uint32_t wFrame, uint32_t wLayer);
        GActor::Frame*  GetFrame(uint16_t wAct, uint32_t wFrame);
        GActor::Action* GetAction(uint16_t wAct);
        char* GetSound(uint32_t dwSound);

    private:
        bool bValid;
        uint16_t wVersion;
        std::vector<GActor::Action*> vActions;
        std::vector<char*> vSounds;

        void init(std::istream &stream);

        void fetchAction(std::istream &stream, GActor::Action* pAction);
        void fetchFrame(std::istream &stream, GActor::Frame* pFrame);
        void fetchLayer(std::istream &stream, GActor::Layer* pLayer);
};

#endif//_FORMATS_GACTOR_H
