#ifndef __ETHER_SHADER_H__
#define __ETHER_SHADER_H__

EAPI int ether_shader_load(const char* vertexPath, const char* fragmentPath, EtherShader *id);
EAPI int ether_shader_unload(EtherShader id);
EAPI int ether_shader_use(EtherShader id);
EAPI int ether_shader_uniform_boolean(EtherShader id, const char* name, int value);
EAPI int ether_shader_uniform_integer(EtherShader id, const char* name, int value);
EAPI int ether_shader_uniform_float(EtherShader id, const char* name, float value);

#endif /* __ETHER_SHADER_H__ */
