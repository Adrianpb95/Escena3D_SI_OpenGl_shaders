/*
	ADRIAN PONCE BALSEIRO
	ESNE G4.3 DDVJ
	adrianpb95@gmail.com
	10 / 06 / 2018
*/

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <vector>
#include <GL/glew.h>            // Debe incluirse antes que gl.h
#include "Elevation_Mesh.hpp"
#include <SFML/OpenGL.hpp>
#include "Color_Buffer_Rgba8888.hpp"
#include <algorithm>
#include <glm\gtx\normal.hpp>
#include "Material.hpp"


////////////////////////////////////////////////////////////
// NameSpaces
////////////////////////////////////////////////////////////

using std::vector;
extern "C"
{
#include <targa.h>
}
namespace example
{

	Elevation_Mesh::Elevation_Mesh(std::shared_ptr<Material> &material1, std::shared_ptr<Material> &material2, glm::vec3 position_aux, glm::vec3 scale_aux, int cols, int rows, float width, float depth, float elevation)
		: 
		material_height(material1),
		material_color(material2)
	{
		//Inicializacion de las variables
		angle = 0.f;
		position = position_aux;
		scale = scale_aux;
		
		unsigned number_of_vertices =  cols      *  rows;
				 number_of_indices  = (cols - 1) * (rows - 1) * 2 * 3;

		std::vector< Point3f  > positions  (number_of_vertices);
		std::vector< Point3f  > normals    (number_of_vertices);
		std::vector< Point2f  > texture_uvs(number_of_vertices);
		std::vector< unsigned > indices;

		indices.reserve (number_of_indices);

		// Se generan los datos de las posiciones:

		float x_step = width / (cols - 1);
		float z_step = depth / (rows - 1);
		 
		int   textureWidth  = int(material_height->get_texture()->get_width  ());
		int   textureHeight = int(material_height->get_texture()->get_height ());

		float textureWidth_step  = (float)textureWidth  / (cols - 1);
		float textureHeigth_step = (float)textureHeight / (rows - 1);

		float z = -depth / 2.f;
		float texture_z = 0;

		float v = 1.f;

		float texture_coordinate[] = { 0.f, 1.f };

		//Se recorren las filas y columnas y se dan valores en x, y, z ; y ademas se crean las coordenas de textura 
		for (int row = 0, i = 0; row < rows; ++row)		// row = z's
		{
			float x = (-width / 2.f);
			float texture_x = 0;

			float v = texture_coordinate[row & 1];

			for (int col = 0; col < cols; ++col, ++i)		// col = x's
			{
				float u = texture_coordinate[col & 1];

				//El valor de la Y va en funcion del valor de color de la textura(escala de grises)				
				uint8_t level = material_height->get_texture()->get_pixel(std::min((int)floor(texture_x), textureWidth - 1), std::min((int)floor(texture_z), textureHeight - 1)).data.component.r;
				float   y     = level / 255.f * elevation;
				positions[i] = Point3f{ x, y, z };
				texture_uvs[i] = Point2f{ u, v };

				x += x_step;
				texture_x += textureWidth_step;
			}
			z += z_step;	
			texture_z += textureHeigth_step;			
		}				

		//Se generan los valores de las normales de la malla
		int vertex_index = 0;
		for (int row = 0; row < rows; ++row)
		{
			for (int col = 0; col < cols; ++col)
			{
				glm::vec3 a, b, c, d, e, f, g;				

				d = { positions[vertex_index].x, positions[vertex_index].y, positions[vertex_index].z };

				if (row > 0)
				{
					a = { positions[vertex_index - cols].x, positions[vertex_index - cols].y, positions[vertex_index - cols].z };
					
					if (col < cols - 1)
						b = { positions[vertex_index - cols + 1].x, positions[vertex_index - cols + 1].y, positions[vertex_index - cols + 1].z };
					else 
						b = d;
				}
				else
					a = b = d;

				if (col > 0)
				{
					c = { positions[vertex_index - 1].x, positions[vertex_index - 1].y, positions[vertex_index - 1].z };

					if (row < rows - 1)
						f = { positions[vertex_index + cols - 1].x, positions[vertex_index + cols - 1].y, positions[vertex_index + cols - 1].z };
					else
						f = d;
				}
				else
					c = f = d;

				if (col < cols-1)
				{
					e = { positions[vertex_index + 1].x, positions[vertex_index + 1].y, positions[vertex_index + 1].z };

					if (row < rows - 1)
						g = { positions[vertex_index + cols].x, positions[vertex_index + cols].y, positions[vertex_index + cols].z };
					else
						g = d;
				}
				else
					e = d;

				if (row < rows - 1)
				{
					g = { positions[vertex_index + cols].x, positions[vertex_index + cols].y, positions[vertex_index + cols].z };

					if (col < cols - 1)
						e = { positions[vertex_index + 1].x, positions[vertex_index + 1].y, positions[vertex_index + 1].z };
					else
						e = d;
				}
				else
					g = d;

				vector < glm::vec3 > triangleNormals(6);
				triangleNormals[0] = glm::triangleNormal(a, c, d);
				triangleNormals[1] = glm::triangleNormal(b, a, d);
				triangleNormals[2] = glm::triangleNormal(e, b, d);
				triangleNormals[3] = glm::triangleNormal(g, e, d);
				triangleNormals[4] = glm::triangleNormal(f, g, d);
				triangleNormals[5] = glm::triangleNormal(c, f, d);

				//Si el resultado es nan, pongo 0,0,0 para poder seguir con los calculos
				for (size_t i = 0; i < triangleNormals.size(); i++)
				{
					if (isnan(triangleNormals[i].x))
					{
						triangleNormals[i].x = 0.f;
						triangleNormals[i].y = 0.f;
						triangleNormals[i].z = 0.f;
					}
				}

				normals[vertex_index++] = {
					(triangleNormals[0].x + triangleNormals[1].x + triangleNormals[2].x + triangleNormals[3].x + triangleNormals[4].x + triangleNormals[5].x) / 6.f,
					(triangleNormals[0].y + triangleNormals[1].y + triangleNormals[2].y + triangleNormals[3].y + triangleNormals[4].y + triangleNormals[5].y) / 6.f,
					(triangleNormals[0].z + triangleNormals[1].z + triangleNormals[2].z + triangleNormals[3].z + triangleNormals[4].z + triangleNormals[5].z) / 6.f
				};
			}
		}


		// Se generan los datos de los índices:
		for (int row = 0; row < rows - 1; ++row)				// r = z's
		{
			int i = row * cols;

			for (int col = 0; col < cols - 1; ++col, ++i)		// c = x's
			{
				// Se añade el primer triángulo:
				indices.push_back (i + 1);
				indices.push_back (i);
				indices.push_back (i + cols);

				// Se añade el segundo triángulo:
				indices.push_back (i + cols);
				indices.push_back (i + cols + 1);
				indices.push_back (i + 1);				
			}
		}

        // Se generan índices para los VBOs del cubo:
        glGenBuffers (VBO_COUNT, vbo_ids);

		//Se generan los indices de los VAOs
		glGenVertexArrays(1, &vao_id);
		glBindVertexArray(vao_id);

        // Se suben a un VBO los datos de coordenadas:
        glBindBuffer (GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
        glBufferData (GL_ARRAY_BUFFER, positions.size () * sizeof(Point3f), positions.data (), GL_STATIC_DRAW);
		//VertexAtributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Se suben a un VBO los datos de textura y se vinculan al VAO:
		glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[TEXTURE_UVS_VBO]);
		glBufferData(GL_ARRAY_BUFFER, texture_uvs.size() * sizeof(Point2f), texture_uvs.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		//Se suben a un VBO los datos de normales:
		glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[NORMALS_VBO]);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Point3f), normals.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

        // Se suben a un VBO los datos de índices:
		glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_IBO]);
        glBufferData (GL_ELEMENT_ARRAY_BUFFER, indices.size () * sizeof(GLuint), indices.data (), GL_STATIC_DRAW);
		
		glBindVertexArray(0);		
    }

	Elevation_Mesh::~Elevation_Mesh()
    {
        // Se liberan los VBOs usados:
		glDeleteVertexArrays(1, &vao_id);
        glDeleteBuffers (VBO_COUNT, vbo_ids);
    }

	void Elevation_Mesh::update()
	{
		angle += 0.5;
	}

    void Elevation_Mesh::render (Camera camera)
    {
		//Se activa el shader del material
		material_color->use();

		// Matriz de transformacion de la camara
		glm::mat4 model_view_matrix = camera.get_model_view();

		// Se rota la mesh y se mueve a la posicion introducida por parametros:
		model_view_matrix = glm::translate(model_view_matrix, position);
		model_view_matrix = glm::rotate(model_view_matrix, angle, glm::vec3(0.f, 1.f, 0.f));
		model_view_matrix = glm::scale(model_view_matrix, scale);
		glUniformMatrix4fv(material_color->get_model_view_id(), 1, GL_FALSE, glm::value_ptr(model_view_matrix));

		// Matriz de normales
		glm::mat4 normal_matrix = glm::transpose(glm::inverse(model_view_matrix));
		glUniformMatrix4fv(material_color->get_normal_matrix_id(), 1, GL_FALSE, glm::value_ptr(normal_matrix));

		glBindVertexArray(vao_id);
		glDrawElements(GL_TRIANGLES, number_of_indices, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
    }
}
