#include "Mesh.h"

Mesh::Mesh(vector<Vertex> vertices,vector<unsigned int> indices,vector<meshTexture> textures, bool usesSpecularMaps, Renderer* rend) 
{
	_vertices = vertices;
	_indices = indices;
	_textures = textures;
	_usesSpecularMaps = usesSpecularMaps;
	_rend = rend;
	setupMesh();
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1,&vbo);
	glGenBuffers(1, &ebo);


	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,_vertices.size()* sizeof(Vertex), &_vertices[0],GL_STATIC_DRAW);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(unsigned int), &_indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),(void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

	glBindVertexArray(0);
}

void Mesh::Draw()
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	for (unsigned int i = 0; i < _textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		string number;
		string name = _textures[i].type;
		string matStructName = "mat.";
		if(name=="texture_diffuse")
		{
			number = to_string(diffuseNr++);
		}
		else if(name == "texture_specular")
		{
			number = to_string(specularNr++);
		}
		else if (name == "texture_normal")
		{
			number = to_string(normalNr++);
		}
		else if (name == "texture_height")
		{
			number = to_string(heightNr++);
		}
		_rend->setMesh((matStructName + name + number).c_str(),i,_usesSpecularMaps);
		glBindTexture(GL_TEXTURE_2D, _textures[i].id);
	}

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES,_indices.size(),GL_UNSIGNED_INT,0);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}