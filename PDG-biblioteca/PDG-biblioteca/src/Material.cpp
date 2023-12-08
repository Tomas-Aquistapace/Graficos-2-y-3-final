#include "Material.h"

Material::Material()
{
	_ambient =  glm::vec3(0.0f, 0.0f, 0.0f);
	_diffuse =  glm::vec3(0.0f, 0.0f, 0.0f);
	_specular = glm::vec3(0.0f, 0.0f, 0.0f);
	_shininess = 0;
}
Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess)
{
	_ambient = ambient;
	_diffuse = diffuse;
	_specular = specular;
	_shininess = shininess;
}