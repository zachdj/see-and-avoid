#include "CubeDrawer.h"
#include <vector>


CubeDrawer::CubeDrawer(Texture & tex1, Texture & tex2, Shader & defaultShader)
{
	this->tex1 = tex1;
	this->tex2 = tex2;
	this->defaultShader = defaultShader;

	//vertices and texture coords for a cube
	GLfloat vertices[] = {
		//positions			  //texture coords
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	
	//create VAO for a cube
	GLuint VBO, VAO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO); //bind vertex array
	// bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//set vertex attributes pointers - position and tex coords
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); //unbind array buffer
	glBindVertexArray(0); //unbind vertex array	
	this->VAO = VAO;
}

void CubeDrawer::Draw(glm::mat4 view, glm::mat4 projection, GLfloat timeValue, std::vector<Cube>& cubes)
{
	// draw each cube
	for (GLuint i = 0; i < cubes.size(); i++) {
		glm::mat4 model;
		Cube current = cubes[i];
		this->defaultShader.Use();

		// load up textures
		this->tex1.Use(GL_TEXTURE0);
		glUniform1i(glGetUniformLocation(this->defaultShader.Program, "texture1"), 0);
		this->tex2.Use(GL_TEXTURE1);
		glUniform1i(glGetUniformLocation(this->defaultShader.Program, "texture2"), 1);

		//bind VAO and transformation matrices then draw
		glBindVertexArray(VAO);		
		glUniformMatrix4fv(glGetUniformLocation(this->defaultShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(this->defaultShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		//apply the cube's current position and rotation
		model = glm::translate(model, current.position);
		// apply linear velocity
		GLfloat positionChangeMagnitude = current.velocityMagnitude * timeValue;
		model = glm::translate(model, current.velocityDirection * positionChangeMagnitude);
		// set current rotation
		model = glm::rotate(model, glm::radians(current.rotationAngle), current.rotationAxis);
		//apply angular velocity
		model = glm::rotate(model, glm::radians(current.angularVelocityMagnitude*timeValue), current.angularVelocityAxis);
		model = glm::scale(model, glm::vec3(16.0f, 16.0f, 16.0f));

		glUniformMatrix4fv(glGetUniformLocation(this->defaultShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0); //unbind vertex array

		//unbind textures after drawing
		Texture::Unbind();
	}
}


CubeDrawer::~CubeDrawer()
{
}
