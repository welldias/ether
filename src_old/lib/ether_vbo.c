#include <glad/glad.h> 
#include <GLFW\glfw3.h>

#include <Ether.h>

static void _ether_vbo_gpu_store_array(EtherVbo* vbo);
static void _ether_vbo_gpu_store_mesh(EtherVbo* vbo);
static void _ether_vbo_gpu_store_path(EtherVbo* vbo);

EtherVbo* ether_vbo_create(EtherVboType type, void* buffer)
{
	if (buffer == NULL)
		return NULL;

	EtherVbo* vbo = calloc(1, sizeof(EtherVbo));
	if (vbo == NULL)
		return NULL;

	ether_vbo_init(vbo, type, buffer);

	return vbo;
}

void ether_vbo_init(EtherVbo* vbo, EtherVboType type, void* buffer)
{	 
	if (vbo == NULL || buffer == NULL)
		return;

	glGenVertexArrays(1, &(vbo->vaoId));
	glGenBuffers(1, &(vbo->verticesVboId));
	glGenBuffers(1, &(vbo->indicesVboId));

	vbo->type = type;
	vbo->data = buffer;
}	 

void ether_vbo_relesase(EtherVbo* vbo)
{
	if (vbo == NULL)
		return;

	glBindVertexArray(vbo->vaoId);
	glDeleteBuffers(1, &(vbo->verticesVboId));
	glDeleteBuffers(1, &(vbo->indicesVboId));
	glDeleteVertexArrays(1, &(vbo->vaoId));
	glBindVertexArray(0);

	vbo->vaoId = 0;
	vbo->verticesVboId = 0;
	vbo->vaoId = 0;
}

void ether_vbo_destroy(EtherVbo* vbo)
{
	if (vbo == NULL)
		return;

	if (vbo->vaoId != 0)
		ether_vbo_relesase(vbo);

	switch (vbo->type) {
	case ETHER_VBO_TYPE_ARRAY:
		break;
	case ETHER_VBO_TYPE_MESH:
		ether_mesh_destroy((EtherMesh*)vbo->data);
		break;
	case ETHER_VBO_TYPE_PATH:
		break;
	}

	free(vbo);
}

void ether_vbo_gpu_store(EtherVbo* vbo)
{
	if (vbo == NULL || vbo->data == NULL)
		return;

	glBindVertexArray(vbo->vaoId);

	switch (vbo->type) {
	case ETHER_VBO_TYPE_ARRAY:
		_ether_vbo_gpu_store_array(vbo);
		break;
	case ETHER_VBO_TYPE_MESH:
		_ether_vbo_gpu_store_mesh(vbo);
		break;
	case ETHER_VBO_TYPE_PATH:
		_ether_vbo_gpu_store_path(vbo);
		break;
	}

	glBindVertexArray(0);
}

void ether_vbo_draw(EtherVbo* vbo, EtherShader shaderId)
{
	if (vbo == NULL || vbo->data == NULL)
		return;

	glBindVertexArray(vbo->vaoId);

	switch (vbo->type) {
	case ETHER_VBO_TYPE_ARRAY:
		break;
	case ETHER_VBO_TYPE_MESH:
	{
		EtherMesh* mesh = (EtherMesh*)vbo->data;
		glDrawElements(GL_TRIANGLES, (GLsizei)mesh->totalIndices, GL_UNSIGNED_INT, 0);
	}
		break;
	case ETHER_VBO_TYPE_PATH:
		break;
	}

	glBindVertexArray(0);

}

void _ether_vbo_gpu_store_mesh(EtherVbo* vbo)
{
	if (vbo == NULL || vbo->data == NULL)
		return;

	EtherMesh* mesh = (EtherMesh*)vbo->data;
	unsigned int vSize = ether_mesh_vertices_size(mesh);
	unsigned int iSize = ether_mesh_indices_size(mesh);

	glBindBuffer(GL_ARRAY_BUFFER, vbo->verticesVboId);
	glBufferData(GL_ARRAY_BUFFER, vSize, mesh->vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo->indicesVboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, mesh->indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

void _ether_vbo_gpu_store_array(EtherVbo* vbo)
{
	if (vbo == NULL || vbo->data == NULL)
		return;
}

void _ether_vbo_gpu_store_path(EtherVbo* vbo)
{
	if (vbo == NULL || vbo->data == NULL)
		return;
}