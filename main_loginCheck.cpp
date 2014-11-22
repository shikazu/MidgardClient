#include "UI/Manager.h"
#include "views/LoginView.h"
#include "views/CharView.h"
#include "UI/PlayerFrame.h"
#include "render/Player.h"

#include <iostream>

void MainMachine(sf::RenderWindow& window, UI::Manager& manager, sf::View& view);
void EventLoop(sf::RenderWindow& window, UI::Manager& manager, sf::View& view);
void RenderBackground(sf::RenderWindow& window, sf::View& view);

int main(int argc, char** argv)
{
	//Settings
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 0;

	//MainWindow
	sf::RenderWindow window(sf::VideoMode(800, 600), "Midgard Client Test", sf::Style::Default, settings);
	window.setVerticalSyncEnabled(true);
	window.setFramerateLimit(30);
	sf::View view({0, 0, 800, 600});
	window.setView(view);
	window.setMouseCursorVisible(false);

	//Manager
	UI::Manager manager(MAIN_MANAGER, window);

	//LoginView
	LoginView::Init();

	//Main Loop
	while(window.isOpen())
	{
		//State Machine - does all the creation and deletion activities
		MainMachine(window, manager, view);

		//Event Loop
		EventLoop(window, manager, view);

		//Display Widgets
		window.clear();
		RenderBackground(window, view);
		window.draw(manager);
		window.draw(GetMouseCursor());
		window.display();
	}
	return 0;
}

void EventLoop(sf::RenderWindow& window, UI::Manager& manager, sf::View& view)
{
	sf::Event event;
	while(window.pollEvent(event))
	{
		if (event.type == event.Closed)
		{
			window.close();
			exit(0);
		}
		if (event.type == event.Resized)
		{
			window.clear();
			view.reset(sf::FloatRect(0, 0, event.size.width, event.size.height));
			window.setView(view);
		}
		manager.ParseEvent(event);
		GetMouseCursor().CheckMouse(event);
	}
}

void RenderBackground(sf::RenderWindow& window, sf::View& view)
{
	static sf::Texture texture;
	static sf::View bgview(sf::FloatRect(0, 0, 1024, 768));
	if (texture.getSize().x == 0 && texture.getSize().y == 0)
	{
		FileStream stream;
		sf::RenderTexture rt;
		rt.create(1024, 768);
		rt.clear();
		for (uint8_t i = 0; i < 4; i++)
		{
			for (uint8_t j = 0; j < 3; j++)
			{
				char sFile[16];
				sprintf(sFile, "t_배경%d-%d.bmp", j+1, i+1);
				//std::ostringstream oss;
				//oss << "t_배경" << j+1 << "-" << i+1 << ".bmp";
				sf::Texture tsplice;
				//GetPipe().getTexture(oss.str(), &tsplice);
				GetPipe().getTexture(sFile, &tsplice);
				sf::Sprite sprite(tsplice);
				sprite.setPosition(i*256, j*256);
				rt.draw(sprite);
			}
		}
		rt.display();
		texture = rt.getTexture();
		texture.setSmooth(true);
	}
	window.setView(bgview);
	sf::Sprite sprite(texture);
	window.draw(sprite);
	window.setView(view);
}

void MainMachine(sf::RenderWindow& window, UI::Manager& manager, sf::View& view)
{
	static uint32_t uState = 0;
	switch(uState)
	{
		case 0:
		{
			//Create LoginView
			LoginView::Create(manager);
			uState++;
			break;
		}
		case 1:
		{
			if (LoginView::IsExitPressed())
			{
				std::cout << "Now Exiting the Game\n";
				window.close();
				exit(0);
			}
			if (LoginView::IsLoggedIn())
			{
				std::cout << "Credentials: " << LoginView::GetUserName().toAnsiString() << " " << LoginView::GetPassWord().toAnsiString() << std::endl;
				uState++;
			}
			break;
		}
		case 2:
		{
			CharView::Create(manager);
			uState++;
			break;
		}
		case 3:
		{
			if (CharView::IsCancelled())
			{
				uState = 0;
				break;
			}
			if (CharView::IsDelPressed())
			{
				uState+=5;
				break;
			}
			if (CharView::IsOkPressed())
			{
				uState++;
				break;
			}
		}
		case 4:
		{

		}
	}
}
