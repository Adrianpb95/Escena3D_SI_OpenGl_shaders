
/*
	ADRIAN PONCE BALSEIRO
	ESNE G4.3 DDVJ
	adrianpb95@gmail.com
	10 / 06 / 2018
*/

#ifndef ELEVATION_MESH_HEADER
#define ELEVATION_MESH_HEADER

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
    #include <SFML/OpenGL.hpp>
	#include <memory>
	#include <glm\glm.hpp>
	#include <glm\gtc\matrix_transform.hpp>
	#include <glm\gtc\type_ptr.hpp>
	#include <string>
	#include "Camera.hpp"


////////////////////////////////////////////////////////////
// NameSpaces
////////////////////////////////////////////////////////////
namespace example
{
	class Material;
    class Elevation_Mesh
    {

		////////////////////////////////////////////////////////////
		// Declaracion de variables			
		////////////////////////////////////////////////////////////

    private:

		struct Point2f
		{
			float x, y;
		};

		struct Point3f
		{
			float x, y, z;
		};       

		enum
		{
			COORDINATES_VBO,
			TEXTURE_UVS_VBO,
			NORMALS_VBO,
			INDICES_IBO,
			VBO_COUNT
		};												///<Índices para indexar el array vbo_ids:  

        GLuint  vbo_ids[VBO_COUNT];						///<IDs de los VBOs que se usan
		GLsizei number_of_indices;						///<Numero de indices de la malla de elevacion

		GLuint vao_id;									///<ID del VAO

		float angle;									///<Angulo de giro de la mesh
		glm::vec3 position;								///<Posicion de la mesh
		glm::vec3 scale;								///<Escala de la mesh

		std::shared_ptr<Material> material_height;		///<Material: Mapa de altura
		std::shared_ptr<Material> material_color;		///<Material: Color de la malla


		////////////////////////////////////////////////////////////
		// Declaracion de funciones			
		////////////////////////////////////////////////////////////

    public:

		////////////////////////////////////////////////////////////
		/// \brief	Constructor de Elevation_Mesh:	Recibe los parametros del tinyobj, los ordena y genera los VBOs (Vertices, normales, indices y uvs)
		///	@param	material_height		Material: mapa de altura
		///	@param	material_color		Material: color de la malla
		///	@param	position_aux		Posicion donde spawnear la malla
		///	@param	scale_aux			Escala de la malla
		///	@param	cols				Columnas de la malla
		///	@param	rows				Filas de la malla
		///	@param	width				Ancho de la malla
		///	@param	depth				Alto de la malla
		///	@param	elevation			Altura de la malla
		////////////////////////////////////////////////////////////
		Elevation_Mesh(std::shared_ptr<Material> &material_height, std::shared_ptr<Material> &material_color, glm::vec3 position_aux, glm::vec3 scale_aux, int cols, int rows, float width, float depth, float elevation);
       
		////////////////////////////////////////////////////////////
		/// \brief	Destructor de Elevation_Mesh: Se liberan los VBOs usados y la textura
		////////////////////////////////////////////////////////////
		~Elevation_Mesh();

		////////////////////////////////////////////////////////////
		/// \brief	Dibuja la malla de elevacion:	Pinta el material y actualiza la matriz de transformacion y la normal
		///	@param	camera							Camara de la escena
		////////////////////////////////////////////////////////////
		void render(Camera camera);

		////////////////////////////////////////////////////////////
		/// \brief	Actualiza el movimiento de la malla de elevacion
		////////////////////////////////////////////////////////////
		void update();			
    };

}

#endif




