#ifndef _CMODEL_H
#define _CMODEL_H
#include <vector>
#include "../common/FileStream.h"

class CModel///Handler for RSM files
{
    public:
        struct Vertex
        {
            float x;
            float y;
            float z;
        };
        struct Texture
        {
            uint32_t dwColor;
            float u;
            float v;
        };
        struct Face
        {
            uint16_t wVertIDs[3];
            uint16_t wTexVertIDs[3];
            uint16_t wTexID;
            uint16_t wPadding;
            int32_t lDoubleSide;
            int32_t lSmoothGroup;
        };
        struct KeyFrame
        {
            int32_t lFrame;
            float x;
            float y;
            float z;
            float w;
        };
        struct Node
        {
            char sName[40];
            char sParent[40];
            std::vector<int32_t> vTexIDs;

            float fOffsetMT[9];//3x3 Matrix that Identifies the axis and origin of this node
            Vertex pos_;//dummy?
            Vertex pos;
            float fAngle;
            Vertex axis;
            Vertex scale;

            std::vector<Vertex*> vVertices;
            std::vector<Texture*> vTextures;
            std::vector<Face*>vFaces;
            std::vector<KeyFrame*>vPosFrames;
            std::vector<KeyFrame*>vRotFrames;
        };

        CModel(FileStream &flstream);
        virtual ~CModel();

        uint32_t GetNodeCount();
        CModel::Node* GetNode(uint32_t dwIndex);
        uint32_t GetTextureCount();
        char* GetTexture(uint32_t dwIndex);
        bool IsValid();

    private:
        bool bValid;
        uint16_t wVersion;
        int32_t lAnimLength;
        int32_t lShadeType;
        uint8_t uAlpha;
        std::vector<char*> vTextures;
        char sNode[40];
        std::vector<Node*> vNodes;

        void fetchNode(FileStream& flstream, CModel::Node* pNode);
};

#endif//_CMODEL_H
