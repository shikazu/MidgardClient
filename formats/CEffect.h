#ifndef _GEFFECT_H
#define _GEFFECT_H
#include <vector>
#include "../common/FileStream.h"

class CEffect///Handler for STR files used for creating Effects
{
	public:
		struct Frame //Animation KeyFrame
		{
			uint32_t dwFrameNum;
			uint32_t dwFrameType;
			float x;
			float y;
			float u;
			float v;
			float us;
			float vs;
			float u2;
			float v2;
			float us2;
			float vs2;
			float ax[4];//Vertice coordinates
			float ay[4];//Vertice coordinates
			float fAniFrame;
			uint32_t dwAniType;
			float fAniDelta;
			float fRotation;
			uint32_t dwRGBA;//should be converted to color
			uint32_t dwSrcBlend;
			uint32_t dwDstBlend;
			uint32_t dwMtPreset;
		};

		struct Layer
		{
			std::vector<char*> vImages;
			std::vector<Frame> vFrames;
		};

		CEffect(sf::String sFile);
		CEffect(FileStream &flstream);
		virtual ~CEffect();

		const Layer& GetLayer(uint32_t dwLayer) const;
		const Frame& GetFrame(uint32_t dwLayer, uint32_t dwFrame) const;
		const char*  GetImageName(uint32_t dwLayer, uint32_t dwImage) const;

		const uint32_t GetLayerCount() const;
		const uint32_t GetFrameCount(uint32_t dwLayer) const;
		const uint32_t GetImageCount(uint32_t dwLayer) const;
		const bool IsValid() const;

	private:
		bool bValid;
		uint32_t dwVersion;
		std::vector<Layer> vLayers;

		void construct(FileStream &flstream);
		void fetchLayer(FileStream &flstream, Layer &layer);
};

#endif//_CEFFECT_H
