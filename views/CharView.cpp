#include "CharView.h"
#include "../UI/Frame.h"
#include "../UI/Button.h"
#include "../UI/TextBox.h"
#include "../UI/PlayerFrame.h"
#include <iostream>
#include <cstring>

namespace CharView
{
	UI::Frame* pFrame;
	bool bSwitchToMap;
	bool bDeleteChar;
	bool bCancelled;
	std::array<UI::PlayerFrame*, 3*2> players;
	uint32_t dwFrame = 0, dwSelected = 0;
	const uint32_t dwMaxFrame = 2;
//	sf::Texture selectTexture;

	void Init()
	{
	}

	void Create(UI::Manager &mgr)
	{
		//Dummy values for test
		CHARACTER_INFO_NEO_UNION cinuDummy;
		cinuDummy.lRobe = 0;
		cinuDummy.shAccessory  = 1;
		cinuDummy.shAccessory2 = 2;
		cinuDummy.shAccessory3 = 3;
		cinuDummy.shHead = 1;
		cinuDummy.shJob = 10;
		strcpy(cinuDummy.sName, "TestName");
		cinuDummy.uLuk = 3;
		cinuDummy.uDex = 4;
		cinuDummy.uInt = 5;
		cinuDummy.uStr = 5;
		cinuDummy.uAgi = 4;
		cinuDummy.uVit = 3;
		cinuDummy.shLevel = 3;
		cinuDummy.lExp = 23913;
		cinuDummy.lHP = 3000;
		cinuDummy.shSP = 230;

		pFrame = new UI::Frame(CV_FRAME, 0, 0);
		pFrame->SetTexture("login_interface\\win_select2.bmp");
		pFrame->SetAlign(UI::CENTER, UI::MIDDLE);
		pFrame->SetDragArea(0, 0, pFrame->GetWidth(), pFrame->GetHeight());
		mgr.AddChild(pFrame);

		//OK Button
		UI::Button* pOK = new UI::Button(CV_OKBTN, -49, -4);
		pOK->SetAlign(UI::RIGHT, UI::BOTTOM);
		pOK->SetTexture("btn_ok.bmp", pOK->INACTIVE);
		pOK->SetTexture("btn_ok_a.bmp", pOK->ACTIVE);
		pOK->SetTexture("btn_ok_b.bmp", pOK->PRESSED);
		pOK->SetCallback(CharView::HandleOK);
		pFrame->AddChild(pOK);

		//Cancel Button
		UI::Button* pCancel = new UI::Button(CV_CANCBTN, -4, -4);
		pCancel->SetAlign(UI::RIGHT, UI::BOTTOM);
		pCancel->SetTexture("btn_cancel.bmp", pCancel->INACTIVE);
		pCancel->SetTexture("btn_cancel_a.bmp", pCancel->ACTIVE);
		pCancel->SetTexture("btn_cancel_b.bmp", pCancel->PRESSED);
		pCancel->SetCallback(CharView::HandleCancel);
		pFrame->AddChild(pCancel);

		//Delete Button
		UI::Button* pDelete = new UI::Button(CV_DELBTN, 4, -4);
		pDelete->SetAlign(UI::LEFT, UI::BOTTOM);
		pDelete->SetTexture("btn_del.bmp", pDelete->INACTIVE);
		pDelete->SetTexture("btn_del_a.bmp", pDelete->ACTIVE);
		pDelete->SetTexture("btn_del_b.bmp", pDelete->PRESSED);
		pDelete->SetCallback(CharView::HandleDelete);
		pFrame->AddChild(pDelete);

		//Create Button - Invisible unless on a blank slot
		UI::Button* pMake = new UI::Button(CV_MAKEBTN, -49, -4);
		pMake->SetAlign(UI::RIGHT, UI::BOTTOM);
		pMake->SetTexture("btn_make.bmp",   pMake->INACTIVE);
		pMake->SetTexture("btn_make_a.bmp", pMake->ACTIVE);
		pMake->SetTexture("btn_make_b.bmp", pMake->PRESSED);
		pMake->SetCallback(CharView::HandleCreate);
		pMake->SetVisible(false);
		pFrame->AddChild(pMake);

		//Scroll Buttons - Left & Right
		UI::Button* pScroll = new UI::Button(CV_LEFTBTN, 45, 110);
		pScroll->SetTexture("scroll1left.bmp",   pScroll->INACTIVE);
		pScroll->SetCallback(CharView::HandleScroll);
		pFrame->AddChild(pScroll);

		pScroll = new UI::Button(CV_RIGTBTN, -45, 110);
		pScroll->SetHAlign(UI::RIGHT);
		pScroll->SetTexture("scroll1right.bmp",   pScroll->INACTIVE);
		pScroll->SetCallback(CharView::HandleScroll);
		pFrame->AddChild(pScroll);

		//Labels - Character Stats & Other Info
		uint32_t dwX = 60;
		uint32_t dwY = -140;
		uint32_t dwID = CV_NAMELBL;
		UI::TextBox* pText;
		for (uint32_t i = 0; i < 7; i++, dwY += 16, dwID++)
		{
			pText = new UI::TextBox(dwID, UI::REGULAR, dwX-3, dwY, 100, 12);
			pText->SetVAlign(UI::BOTTOM);
			pText->SetFontID(0);
			pText->SetColor(sf::Color::Black, UI::FOREGROUND);
			pText->SetBorderWidth(1);
			pText->SetCharSize(10);
      pText->SetFocusable(false);
      pText->SetEditable(false);
      switch (i)
      {
      	case 0 : {pText->SetText(cinuDummy.sName); break;}
      	case 1 : {
      		char sName[40];
					GetDB().GetJobName(cinuDummy.shJob, 1, sName);
					pText->SetText(sName);
					break;
				}
				case 6 : {
					pText->SetText("Map Name here");
					break;
				}
				default: {
					int32_t lValue;
					switch (i)
					{
						case 2 : {lValue = cinuDummy.shLevel; break;}
						case 3 : {lValue = cinuDummy.lExp;    break;}
						case 4 : {lValue = cinuDummy.lHP;     break;}
						default: {lValue = cinuDummy.shSP;    break;}//case 5
					}
					char sText[10];
					sprintf(sText, "%d", lValue);
					pText->SetText(sText);
					break;
				}
      }
			pFrame->AddChild(pText);
		}
		pText->SetWidth(240);//for the MAP

		dwY  = -140;
		dwX += 143;
		dwID = CV_STRLBL;
		for (uint32_t i = 0; i < 6; i++, dwY += 16, dwID++)
		{
			pText = new UI::TextBox(dwID, UI::REGULAR, dwX-3, dwY, 100, 12);
			pText->SetVAlign(UI::BOTTOM);
			pText->SetFontID(0);
			pText->SetColor(sf::Color::Black, UI::FOREGROUND);
			pText->SetBorderWidth(1);
			pText->SetCharSize(10);
      pText->SetFocusable(false);
      pText->SetEditable(false);
      uint8_t uStat;
      switch (i)
      {
      	case 0 : {uStat = cinuDummy.uStr; break;}
      	case 1 : {uStat = cinuDummy.uAgi; break;}
      	case 2 : {uStat = cinuDummy.uVit; break;}
      	case 3 : {uStat = cinuDummy.uInt; break;}
      	case 4 : {uStat = cinuDummy.uDex; break;}
      	default: {uStat = cinuDummy.uLuk; break;}
      }
      char stat[4];
      sprintf(stat, "%d", uStat);
      pText->SetText(stat);
			pFrame->AddChild(pText);
		}

		//GetPipe().getTexture("box_select.bmp", &selectTexture);

		for (uint32_t i = 0; i < dwMaxFrame; i++)//Total Number of sets of 3
		{
			sf::Vector2i vPos(56, 41);
			for (uint32_t j = 0; j < 3; j++)
			{
				UI::PlayerFrame* pSprFrame = new UI::PlayerFrame(CV_SPRFRAME+i*3+j, vPos, new Player(&cinuDummy));//player is for dummy test
				pSprFrame->SetTexture("login_interface\\box_select.bmp");
				if (i != dwFrame)
				{
					pSprFrame->SetVisible(false);
				}
				pFrame->AddChild(pSprFrame);
				players[i*3 + j] = pSprFrame;
				vPos.x += 163;
			}
		}
		players[dwSelected]->Select();

		//Status Flags
		bSwitchToMap = false;
		bDeleteChar = false;
		bCancelled = false;
	}

	void HandleOK(UI::Widget* pButton, UI::Manager* pManager)
	{
		pManager->DelChild(pFrame, pManager);
		bSwitchToMap = true;
		GetMouseCursor().SetState(CRS_DEFAULT);
	}

	void HandleCancel(UI::Widget* pButton, UI::Manager* pManager)
	{
		pManager->DelChild(pFrame, pManager);
		bCancelled = true;
	}

	void HandleDelete(UI::Widget* pButton, UI::Manager* pManager)
	{
		pManager->DelChild(pFrame, pManager);
		bDeleteChar = true;
	}

	void HandleCreate(UI::Widget* pButton, UI::Manager* pManager)
	{

	}

	void HandleScroll(UI::Widget* pButton, UI::Manager* pManager)
	{
		dwSelected = UI::PlayerFrame::GetSelected()->GetID() - CV_SPRFRAME;
		if (pButton->GetID() == CV_RIGTBTN)
		{
			if (dwSelected < dwMaxFrame*3 - 1)
			{
				dwSelected++;
			}
		}
		else if (pButton->GetID() == CV_LEFTBTN)
		{
			if (dwSelected > 0)
			{
				dwSelected--;
			}
		}
		dwFrame = dwSelected/3;
		for (uint32_t i = 0; i < dwMaxFrame; i++)
		{
			for (uint32_t j = 0; j < 3; j++)
			{
				if (i == dwFrame)
				{
					players[i*3 + j]->SetVisible(true);
				}
				else
				{
					players[i*3 + j]->SetVisible(false);
				}
			}
		}
		UI::PlayerFrame* pLFrame = (UI::PlayerFrame*)(pFrame->GetChild(CV_SPRFRAME + dwSelected));
		pLFrame->Select();
	}

	bool IsOkPressed()
	{
		return bSwitchToMap;
	}

	bool IsCancelled()
	{
		return bCancelled;
	}

	bool IsDelPressed()
	{
		return bDeleteChar;
	}
}
