
/*	
	ADRIAN PONCE BALSEIRO
	ESNE G4.3 DDVJ
	adrianpb95@gmail.com
	10 / 06 / 2018
*/


////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include "View.hpp"
#include <glm/glm.hpp>                          // vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp>                 // value_ptr
#include <SFML/OpenGL.hpp>

#include "Vertex_Shader.hpp"
#include "Fragment_Shader.hpp"
#include "Material_Shaders.hpp"


////////////////////////////////////////////////////////////
// NameSpaces
////////////////////////////////////////////////////////////
namespace example
{
    View::View(int width, int height)    
    {	
		//Creacion de los tipos de materiales
		materials["mapa"]  = make_shared<Material>(vertex_shader_code, fragment_shader_code, "../../../../assets/Heightmap_top.tga");
		materials["checker1"] = make_shared<Material>(vertex_shader_code, fragment_shader_code, "../../../../assets/uv-checker-1.tga");
		materials["checker2"] = make_shared<Material>(vertex_shader_code, fragment_shader_code, "../../../../assets/uv-checker-2.tga");
		materials["madera1"] = make_shared<Material>(vertex_shader_code, fragment_shader_code, "../../../../assets/wood-crate-1.tga");
		materials["madera2"] = make_shared<Material>(vertex_shader_code, fragment_shader_code, "../../../../assets/wood-crate-2.tga");
		
		//Creacion de los elementos de la escena
		camera.reset(new Camera());
		skybox.reset(new Skybox("../../../../assets/sky-cube-map-"));			
		elevation_mesh.reset(new Elevation_Mesh(materials["mapa"], materials["checker1"], glm::vec3(0.f, -2.f, -8.f), glm::vec3(1.f, 1.f, 1.f), 12, 12, 5.f, 5.f, 2.f));
		cube.reset(new Cube(materials["checker1"], glm::vec3(-2.f, 1.f, -5.f), glm::vec3(0.5f, 0.5f, 0.5f)));
		mesh.reset(new Mesh(materials["madera1"], glm::vec3(2.f, 1.f, -5.f), glm::vec3(1.f, 1.f, 1.f), "../../../../assets/chair.obj"));
		
		//Inicializacion de los hijos
		shared_ptr<Cube> cube2_ptr = make_shared<Cube>(materials["madera1"], glm::vec3(0.f, 0.f, -2.f), glm::vec3(0.5f, 0.5f, 0.5f));
		cube->add(cube2_ptr);
		shared_ptr<Mesh> mesh2_ptr = make_shared<Mesh>(materials["checker2"], glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.5f, 0.5f, 0.5f), "../../../../assets/chair.obj");
		mesh->add(mesh2_ptr);

		//Creacion del framebuffer de postproceso
		build_framebuffer();
		
		//Configuracion de elementos de OpenGL
        glEnable (GL_DEPTH_TEST);
        glEnable (GL_CULL_FACE);

		//Creacion de shader de postproceso
		shader_program_postprocess.attach(Vertex_Shader(Shader::Source_Code::from_string(effect_vertex_shader_code)));
		shader_program_postprocess.attach(Fragment_Shader(Shader::Source_Code::from_string(effect_fragment_shader_code)));
		shader_program_postprocess.link();
				
		//Configuracion de la ventana
        resize (width, height);
    }

    void View::update ()
    {
		//Update de los elementos
		elevation_mesh->update();
		cube->update();
		mesh->update();		
    }

    void View::render ()
    {
		glViewport(0, 0, framebuffer_width, framebuffer_height);

		// Se activa el framebuffer de la textura
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);     

		//Skybox tiene su propio shader que activa y desactiva 
		skybox->render(*camera);        
      
		glClear(GL_DEPTH_BUFFER_BIT);	
		
		//Cada elemento tiene su propio material que gestiona los shaders

		elevation_mesh->render(*camera);

		mesh->render(camera->get_model_view());

		cube->render(camera->get_model_view());

		//Se renderiza el postproceso
		render_framebuffer();
		
    }

    void View::resize (int new_width, int new_height)
    {
        width  = new_width;
        height = new_height;		

        camera->set_ratio (float(width) / height);

		for (auto & mat : materials)
		{
			mat.second->use();
			glm::mat4 projection_matrix = camera->get_projection();
			glUniformMatrix4fv(mat.second->get_projection_id(), 1, GL_FALSE, glm::value_ptr(projection_matrix));
		}
		
        glViewport (0, 0, width, height);		
    }

	void View::on_key(sf::Keyboard::Key key, bool down)
	{			
		//Movimiento de la camara
		glm::mat4 camera_rotation;
		if (down) 
		{
			switch (key)
			{	
			//Hacia delante / Hacia atras
			case sf::Keyboard::W: {camera->move(glm::vec3(camera->get_direction().x*0.3f, camera->get_direction().y*0.3f, camera->get_direction().z*0.3f)); break; }
			case sf::Keyboard::S: {camera->move(glm::vec3(camera->get_direction().x*-0.3f, camera->get_direction().y*-0.3f, camera->get_direction().z*-0.3f)); break; }

			//Arriba / Abajo
			case sf::Keyboard::Q: {camera->move(glm::vec3(0.0f,0.5f,0.0f)); break; }
			case sf::Keyboard::E: {camera->move(glm::vec3(0.0f, -0.5f, 0.0f)); break; }

			//Giro derecha / Giro izquierda
			case sf::Keyboard::D: {camera->rotate(glm::mat4(glm::rotate(camera_rotation, glm::degrees(-0.15f), glm::vec3(0.f, 1.f, 0.f)))); break; }
			case sf::Keyboard::A: {camera->rotate(glm::mat4(glm::rotate(camera_rotation, glm::degrees(0.15f), glm::vec3(0.f, 1.f, 0.f)))); break; }
			}			
		}	
	}  

	void View::build_framebuffer()
	{
		// Se crea un framebuffer en el que poder renderizar:
		{
			glGenFramebuffers(1, &framebuffer_id);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_id);
		}

		// Se crea una textura que será el búfer de color vinculado al framebuffer:
		{
			glGenTextures(1, &out_texture_id);
			glBindTexture(GL_TEXTURE_2D, out_texture_id);

			// El búfer de color tendrá formato RGB:

			glTexImage2D
			(
				GL_TEXTURE_2D,
				0,
				GL_RGB,
				framebuffer_width,
				framebuffer_height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				0
			);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}

		// Se crea un Z-Buffer para usarlo en combinación con el framebuffer:
		{
			glGenRenderbuffers(1, &depthbuffer_id);
			glBindRenderbuffer(GL_RENDERBUFFER, depthbuffer_id);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, framebuffer_width, framebuffer_height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthbuffer_id);
		}

		// Se configura el framebuffer:
		{
			glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, out_texture_id, 0);

			const GLenum draw_buffer = GL_COLOR_ATTACHMENT0;

			glDrawBuffers(1, &draw_buffer);
		}

		// Se comprueba que el framebuffer está listo:

		assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

		// Se desvincula el framebuffer:

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// Se crea el quad para hacer el render del framebuffer:

		static const GLfloat triangle_positions[] =
		{
			+1.0f, -1.0f, 0.0f,
			+1.0f, +1.0f, 0.0f,
			-1.0f, +1.0f, 0.0f,
			-1.0f, +1.0f, 0.0f,
			-1.0f, -1.0f, 0.0f,
			+1.0f, -1.0f, 0.0f

		};

		static const GLfloat triangle_texture_uv[] =
		{
			+1.0f,  0.0f,
			+1.0f, +1.0f,
			0.0f, +1.0f,
			0.0f, +1.0f,
			0.0f,  0.0f,
			+1.0f,  0.0f


		};

		glGenBuffers(1, &triangle_vbo0);
		glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo0);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_positions), triangle_positions, GL_STATIC_DRAW);

		glGenBuffers(1, &triangle_vbo1);
		glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo1);
		glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_texture_uv), triangle_texture_uv, GL_STATIC_DRAW);

	}

	void View::render_framebuffer()
	{			
		glViewport(0, 0, width, height);

		// Se activa el framebuffer de la ventana:

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		shader_program_postprocess.use();

		// Se activa la textura del framebuffer:

		glBindTexture(GL_TEXTURE_2D, out_texture_id);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, triangle_vbo1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

		glDisable(GL_DEPTH_TEST);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		glEnable(GL_DEPTH_TEST);
	}

}
