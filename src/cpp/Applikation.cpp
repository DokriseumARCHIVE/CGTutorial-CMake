#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "objloader.hpp"
#include "texture.hpp"
using namespace glm;
#include "shader.hpp"
#include "objects.hpp"
#include "Applikation.h"

Applikation::Applikation(unsigned int breite, unsigned int hoehe, const char* titel) {
	this->breite = breite;
	this->hoehe = hoehe;
	this->titel = titel;

	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return;
	}

	this->hwnd = glfwCreateWindow(breite, hoehe, titel, NULL, NULL);
}

Applikation::~Applikation(){
	glfwDestroyWindow(this->hwnd);
}

void Applikation::run() {
	if (!hwnd) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(hwnd);
	glewExperimental = true;

	if (glewInit()!=GLEW_OK) {
		fprintf(stderr, "GLEW wurde nicht ordnungsgem‰ﬂ initialisiert.");
		return;
	}

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
	glDepthFunc(GL_LESS);
	//glfwSetKeyCallback(hwnd, key_callback);
	programmID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");
	glUseProgram(programmID);

	
}