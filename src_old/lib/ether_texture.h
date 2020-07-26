#ifndef __ETHER_TEXTURE_H__
#define __ETHER_TEXTURE_H__

EAPI int ether_texture_load(char const* picturePath, EtherTexture* id);
EAPI int ether_texture_unload(EtherTexture id);

#endif /* __ETHER_TEXTURE_H__ */
