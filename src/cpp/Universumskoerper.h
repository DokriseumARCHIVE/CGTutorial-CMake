#ifndef _UNIVERSUMSKOERPER_H
#define _UNIVERSUMSKOERPER_H

#include "GLFW/glfw3.h"
#include "RenderInformation.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>



//1
class Universumskoerper {

private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> location;
    unsigned int rotation;
    unsigned int programID;
    unsigned int textureID;
    glm::mat4 gameObjectModel;
    glm::vec3 minDistSun;
    glm::vec3 maxDistSun;


public:
    Universumskoerper(const char* path);
    ~Universumskoerper();
    RenderInformation getRenderInformation();

};
#endif
