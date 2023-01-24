#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "objloader.hpp"
#include "texture.hpp"
using namespace glm;
#include "shader.hpp"
#include "objects.hpp"
#define _USE_MATH_DEFINES
#include <math.h>

class Universumskoerper {
	GLuint VertexArrayIDSphere = 0;
	GLuint lats;
	GLuint longs;

	Universumskoerper() {
		
	}

	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool res = loadOBJ("Kugel.obj", vertices, uvs, normals);
};
