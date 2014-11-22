#ifndef _MESSAGEBOX_H
#define _MESSAGEBOX_H
#include "Frame.h"
#include "Button.h"
#include "Manager.h"

namespace UI
{
	//Enumerations
	enum MsgButton
	{
		MSG_NONE	 = 0x00,//Only used in GetStatus dont use for making buttons
		MSG_OK	   = 0x01,
		MSG_CANCEL   = 0x02,
//		MSG_YES	  = 0x04,//Not Implemented - will do if necessary
//		MSG_NO	   = 0x08
	};
	class MessageBox : public Frame
	{
		public:
			//Constructors and Destructor -  callback is triggered when one of the buttons have been clicked
			MessageBox(Manager *pManager, uint32_t dwFontID, uint8_t uStyle, uint32_t dwCharSize, CALLBACK callback, sf::String sText = "", uint8_t uButtons = MSG_OK, sf::Vector2i vPos = sf::Vector2i(0,0));
			MessageBox(Manager *pManager, uint32_t dwFontID, uint8_t uStyle, uint32_t dwCharSize, CALLBACK callback, sf::String sText = "", uint8_t uButtons = MSG_OK, int32_t x = 0, int32_t y = 0);
			~MessageBox();

			//Methods
			static void SelectButton(Widget* pButton, Manager* pManager);//CALLBACK format
			const MsgButton GetSelected() const;

		protected:
			virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
			virtual bool ParseEvent(sf::Event event, Manager* pManager);//Checks the event and spreads it to children.

		private:
			void construct(Manager *pManager, uint8_t uStyle, sf::String sText, uint8_t uButtons);
			Widget* pFocusPrevious;
			MsgButton selected;
			CALLBACK pCallback;
			uint32_t dwFontID, dwCharSize;
	};
}
#endif//_MESSAGEBOX_H
