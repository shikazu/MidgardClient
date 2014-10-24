#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <libconfig.h++>

#include <thread>
#include <iostream>
#include "grf/libgrf.h"

#include "views/LoginView.h"

using namespace libconfig;

int main(int argc, char **argv)
{
    /*grf_handle myGRFHandle = grf_load("data.grf", false);

    std::cout << "GRF File Count: " << grf_filecount(myGRFHandle) << std::endl;

    //grf_node* root_node = grf_get_file_list(myGRFHandle);
    grf_node *all_nodes = grf_get_file_id_list(myGRFHandle);
    for(int i = 0; i < grf_filecount(myGRFHandle); i++)
    {
        std::cout << "File: " << grf_file_get_filename(all_nodes[i]) << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    grf_free(myGRFHandle);*/

    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 4;
    settings.majorVersion = 3;
    settings.minorVersion = 0;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Midgard Client - Made by Shikazu", sf::Style::Default, settings);
    window.setVerticalSyncEnabled(true);

    // the event/logic/whatever loop
    LoginView *loginView = new LoginView(&window);


    return 0;
}
