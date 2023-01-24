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
#include "Universumskoerper.h"
#include "asset.hpp"



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

void Applikation::sendMVP()
{
    // Zunächst können wir die drei Matrizen einfach kombinieren, da unser einfachster Shader
    // wirklich nur eine Transformationsmatrix benötigt, wie in der Vorlesung erklärt.
    // Später werden wir hier auch die Teilmatrizen an den Shader übermitteln mässen.
    // Interessant ist hier, dass man in C++ (wie auch in C#) den "*"-Operator überladen kann, sodass
    // man Klassenobjekte miteinander multiplizieren kann (hier Matrizen bzw. "mat4"),
    // das ginge in JAVA so natürlich nicht.
    glm::mat4 MVP = projektion * ansicht * modell;

    // "glGetUniformLocation" liefert uns eine Referenz auf eine Variable, die im Shaderprogramm
    // definiert ist, in diesem Fall heisst die Variable "MVP".
    // "glUniformMatrix4fv" überträgt Daten, genauer 4x4-Matrizen, aus dem Adressraum unserer CPU
    // (vierter Parameter beim Funktionsaufruf, wir generieren mit "&" hier einen Pointer auf das erste
    //  Element, und damit auf das gesamte Feld bzw den Speicherbereich)
    // in den Adressraum der GPUs. Beim ersten Parameter
    // muss eine Referenz auf eine Variable im Adressraum der GPU angegeben werden.
    glUniformMatrix4fv(glGetUniformLocation(programmID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    // Aufgabe 6
    glUniformMatrix4fv(glGetUniformLocation(programmID, "M"), 1, GL_FALSE, &modell[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programmID, "V"), 1, GL_FALSE, &ansicht[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programmID, "P"), 1, GL_FALSE, &projektion[0][0]);
}

void drawVertices(Universumskoerper uk) {
    GLuint VertexArrayIDSphere;
    glGenVertexArrays(1, &VertexArrayIDSphere);
    glBindVertexArray(VertexArrayIDSphere);
    // A5.3
    // Ein ArrayBuffer speichert Daten zu Eckpunkten (hier xyz bzw. Position)
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer); // Kennung erhalten
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); // Daten zur Kennung definieren
    // Buffer zugreifbar für die Shader machen
    glBufferData(GL_ARRAY_BUFFER, uk.getRenderInformation().renderVertices.size() * sizeof(glm::vec3), &uk.getRenderInformation().renderVertices[0], GL_STATIC_DRAW);

    // A5.3
    // Erst nach glEnableVertexAttribArray kann DrawArrays auf die Daten zugreifen...
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,                            3,                            GL_FLOAT,                          GL_FALSE,                           0,                           (void*) 0);

}

void Applikation::run() {
	if (!this->hwnd) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(hwnd);
	glewExperimental = true;

	if (glewInit()!=GLEW_OK) {
		fprintf(stderr, "GLEW wurde nicht ordnungsgemäß initialisiert.");
		return;
	}




	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.4f, 0.2f, 0.9f, 0.9f);

	//glfwSetKeyCallback(hwnd, key_callback);
	programmID = LoadShaders(SHADER_DIR "/StandardShading.vertexshader", SHADER_DIR "/StandardShading.fragmentshader");
	glUseProgram(programmID);
    Universumskoerper universumskoerper = Universumskoerper("/home/dustin/.projects/2022/CLionProjects/CGTutorial-CMake/src/resources/teapot.obj");
    this->projektion = glm::perspective(45.0f, 16.0f / 9.0f, 12.0f, 12.0f);
    this->ansicht = glm::lookAt(glm::vec3(0.0, 0.0f, -1.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    //----------------------------

    //----------------------------

    while (!glfwWindowShouldClose(hwnd)) {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        projektion = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
        ansicht = glm::lookAt(glm::vec3(0,0,-5),
                           glm::vec3(0,0,0),
                           glm::vec3(0,1,0));
        modell = glm::mat4(1.0f);

        glUniform1i(glGetUniformLocation(programmID, "myTextureSampler"), 0);

        sendMVP();
        glfwSwapBuffers(hwnd);
        //glBindVertexArray();
    }
}
