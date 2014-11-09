#include "CModel.h"

CModel::CModel(FileStream &flstream)
{
    uint32_t wSig;
    flstream.read(&wSig, 4);
    if (wSig != 0x4D535247)//GRSM
    {
        bValid = false;
        return;
    }
    uint16_t wVersion;
    flstream.read(&wVersion, 2);

    flstream.read(&lAnimLength, 4);
    flstream.read(&lShadeType, 4);
    if (wVersion >= 0x0104)
    {
        uAlpha = flstream.readByte();
    }
    else
    {
        uAlpha = 0xFF;
    }
    flstream.seek(16, flstream.CUR);

    //Load the Textures
    int32_t lTexCount;
    flstream.read(&lTexCount, 4);
    if (lTexCount > 0)
    {
        vTextures.reserve(lTexCount);
        for (int32_t i = 0; i < lTexCount; i++)
        {
            char* sTexture = new char[40];
            flstream.read(sTexture, 40);
            sTexture[39] = 0;
            vTextures.push_back(sTexture);
        }
    }

    //Load the Nodes
    flstream.read(&sNode, 40);//Main Node

    int32_t lNodeCount;
    flstream.read(&lNodeCount, 4);
    if (lNodeCount > 0)
    {
        vNodes.reserve(lNodeCount);
        for (int32_t i = 0; i < lNodeCount; i++)
        {
            Node* pNode = new Node;
            fetchNode(flstream, pNode, wVersion);
            vNodes.push_back(pNode);
        }
    }
    bValid = true;
}

CModel::~CModel()
{
}

bool CModel::IsValid()
{
    return bValid;
}

void CModel::fetchNode(FileStream &flstream, CModel::Node* pNode, uint16_t wVersion)
{
    flstream.read(pNode->sName, 40);
    flstream.read(pNode->sParent, 40);
    int32_t lTexCount;
    flstream.read(&lTexCount, 4);
    if (lTexCount > 0)
    {
        pNode->vTexIDs.reserve(lTexCount);
        for (int32_t i = 0; i < lTexCount; i++)
        {
            flstream.read(&(pNode->vTexIDs.at(i)), 4);
        }
    }
    flstream.read(&(pNode->fOffsetMT), 9*4);
    flstream.read(&(pNode->pos_), sizeof(Vertex));
    flstream.read(&(pNode->pos) , sizeof(Vertex));
    flstream.read(&(pNode->fAngle), 4);
    flstream.read(&(pNode->axis), sizeof(Vertex));
    flstream.read(&(pNode->scale), sizeof(Vertex));

    //Node Vertices
    int32_t lVertCount;
    flstream.read(&lVertCount, 4);
    if (lVertCount > 0)
    {
        pNode->vVertices.reserve(lVertCount);
        for (int32_t i = 0; i < lVertCount; i++)
        {
            Vertex* pVertex = new Vertex;
            flstream.read(pVertex, sizeof(Vertex));
            pNode->vVertices.push_back(pVertex);
        }
    }

    //Texture Vertices
    int32_t lTexVCount;
    flstream.read(&lTexVCount, 4);
    if (lTexVCount > 0)
    {
        pNode->vTextures.reserve(lTexVCount);
        for (int32_t i = 0; i < lTexVCount; i++)
        {
            Texture* pTexture = new Texture;
            if (wVersion > 0x0102)
            {
                flstream.read(pTexture, sizeof(Texture));
            }
            else
            {
                pTexture->dwColor = 0xFFFFFFFF;
                flstream.read(&(pTexture->u), 4);
                flstream.read(&(pTexture->v), 4);
            }
            pNode->vTextures.push_back(pTexture);
        }
    }

    //Faces
    int32_t lFaceCount;
    flstream.read(&lFaceCount, 4);
    if (lFaceCount > 0)
    {
        pNode->vFaces.reserve(lFaceCount);
        for (int32_t i = 0; i < lFaceCount; i++)
        {
            Face* pFace = new Face;
            if (wVersion >= 0x0102)
            {
                flstream.read(pFace, sizeof(Face));
            }
            else
            {
                flstream.read(pFace, sizeof(Face)-4);//SmoothGroup is not included
                pFace->lSmoothGroup = 0;
            }
            pNode->vFaces.push_back(pFace);
        }
    }

    //Position KeyFrames
    if (wVersion >= 0x0105)
    {
        int32_t lPosKeyCount = 0;
        flstream.read(&lPosKeyCount, 4);
        if (lPosKeyCount > 0)
        {
            pNode->vPosFrames.reserve(lPosKeyCount);
            for (int32_t i = 0; i < lPosKeyCount; i++)
            {
                KeyFrame* pFrame = new KeyFrame;
                flstream.read(pFrame, sizeof(KeyFrame) - 4);//w is not there
                pFrame->w = 0;
                pNode->vPosFrames.push_back(pFrame);
            }
        }
    }

    //Rotation KeyFrames
    int32_t lRotKeyCount;
    flstream.read(&lRotKeyCount, 4);
    if (lRotKeyCount > 0)
    {
        pNode->vRotFrames.reserve(lRotKeyCount);
        for (int32_t i = 0; i < lRotKeyCount; i++)
        {
            KeyFrame* pFrame = new KeyFrame;
            flstream.read(pFrame, sizeof(KeyFrame));
            pNode->vRotFrames.push_back(pFrame);
        }
    }
}
