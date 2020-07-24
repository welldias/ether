#ifndef __ETHER_MESH_H__
#define __ETHER_MESH_H__

struct _Ether_Mesh
{
	float* vertices;
	unsigned int* indices;

	unsigned int totalVestices;
	unsigned int totalIndices;
};

EAPI EtherMesh*   ether_mesh_create       (void);
EAPI void         ether_mesh_init         (EtherMesh* mesh);
EAPI void         ether_mesh_destroy      (EtherMesh* mesh);
EAPI unsigned int ether_mesh_vertices_size(EtherMesh* mesh);
EAPI unsigned int ether_mesh_indices_size (EtherMesh* mesh);
EAPI unsigned int ether_mesh_size         (EtherMesh* mesh);

#endif /* __ETHER_MESH_H__ */
