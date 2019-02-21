/*
	ADRIAN PONCE BALSEIRO
	ESNE G4.3 DDVJ
	adrianpb95@gmail.com
	10 / 06 / 2018
*/

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "Cube.hpp"
#include "Color_Buffer_Rgba8888.hpp"
#include "Material.hpp"

extern "C"
{
#include <targa.h>
}

namespace example
{

	const GLfloat Cube::coordinates[] =
	{
		-1.f, +1.f, +1.f,     // 0
		+1.f, +1.f, +1.f,     // 1
		+1.f, -1.f, +1.f,     // 2
		-1.f, -1.f, +1.f,     // 3
		-1.f, +1.f, -1.f,     // 4
		-1.f, +1.f, +1.f,     // 5
		-1.f, -1.f, +1.f,     // 6
		-1.f, -1.f, -1.f,     // 7
		+1.f, +1.f, -1.f,     // 8
		-1.f, +1.f, -1.f,     // 9
		-1.f, -1.f, -1.f,     // 10
		+1.f, -1.f, -1.f,     // 11
		+1.f, +1.f, +1.f,     // 12
		+1.f, +1.f, -1.f,     // 13
		+1.f, -1.f, -1.f,     // 14
		+1.f, -1.f, +1.f,     // 15
		-1.f, -1.f, +1.f,     // 16
		+1.f, -1.f, +1.f,     // 17
		+1.f, -1.f, -1.f,     // 18
		-1.f, -1.f, -1.f,     // 19
		-1.f, +1.f, -1.f,     // 20
		+1.f, +1.f, -1.f,     // 21
		+1.f, +1.f, +1.f,     // 22
		-1.f, +1.f, +1.f,     // 23
	};

	const GLfloat Cube::normals[] =
	{
		 0.f,  0.f, +1.f,     // 0
		 0.f,  0.f, +1.f,     // 1
		 0.f,  0.f, +1.f,     // 2
		 0.f,  0.f, +1.f,     // 3
		-1.f,  0.f,  0.f,     // 4
		-1.f,  0.f,  0.f,     // 5
		-1.f,  0.f,  0.f,     // 6
		-1.f,  0.f,  0.f,     // 7
		 0.f,  0.f, -1.f,     // 8
		 0.f,  0.f, -1.f,     // 9
		 0.f,  0.f, -1.f,     // 10
		 0.f,  0.f, -1.f,     // 11
		+1.f,  0.f,  0.f,     // 12
		+1.f,  0.f,  0.f,     // 13
		+1.f,  0.f,  0.f,     // 14
		+1.f,  0.f,  0.f,     // 15
		 0.f, -1.f,  0.f,     // 16
		 0.f, -1.f,  0.f,     // 17
		 0.f, -1.f,  0.f,     // 18
		 0.f, -1.f,  0.f,     // 19
		 0.f, +1.f,  0.f,     // 20
		 0.f, +1.f,  0.f,     // 21
	 	 0.f, +1.f,  0.f,     // 22
		 0.f, +1.f,  0.f,     // 23
	};

	const GLfloat Cube::texture_uvs[] =
	{
		0.f, 0.f,             // 0
		1.f, 0.f,             // 1
		1.f, 1.f,             // 2
		0.f, 1.f,             // 3
		0.f, 0.f,             // 4
		1.f, 0.f,             // 5
		1.f, 1.f,             // 6
		0.f, 1.f,             // 7
		0.f, 0.f,             // 8
		1.f, 0.f,             // 9
		1.f, 1.f,             // 10
		0.f, 1.f,             // 11
		0.f, 0.f,             // 12
		1.f, 0.f,             // 13
		1.f, 1.f,             // 14
		0.f, 1.f,             // 15
		0.f, 0.f,             // 16
		1.f, 0.f,             // 17
		1.f, 1.f,             // 18
		0.f, 1.f,             // 19
		0.f, 0.f,             // 20
		1.f, 0.f,             // 21
		1.f, 1.f,             // 22
		0.f, 1.f,             // 23
	};

	const GLubyte Cube::indices[] =
	{
		1,  0,  3,            // front
		1,  3,  2,
		5,  4,  7,            // left
		5,  7,  6,
		9,  8, 11,            // back
		9, 11, 10,
		13, 12, 15,           // right
		13, 15, 14,
		17, 16, 19,           // top
		17, 19, 18,
		20, 23, 22,           // bottom
		20, 22, 21,
	};

	Cube::Cube(shared_ptr<Material> &material_aux, glm::vec3 position_aux, glm::vec3 scale_aux)
		:
		material(material_aux)
	{
		angle = 0.f;
		position = position_aux;
		scale = scale_aux;

		// Se generan índices para los VBOs del cubo:

		glGenBuffers(VBO_COUNT, vbo_ids);
		glGenVertexArrays(1, &vao_id);

		// Se activa el VAO del cubo para configurarlo:

		glBindVertexArray(vao_id);

		// Se suben a un VBO los datos de coordenadas y se vinculan al VAO:

		glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[COORDINATES_VBO]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(coordinates), coordinates, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Se suben a un VBO los datos de textura y se vinculan al VAO:

		glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[TEXTURE_UVS_VBO]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(texture_uvs), texture_uvs, GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		// Se suben a un VBO los datos de normales y se vinculan al VAO:

		glBindBuffer(GL_ARRAY_BUFFER, vbo_ids[NORMALS_VBO]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

		// Se suben a un IBO los datos de índices:

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_ids[INDICES_IBO]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindVertexArray(0);		
	}

	Cube::~Cube()
	{
		glDeleteVertexArrays(1, &vao_id);
		glDeleteBuffers(VBO_COUNT, vbo_ids);
	}

	void Cube::add(shared_ptr<Cube > & m) {
		children.push_back(m);
	}

	void Cube::update()
	{
		angle += 0.5;

		//Se actualiza cada hijo
		for (auto & child : children)
		{
			child->update();
		}
	}

	void Cube::render(glm::mat4 model_view_matrix_parent)
	{
		// Se activa el shader del material	     
		material->use();

		// Matriz de transformacion
		glm::mat4 model_view_matrix = model_view_matrix_parent;

		// Se rota el cubo y se mueve a la posicion introducida por parametros:
		model_view_matrix = glm::translate(model_view_matrix, position);
		model_view_matrix = glm::rotate(model_view_matrix, angle, glm::vec3(1.f, 1.f, 0.f));
		model_view_matrix = glm::scale(model_view_matrix, scale);
		glUniformMatrix4fv(material->get_model_view_id(), 1, GL_FALSE, glm::value_ptr(model_view_matrix));

		// Matriz de normales
		glm::mat4 normal_matrix = glm::transpose(glm::inverse(model_view_matrix));
		glUniformMatrix4fv(material->get_normal_matrix_id(), 1, GL_FALSE, glm::value_ptr(normal_matrix));

		// Se selecciona el VAO que contiene los datos del objeto y se dibujan sus elementos:
		glBindVertexArray(vao_id);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_BYTE, 0);
		glBindVertexArray(0);		

		//Se actualiza cada hijo
		for (auto & child : children)
		{
			child->render(model_view_matrix);
		}
	}
}
