#include <Ether.h>

EtherMesh* ether_mesh_create(void)
{
	EtherMesh* mesh = malloc(sizeof(EtherMesh));
	if (mesh == NULL)
		return NULL;

	ether_mesh_init(mesh);

	return mesh;
}

void ether_mesh_init(EtherMesh* mesh)
{	 
	if (mesh == NULL)
		return;

	memset(mesh, 0, sizeof(EtherMesh));
}	 
	 
void ether_mesh_destroy(EtherMesh* mesh)
{	 
	if (mesh == NULL)
		return;

	free(mesh);
}

unsigned int ether_mesh_vertices_size(EtherMesh* mesh)
{
	if (mesh == NULL)
		return 0;

	return (unsigned int)(sizeof(float)* mesh->totalVertices * 3);
}

unsigned int ether_mesh_indices_size(EtherMesh* mesh)
{
	if (mesh == NULL)
		return 0;

	return (unsigned int)(sizeof(int) * mesh->totalIndices);
}

unsigned int ether_mesh_size(EtherMesh* mesh)
{
	if (mesh == NULL)
		return 0;

	return ether_mesh_indices_size(mesh) + ether_mesh_vertices_size(mesh);
}
