#ifndef _CMODEL_H
#define _CMODEL_H
#include <vector>
#include "../common/FileStream.h"
#include <SFML/Graphics/Color.hpp>

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
			//uint32_t dwColor;
			sf::Color color;
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

			std::vector<Vertex> vVertices;
			std::vector<Texture> vTextures;
			std::vector<Face>vFaces;
			std::vector<KeyFrame>vPosFrames;
			std::vector<KeyFrame>vRotFrames;
		};

		CModel(sf::String sFile);
		CModel(FileStream &flstream);
		virtual ~CModel();

		const CModel::Node& GetNode(uint32_t dwIndex) const;
		const char* GetTexName(uint32_t dwIndex) const;

		const uint32_t GetNodeCount() const;
		const uint32_t GetTexNameCount() const;
		const bool IsValid() const;

	private:
		bool bValid;
		uint16_t wVersion;
		int32_t lAnimLength;
		int32_t lShadeType;
		uint8_t uAlpha;
		char sNode[40];
		std::vector<char*> vTextures;
		std::vector<Node> vNodes;

		void construct(FileStream &flstream);
		void fetchNode(FileStream &flstream, CModel::Node &node);
};

#endif//_CMODEL_H
