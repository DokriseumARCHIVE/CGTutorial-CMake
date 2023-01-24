#ifndef _APPLIKATION_H
#define _APPLIKATION_H

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Applikation {

public: 
	Applikation(unsigned int breite = 500, unsigned int hoehe = 500, const char* titel = "");
	~Applikation();

	void run();
	void sendMVP();

private:
	GLFWwindow* hwnd;
	int breite, hoehe;
	const char* titel;
	unsigned int programmID;
	glm::mat4 projektion;
	glm::mat4 ansicht;
};

#endif