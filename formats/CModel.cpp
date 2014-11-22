#include "CModel.h"
#include "../common/Globals.h"

CModel::CModel(sf::String sFile)
{
	FileStream flstream;
	GetPipe().getFileStream(sFile, flstream);
	construct(flstream);
}

CModel::CModel(FileStream &flstream)
{
	construct(flstream);
}

void CModel::construct(FileStream &flstream)
{
	uint32_t wSig;
	flstream.read(&wSig, 4);
	if (wSig != 0x4D535247)//GRSM
	{
		bValid = false;
		return;
	}

	wVersion = (flstream.readByte() << 8 ) | flstream.readByte();

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
			Node node;
			fetchNode(flstream, node);
			vNodes.push_back(std::move(node));
		}
	}
	bValid = true;
}

CModel::~CModel()
{
	for (uint32_t i = 0; i < vTextures.size(); i++)
	{
		delete[] vTextures.at(i);
	}
	/*
	for (uint32_t i = 0; i < vNodes.size(); i++)
	{
		delete[] vNodes.at(i);
	}
	*/
}

const bool CModel::IsValid() const
{
	return bValid;
}

void CModel::fetchNode(FileStream &flstream, CModel::Node &node)
{
	flstream.read(node.sName, 40);
	flstream.read(node.sParent, 40);
	int32_t lTexCount;
	flstream.read(&lTexCount, 4);
	if (lTexCount > 0)
	{
		node.vTexIDs.reserve(lTexCount);
		for (int32_t i = 0; i < lTexCount; i++)
		{
			flstream.read(&(node.vTexIDs.at(i)), sizeof(int32_t));
		}
	}
	flstream.read(&(node.fOffsetMT), 9*sizeof(float));
	flstream.read(&(node.pos_), sizeof(Vertex));
	flstream.read(&(node.pos) , sizeof(Vertex));
	flstream.read(&(node.fAngle), sizeof(float));
	flstream.read(&(node.axis), sizeof(Vertex));
	flstream.read(&(node.scale), sizeof(Vertex));

	//Node Vertices
	int32_t lVertCount;
	flstream.read(&lVertCount, sizeof(int32_t));
	if (lVertCount > 0)
	{
		node.vVertices.reserve(lVertCount);
		for (int32_t i = 0; i < lVertCount; i++)
		{
			Vertex vertex;
			flstream.read(&vertex, sizeof(Vertex));
			node.vVertices.push_back(std::move(vertex));
		}
	}

	//Texture Vertices
	int32_t lTexVCount;
	flstream.read(&lTexVCount, sizeof(int32_t));
	if (lTexVCount > 0)
	{
		node.vTextures.reserve(lTexVCount);
		for (int32_t i = 0; i < lTexVCount; i++)
		{
			Texture texture;
			if (wVersion > 0x0102)
			{
				texture.color.r = flstream.readByte();
				texture.color.g = flstream.readByte();
				texture.color.b = flstream.readByte();
				texture.color.a = flstream.readByte();
			}
			else
			{
				texture.color = sf::Color::White;
			}
			flstream.read(&(texture.u), sizeof(float));
			flstream.read(&(texture.v), sizeof(float));
			node.vTextures.push_back(std::move(texture));
		}
	}

	//Faces
	int32_t lFaceCount;
	flstream.read(&lFaceCount, 4);
	if (lFaceCount > 0)
	{
		node.vFaces.reserve(lFaceCount);
		for (int32_t i = 0; i < lFaceCount; i++)
		{
			Face face;
			if (wVersion >= 0x0102)
			{
				flstream.read(&face, sizeof(Face));
			}
			else
			{
				flstream.read(&face, sizeof(Face)-sizeof(int32_t));//SmoothGroup is not included
				face.lSmoothGroup = 0;
			}
			node.vFaces.push_back(std::move(face));
		}
	}

	//Position KeyFrames
	if (wVersion >= 0x0105)
	{
		int32_t lPosKeyCount = 0;
		flstream.read(&lPosKeyCount, 4);
		if (lPosKeyCount > 0)
		{
			node.vPosFrames.reserve(lPosKeyCount);
			for (int32_t i = 0; i < lPosKeyCount; i++)
			{
				KeyFrame posframe;
				flstream.read(&posframe, sizeof(KeyFrame) - 4);//w is not there
				posframe.w = 0;
				node.vPosFrames.push_back(std::move(posframe));
			}
		}
	}

	//Rotation KeyFrames
	int32_t lRotKeyCount;
	flstream.read(&lRotKeyCount, 4);
	if (lRotKeyCount > 0)
	{
		node.vRotFrames.reserve(lRotKeyCount);
		for (int32_t i = 0; i < lRotKeyCount; i++)
		{
			KeyFrame rotframe;
			flstream.read(&rotframe, sizeof(KeyFrame));
			node.vRotFrames.push_back(std::move(rotframe));
		}
	}
}

const uint32_t CModel::GetNodeCount() const
{
	return vNodes.size();
}

const CModel::Node& CModel::GetNode(uint32_t dwIndex) const
{
	return vNodes.at(dwIndex);
}

const uint32_t CModel::GetTexNameCount() const
{
	return vTextures.size();
}
const char* CModel::GetTexName(uint32_t dwIndex) const
{
	return vTextures.at(dwIndex);
}
