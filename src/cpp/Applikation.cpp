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


RenderInformation renderHelper(Universumskoerper uk);

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
    // Zun�chst k�nnen wir die drei Matrizen einfach kombinieren, da unser einfachster Shader
    // wirklich nur eine Transformationsmatrix ben�tigt, wie in der Vorlesung erkl�rt.
    // Sp�ter werden wir hier auch die Teilmatrizen an den Shader �bermitteln m�ssen.
    // Interessant ist hier, dass man in C++ (wie auch in C#) den "*"-Operator �berladen kann, sodass
    // man Klassenobjekte miteinander multiplizieren kann (hier Matrizen bzw. "mat4"),
    // das ginge in JAVA so nat�rlich nicht.
    glm::mat4 MVP = projektion * ansicht * modell;

    // "glGetUniformLocation" liefert uns eine Referenz auf eine Variable, die im Shaderprogramm
    // definiert ist, in diesem Fall heisst die Variable "MVP".
    // "glUniformMatrix4fv" �bertr�gt Daten, genauer 4x4-Matrizen, aus dem Adressraum unserer CPU
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
    // Buffer zugreifbar f�r die Shader machen
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
		fprintf(stderr, "GLEW wurde nicht ordnungsgem�� initialisiert.");
		return;
	}




	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClearColor(0.4f, 0.2f, 0.9f, 0.9f);

	//glfwSetKeyCallback(hwnd, key_callback);
	programmID = LoadShaders(SHADER_DIR "/StandardShading.vertexshader", SHADER_DIR "/StandardShading.fragmentshader");
	glUseProgram(programmID);

    std::vector<RenderInformation> renderInformationVector;
    Universumskoerper universumskoerper = Universumskoerper("/home/dustin/.projects/2022/CLionProjects/CGTutorial-CMake/src/resources/Kugel.obj");
    renderInformationVector.reserve(1);
    renderInformationVector.push_back(renderHelper(universumskoerper));
    float n = 0.1f;
    float f = 100.0f;
    this->projektion = glm::perspective(45.0f, 16.0f / 9.0f, n, f);
    this->ansicht = glm::lookAt(glm::vec3(0.0, 0.0f, -5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
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
        for (int i = 0; i < renderInformationVector.size(); i++) {
            RenderInformation r = renderInformationVector[i];
            sendMVP(r.renderModel);
            glBindVertexArray(r.renderVertexArray);
            glDrawArrays(GL_TRIANGLES, 0, r.renderVertices.size());
        }
        glUniform1i(glGetUniformLocation(programmID, "myTextureSampler"), 0);
        glm::vec4 lightPos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f)) * glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        glUniform3f(glGetUniformLocation(programmID, "LightPosition_worldspace"), lightPos.x, lightPos.y,
                    lightPos.z);
        glfwSwapBuffers(hwnd);

    }

}

void Applikation::sendMVP(glm::mat4 gameObjectModel) {
    glm::mat4 MVP = projektion * ansicht * gameObjectModel;

    glUniformMatrix4fv(glGetUniformLocation(programmID, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programmID, "M"), 1, GL_FALSE, &gameObjectModel[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programmID, "V"), 1, GL_FALSE, &ansicht[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(programmID, "P"), 1, GL_FALSE, &projektion[0][0]);
}

//2
//helper method for setting ub the vertex-, uv- and normalbuffer
RenderInformation Applikation::renderHelper(Universumskoerper uk) {
    RenderInformation ri = uk.getRenderInformation();
    std::vector<glm::vec3> vertices = ri.renderVertices;
    std::vector<glm::vec2> uvs = ri.renderUvs;
    std::vector<glm::vec3> normals = ri.renderNormals;

    GLuint VertexArrayIDSausage;
    glGenVertexArrays(1, &VertexArrayIDSausage);
    glBindVertexArray(VertexArrayIDSausage);

    // Ein ArrayBuffer speichert Daten zu Eckpunkten (hier xyz bzw. Position)
    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer); // Kennung erhalten
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer); // Daten zur Kennung definieren
    // Buffer zugreifbar f?r die Shader machen
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

    // Erst nach glEnableVertexAttribArray kann DrawArrays auf die Daten zugreifen...
    glEnableVertexAttribArray(0); // siehe layout im vertex shader: location = 0
    glVertexAttribPointer(0,  // location = 0
                          3,  // Datenformat vec3: 3 floats fuer xyz
                          GL_FLOAT,
                          GL_FALSE, // Fixedpoint data normalisieren ?
                          0, // Eckpunkte direkt hintereinander gespeichert
                          (void*)0); // abweichender Datenanfang ?

    GLuint normalbuffer; // Hier alles analog f?r Normalen in location == 2
    glGenBuffers(1, &normalbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2); // siehe layout im vertex shader
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    GLuint uvbuffer; // Hier alles analog f?r Texturkoordinaten in location == 1 (2 floats u und v!)
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1); // siehe layout im vertex shader
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    RenderInformation rhi = ri;
    rhi.renderVertexArray = VertexArrayIDSausage;
    rhi.renderVertices = vertices;
    rhi.renderModel = ri.renderModel;
    return rhi;
}
