#include "CWorld.h"
#include "../common/Globals.h"

CWorld::CWorld(sf::String sFile)
{
	FileStream flstream;
	GetPipe().getFileStream(sFile, flstream);
	construct(flstream);
}

CWorld::CWorld(FileStream &flstream)
{
	construct(flstream);
}

void CWorld::construct(FileStream &flstream)
{
	uint32_t wSig;
	flstream.read(&wSig, 4);
	if (wSig != 0x57535247)//GRSW
	{
		bValid = false;
		return;
	}

	wVersion = (flstream.readByte() << 8 ) | flstream.readByte();
	if (wVersion < 0x0102 || wVersion > 0x0201)
	{
		bValid = false;
		return;
	}

	//Get the Filenames
	flstream.read(sIni, 40);
	flstream.read(sGnd, 40);
	if (wVersion >= 0x0104)
	{
		flstream.read(sGat, 40);
	}
	else
	{
		sGat[0] = 0;
	}
	flstream.read(sScr, 40);
	sIni[39] = sGnd[39] = sGat[39] = sScr[39] = 0;//Guarantee Null termination

	//------ WATER --------
	if (wVersion >= 0x0108)
	{
		flstream.read(&(water.lType), 4);
		flstream.read(&(water.fWaveHeight), 4*3);//Reads all 3 together wave height, speed and pitch
	}
	else
	{
		water.lType = 0;
		water.fWaveHeight =  1.0f;
		water.fWaveSpeed  =  2.0f;
		water.fWavePitch  = 50.0f;
	}
	if (wVersion >= 0x0109)
	{
		flstream.read(&(water.lAnimSpeed), 4);
	}
	else
	{
		water.lAnimSpeed = 3;
	}

	//------ LIGHT --------
	if (wVersion >= 0x0105)
	{
		flstream.read(&(light.lLongitude), 4*2);//Reads Longitude and Latitude
		flstream.read(&(light.fDiffuseColor), 4*3*2);//Reads both colors
	}
	else
	{
		light.lLatitude = 45;
		light.lLongitude = 45;
		light.fDiffuseColor[0] = light.fDiffuseColor[1] = light.fDiffuseColor[2] = 1.0f;
		light.fAmbientColor[0] = light.fAmbientColor[1] = light.fAmbientColor[2] = 0.3f;
	}
	if (wVersion >= 0x0107)
	{
		flstream.seek(4, flstream.CUR);
	}

	//------ GROUND --------
	if (wVersion >= 0x0106)
	{
		flstream.read(&ground, 4*2);
	}
	else
	{
		ground.lTop	= -500;
		ground.lBottom = 500;
		ground.lLeft   = -500;
		ground.lRight  = 500;
	}

	//------ OBJECTS --------
	int32_t lObjCount;
	flstream.read(&lObjCount, 2);
	if (lObjCount > 0)
	{
		vObjects.reserve(lObjCount);
		for (int32_t i = 0; i < lObjCount; i++)
		{
			Object obj;
			flstream.read(&(obj.type), 2);
			switch(obj.type)
			{
				case OBJECT_MODEL:
				{
					ObjModel* pModel = &(obj.model);
					if (wVersion >= 0x0103)
					{
						flstream.read(pModel->sName, 40);
						flstream.read(&(pModel->lAnimType), 2);
						flstream.read(&(pModel->fAnimSpeed), 4);
						flstream.read(&(pModel->lBlockType), 2);
						pModel->sName[39] = 0;
						if (pModel->fAnimSpeed < 0 || pModel->fAnimSpeed > 100.0f)
						{
							pModel->fAnimSpeed = 1.0f;
						}
					}
					else
					{
						pModel->sName[0] = 0;
						pModel->lAnimType = 0;
						pModel->fAnimSpeed = 1.0f;
						pModel->lBlockType = 0;
					}
					flstream.read(pModel->sModel, 80);
					flstream.read(pModel->sNode, 80);
					flstream.read(&(pModel->fPos), 4);
					flstream.read(&(pModel->fRot), 4);
					flstream.read(&(pModel->fScale), 4);
					pModel->sModel[79] = 0;
					pModel->sNode[79] = 0;
					break;
				}
				case OBJECT_LIGHT:
				{
					ObjLight* pLight = &(obj.light);
					flstream.read(pLight->sName, 80);
					flstream.read(&(pLight->fPos), 4*3);
					flstream.read(&(pLight->lRGBA), 4*3);
					flstream.read(&(pLight->fRange), 4);
					pLight->sName[39] = 0;
					break;
				}
				case OBJECT_SOUND:
				{
					ObjSound* pSound = &(obj.sound);
					flstream.read(pSound->sName, 80);
					flstream.read(pSound->sWavName, 80);
					flstream.read(&(pSound->fPos), 4);
					flstream.read(&(pSound->fVol), 4);
					flstream.read(&(pSound->lWidth), 4);
					flstream.read(&(pSound->lHeight), 4);
					flstream.read(&(pSound->fRange), 4);
					if (wVersion >= 0x0200)
					{
						flstream.read(&(pSound->fCycle), 4);
					}
					else
					{
						pSound->fCycle = 4.0f;
					}
					pSound->sName[79] = 0;
					pSound->sWavName[79] = 0;
					break;
				}
				case OBJECT_EFFECT:
				{
					ObjEffect* pEffect = &(obj.effect);
					flstream.read(pEffect->sName, 80);
					flstream.read(&(pEffect->fPos), 4*3);
					flstream.read(&(pEffect->lType), 4);
					flstream.read(&(pEffect->fEmitSpeed), 4);
					flstream.read(&(pEffect->fParam), 4*4);

					pEffect->sName[79] = 0;
					break;
				}
			}
			vObjects.push_back(std::move(obj));
		}
	}

	//------------ QUADTREE ---------------
	if (wVersion >= 0x0201)
	{
		pQuadTree = new QuadTreeNode[1365];
		uint32_t i = 0;
		fetchQuadTree(flstream, 0, i);
	}
	bValid = true;
}

CWorld::~CWorld()
{
	/*
	for (uint32_t i = 0; i < vObjects.size(); i++)
	{
		delete[] vObjects.at(i);
	}
	*/
	delete pQuadTree;
}

const bool CWorld::IsValid() const
{
	return bValid;
}

void CWorld::fetchQuadTree(FileStream &flstream, uint32_t dwLevel, uint32_t &i)
{
	QuadTreeNode *pNode = &pQuadTree[i];
	flstream.read(&(pNode->fMax), 4*3);
	flstream.read(&(pNode->fMin), 4*3);
	flstream.read(&(pNode->fHalf), 4*3);
	flstream.read(&(pNode->fCenter), 4*3);
	i++;

	if (dwLevel < 5)
	{
		for (int32_t j = 0; j < 4; j++)
		{
			pNode->lChildIndex[j] = i;
			fetchQuadTree(flstream, dwLevel+1, i);
		}
	}
	else
	{
		pNode->lChildIndex[0] = pNode->lChildIndex[1] = pNode->lChildIndex[2] = pNode->lChildIndex[3] = 0;
	}
}

const char* CWorld::GetIniName() const
{
	return sIni;
}
const char* CWorld::GetGndName() const
{
	return sGnd;
}
const char* CWorld::GetGatName() const
{
	return sGat;
}
const char* CWorld::GetScrName() const
{
	return sScr;
}
const uint32_t CWorld::GetObjectCount() const
{
	return vObjects.size();
}
const CWorld::Object& CWorld::GetObject(uint32_t dwIndex) const
{
	return vObjects.at(dwIndex);
}
const CWorld::QuadTreeNode* CWorld::GetNode(uint32_t dwIndex) const
{
	if (dwIndex > 1364)
		dwIndex = 1364;
	return &pQuadTree[dwIndex];
}
