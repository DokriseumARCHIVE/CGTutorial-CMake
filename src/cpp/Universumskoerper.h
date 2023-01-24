#ifndef _UNIVERSUMSKOERPER_H
#define _UNIVERSUMSKOERPER_H

#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Universumskoerper {

public:
	Universumskoerper();
	~Universumskoerper();

private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> location;
    unsigned int rotationsgeschwindigkeit;
    unsigned int programmID;
    unsigned int texturID;
    double abstandSonneMinimal;
    double abstandSonneMaximal;
    glm::mat4 modellPlanet;
    double durchmesser, masse, rotationsgeschwindigkeit;
    char* name;
};

#endif