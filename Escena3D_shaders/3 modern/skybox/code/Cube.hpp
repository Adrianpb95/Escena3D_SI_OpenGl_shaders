
/*
	ADRIAN PONCE BALSEIRO
	ESNE G4.3 DDVJ
	adrianpb95@gmail.com
	10 / 06 / 2018
*/


/*
	EDITED BY:
	ADRIAN PONCE BALSEIRO
	ESNE G4.3 DDVJ
	adrianpb95@gmail.com
	10 / 06 / 2018
*/

#ifndef CUBE_HEADER
#define CUBE_HEADER


////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <memory>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <string>
#include <list>
#include "Camera.hpp"


////////////////////////////////////////////////////////////
// NameSpaces
////////////////////////////////////////////////////////////
using namespace std;
namespace example
{	
	class Material;
	class Cube
	{
		////////////////////////////////////////////////////////////
		// Declaracion de variables			
		////////////////////////////////////////////////////////////

	private:

		enum
		{
			COORDINATES_VBO,
			TEXTURE_UVS_VBO,
			NORMALS_VBO,
			INDICES_IBO,
			VBO_COUNT
		};												///<Índices para indexar el array vbo_ids
		
		static const GLfloat coordinates[];				///<Array de vertices del cubo
		static const GLfloat normals[];					///<Array de normales del cubo
		static const GLfloat texture_uvs[];				///<Array de coordenadas de textura del cubo
		static const GLubyte indices[];					///<Array de indices del cubo

		GLuint vbo_ids[VBO_COUNT];						///<IDs de los VBOs que se usan
		GLuint vao_id;									///<ID del VAO

		float angle;									///<Angulo de giro del cubo
		glm::vec3 position;								///<Posicion del cubo
		glm::vec3 scale;								///<Escala del cubo
		list<shared_ptr<Cube>>	children;				///<Lista de hijos para el grafo de escena
		std::shared_ptr<Material> material;				///<Material del cubo


		////////////////////////////////////////////////////////////
		// Declaracion de funciones			
		////////////////////////////////////////////////////////////

	public:

		////////////////////////////////////////////////////////////
		/// \brief	Constructor de Cube:	Genera los VBOs (Vertices, normales, indices y texturas)
		/// @param	material			Material del cubo
		///	@param	position_aux		Posicion donde spawnear el cubo
		///	@param	scale_aux			Escala del cubo
		////////////////////////////////////////////////////////////
		Cube(std::shared_ptr<Material> &material, glm::vec3 position_aux, glm::vec3 scale_aux);
		
		////////////////////////////////////////////////////////////
		/// \brief	Destructor de Cube: Se liberan los VBOs usados y la textura
		////////////////////////////////////////////////////////////
		~Cube();

	public:

		////////////////////////////////////////////////////////////
		/// \brief	Dibuja el cubo y sus hijos:	Pinta el material y actualiza la matriz de transformacion y la normal
		///	@param	model_view_matrix_parent		Matriz de transformacion del padre
		////////////////////////////////////////////////////////////
		void render(glm::mat4 model_view_matrix_parent);
		
		////////////////////////////////////////////////////////////
		/// \brief	Actualiza el movimiento del cubo y de sus hijos
		////////////////////////////////////////////////////////////
		void update();

		////////////////////////////////////////////////////////////
		/// \brief	Añade un cubo como hijo de otro
		///	@param	m			Cubo hijo
		////////////////////////////////////////////////////////////
		void add(shared_ptr <Cube> & m);
	};
}

#endif
