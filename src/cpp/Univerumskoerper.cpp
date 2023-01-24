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
#include "Universumskoerper.h"

Universumskoerper::Universumskoerper(const char* path){
    bool res = loadOBJ(path, vertices, uvs, normals);
    gameObjectModel = glm::mat4(1.0f);
}
Universumskoerper::~Universumskoerper() {}
RenderInformation Universumskoerper::getRenderInformation(){
    RenderInformation renderInformation = RenderInformation();
    renderInformation.renderModel = this->gameObjectModel;
    renderInformation.renderVertices = this->vertices;
    renderInformation.renderUvs = this->uvs;
    renderInformation.renderNormals = this->normals;
    renderInformation.maxDistSun = this->maxDistSun;
    renderInformation.minDistSun = this->minDistSun;

    return renderInformation;
}

