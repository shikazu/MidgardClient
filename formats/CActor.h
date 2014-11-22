#ifndef _CACTOR_H
#define _CACTOR_H
#include <vector>
#include "../common/FileStream.h"
#include <SFML/Graphics/Color.hpp>

class CActor ///Handler for ACT files
{
	public:
		struct Layer
		{
			int32_t x;
			int32_t y;
			int32_t lSprNum;
			sf::Color color;
			float fScaleX;//is already modified according to mirror value
			float fScaleY;
			uint32_t dwAngle;
			uint32_t dwSprType;
			uint32_t dwWidth;
			uint32_t dwHeight;
		};
		struct Pos
		{
			uint32_t dwReserved;
			int32_t x;
			int32_t y;
			uint32_t dwAttrib;
		};
		struct Frame
		{
			std::vector<Layer> vLayers;
			std::vector<Pos> vPositions;
			int32_t lSoundIndex;
		};
		struct Action
		{
			std::vector<Frame> vFrames;
			float fDelay;
		};

		CActor(sf::String sFile);
		CActor(FileStream &flstream);
		virtual ~CActor();

		const Pos&	GetPos(   uint16_t wAct, uint32_t dwFrame, uint32_t dwPos) const;
		const Layer&  GetLayer( uint16_t wAct, uint32_t dwFrame, uint32_t dwLayer) const;
		const Frame&  GetFrame( uint16_t wAct, uint32_t dwFrame) const;
		const Action& GetAction(uint16_t wAct) const;
		const char* GetSound(uint32_t dwSound) const;

		const uint16_t GetActionCount() const;
		const uint32_t GetFrameCount(uint16_t wAct) const;
		const uint32_t GetLayerCount(uint16_t wAct, uint32_t dwFrame) const;
		const uint32_t GetSoundCount() const;
		const bool IsValid() const;

	private:
		bool bValid;
		uint16_t wVersion;
		std::vector<Action> vActions;
		std::vector<char*> vSounds;

		void construct(FileStream &flstream);
		void fetchAction(FileStream &flstream, Action &action);
		void fetchFrame( FileStream &flstream, Frame  &frame);
		void fetchLayer( FileStream &flstream, Layer  &layer);
};

#endif//_CACTOR_H
