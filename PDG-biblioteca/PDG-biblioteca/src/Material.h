#pragma once
#include <glm/vec3.hpp>
#include "Exports.h"
class SABASAENGINE_API Material
{
public:
    glm::vec3 _ambient;
    glm::vec3 _diffuse;
    glm::vec3 _specular;
    float _shininess;
    Material();
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);
};