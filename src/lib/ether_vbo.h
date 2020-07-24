#ifndef __ETHER_VBO_H__
#define __ETHER_VBO_H__

typedef enum _Ether_Vbo_Type
{
	ETHER_VBO_TYPE_ARRAY = 0,
	ETHER_VBO_TYPE_MESH,
	ETHER_VBO_TYPE_PATH,
} EtherVboType;

struct _Ether_Vbo
{
	unsigned int vaoId;
	unsigned int verticesVboId;
	unsigned int indicesVboId;

	void* data;
	EtherVboType type;
};

EAPI EtherVbo*  ether_vbo_create    (EtherVboType type, void* buffer);
EAPI void       ether_vbo_init      (EtherVbo* vbo, EtherVboType type, void* buffer);
EAPI void       ether_vbo_destroy(EtherVbo* vbo);
EAPI void       ether_vbo_gpu_store (EtherVbo* vbo);
EAPI void       ether_vbo_draw      (EtherVbo* vbo);

#endif /* __ETHER_VBO_H__ */
