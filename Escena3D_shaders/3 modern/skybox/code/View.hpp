
/*
	ADRIAN PONCE BALSEIRO
	ESNE G4.3 DDVJ
	adrianpb95@gmail.com
	10 / 06 / 2018
*/

#ifndef VIEW_HEADER
#define VIEW_HEADER


////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Camera.hpp"
#include "Skybox.hpp"
#include "Cube.hpp"
#include <string>
#include <SFML/Window.hpp>
#include <iostream>
#include "Elevation_Mesh.hpp"
#include "Mesh.hpp"
#include "Material.hpp"
#include <map>

////////////////////////////////////////////////////////////
// NameSpaces
////////////////////////////////////////////////////////////
using namespace std;
namespace example
{

    class View
    {

	////////////////////////////////////////////////////////////
	// Declaracion de variables			
	////////////////////////////////////////////////////////////

    private:

		std::shared_ptr< Camera > camera;							///<Elemento camara de la escena
        std::unique_ptr< Skybox > skybox;							///<Elemento skybox de la escena
		std::unique_ptr< Cube > cube;								///<Elemento cubo de la escena
		std::unique_ptr< Elevation_Mesh > elevation_mesh;			///<Elemento malla de elevacion de la escena
		std::unique_ptr< Mesh >  mesh;								///<Elemento silla de la escena

		std::map<std::string, shared_ptr< Material>> materials;		///<Contenedor de materiales
			
        int    width;												///<Ancho ventana			
        int    height;												///<Alto ventana
					
		Shader_Program shader_program_postprocess;					///<Shader para el postproceso

		sf::Keyboard::Key last_key = sf::Keyboard::Escape;			///<Auxiliar para gestionar los inputs de teclado

		GLuint framebuffer_id;										///<ID del framebuffer de la textura
		GLuint depthbuffer_id;										///<ID del zbuffer para combinarlo con el framebuffer
		GLuint out_texture_id;										///<ID de la textura del framebuffer

		GLuint triangle_vbo0;										///<VBO del quad del render del framebuffer
		GLuint triangle_vbo1;										///<VBO de las texturas del render del framebuffer

		static const GLsizei framebuffer_width = 512;				///<Ancho del frambebuffer
		static const GLsizei framebuffer_height = 512;				///<Alto del framebuffer	


		////////////////////////////////////////////////////////////
		// Declaracion de funciones			
		////////////////////////////////////////////////////////////

    public:

		////////////////////////////////////////////////////////////
		/// \brief	Constructor de View:	Crea la escena a renderizar y el postproceso de toda la ventana
		///	@param	width		Ancho de la ventana
		///	@param	height		Alto de la ventana
		////////////////////////////////////////////////////////////
        View(int width, int height);

		////////////////////////////////////////////////////////////
		/// \brief	Actualiza el movimiento de la camara y el movimiento de los elementos de la escena
		////////////////////////////////////////////////////////////
        void update   ();

		////////////////////////////////////////////////////////////
		/// \brief	Dibuja la escena:	Gestiona los diferentes shaders llamando al render de cada elemento
		////////////////////////////////////////////////////////////
        void render   ();

		////////////////////////////////////////////////////////////
		/// \brief	Configura el tamaño de la ventana
		///	@param	width		Ancho de la ventana
		///	@param	height		Alto de la ventana
		////////////////////////////////////////////////////////////
        void resize   (int width, int height);

		////////////////////////////////////////////////////////////
		/// \brief	Input de teclado:	Movimiento de la camara
		///	@param	key			Id de la tecla usada
		///	@param	down		Se ha pulsado o soltado la tecla
		////////////////////////////////////////////////////////////
        void on_key   (sf::Keyboard::Key key, bool down);

		////////////////////////////////////////////////////////////
		/// \brief	Creacion del framebuffer para el postproceso
		////////////////////////////////////////////////////////////
		void   build_framebuffer();

		////////////////////////////////////////////////////////////
		/// \brief	Render del framebuffer del postproceso
		////////////////////////////////////////////////////////////
		void   render_framebuffer();
    };

}

#endif
