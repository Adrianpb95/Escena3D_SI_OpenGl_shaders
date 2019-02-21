/*	
	ADRIAN PONCE BALSEIRO
	ESNE G4.3 DDVJ
	adrianpb95@gmail.com
	10 / 06 / 2018
*/
#include "View.hpp"
#include <GL/glew.h>            // Debe incluirse antes que gl.h
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

using namespace sf;
using namespace example;

int main ()
{
    Window window(VideoMode(800, 600), "OpenGL Examples: Skybox", Style::Default, ContextSettings(32));

    // Una vez se ha creado el contexto de OpenGL ya se puede inicializar Glew:

    GLenum glew_initialization =  glewInit ();

    assert(glew_initialization == GLEW_OK);

    // Glew se inicializa antes de crear view porque view ya usa extensiones de OpenGL:

    View view (800, 600);

    window.setVerticalSyncEnabled (true);

    bool running = true;

    do
    {
        Event event;

        while (window.pollEvent (event))
        {
            switch (event.type)
            {
                case Event::Closed:
                {
                    running = false;
                    break;
                }

                case Event::Resized:
                {
                    Vector2u window_size = window.getSize ();

                    view.resize (window_size.x, window_size.y);

                    break;
                }

                case Event::KeyPressed:
                {
                    view.on_key (event.key.code, true);
                    break;
                }

				case Event::KeyReleased:
				{
					view.on_key(event.key.code, false);
					break;
				}             
            }
        }

        view.update ();
        view.render ();

        window.display ();
    }
    while (running);

    return (EXIT_SUCCESS);
}
