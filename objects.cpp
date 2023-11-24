#include "objects.hpp"



/* -------------------------------------------------------------------------------- */
/*                                       CUBE                                       */
/* -------------------------------------------------------------------------------- */

GLuint VAOblock = 0;
GLuint VAOlamp = 0;

static void createCube()
{
	// Vertex buffer data
	static const GLfloat vertices[] = {
		// Coordinates          // Normals              // Tex coords

		// Back surface (all normals point in the negative z-direction)
		-0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,     0.0f,  0.0f, -1.0f,     0.0f,  0.0f,

		// Front surface (all normals point in the positive z-direction)
		-0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,     0.0f,  0.0f,  1.0f,     0.0f,  0.0f,

		// Left surface (all normals point in the negative x-direction)
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,    -1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,    -1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		// Right surface (all normals point in the positive x-direction)
		 0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,     1.0f,  0.0f,  0.0f,     1.0f,  0.0f,

		 // Lower surface (all normals point in the negative y-direction)
		-0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     0.0f,  1.0f,
		 0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,     0.0f, -1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,     0.0f, -1.0f,  0.0f,     0.0f,  1.0f,

		// Upper surface (all normals point in the positive y-direction)
		-0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     0.0f,  1.0f,
		 0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,     0.0f,  1.0f,  0.0f,     0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,     0.0f,  1.0f,  0.0f,     0.0f,  1.0f
	};

	// Create and bind VBO
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Write vertex data to the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Configure vertex attributes for blocks
	glGenVertexArrays(1, &VAOblock);
	glBindVertexArray(VAOblock);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(GLvoid*)(6 * sizeof(GLfloat)));

	// Configure vertex attributes for lamps
	glGenVertexArrays(1, &VAOlamp);
	glBindVertexArray(VAOlamp);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat),
		(GLvoid*)(6 * sizeof(GLfloat)));

	// Unbind VBO and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void drawBlock()
{
	// Create cube if not already done
	if (! VAOblock)
	{
		createCube();
	}

	// Draw block
	glBindVertexArray(VAOblock);
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void drawLamp()
{
	{
		// Create cube if not already done
		if (! VAOlamp)
		{
			createCube();
		}

		// Draw lamp
		glBindVertexArray(VAOlamp);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}



/* -------------------------------------------------------------------------------- */
/*                                    CROSSHAIR                                     */
/* -------------------------------------------------------------------------------- */

GLuint VAOcrosshair = 0;

static void createCrosshair()
{
	// Vertex buffer data
	static const GLfloat vertices[] = {
		// Vertical rectangle
		-0.002f,  0.03f,    // top left
		 0.002f,  0.03f,    // top right
		 0.002f, -0.03f,    // bottom right
		-0.002f, -0.03f,    // bottom left

		// Horizontal rectangle
		-0.03f,  0.002f,    // left top
		 0.03f,  0.002f,    // right top
		 0.03f, -0.002f,    // right bottom
		-0.03f, -0.002f     // left bottom
	};

	// Element buffer data
	static const GLuint indices[] = {
		// Vertical Rectangle
		0, 1, 2,    // upper right triangle
		0, 3, 2,    // lower left triangle

		// Horizontal rectangle
		4, 5, 6,    // upper right triangle
		4, 7, 6     // lower left triangle
	};

	// Create and bind VAO
	glGenVertexArrays(1, &VAOcrosshair);
	glBindVertexArray(VAOcrosshair);

	// Create and bind VBO
	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// Write vertex data to the buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Configure vertex attributes
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat),
		(GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Create and bind EBO
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	// Write index data to the buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
		GL_STATIC_DRAW);

	// Unbind VBO, VAO and EBO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void drawCrosshair()
{
	// Create crosshair if not already done
	if (! VAOcrosshair)
	{
		createCrosshair();
	}

	// Draw crosshair
	glBindVertexArray(VAOcrosshair);
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);
}