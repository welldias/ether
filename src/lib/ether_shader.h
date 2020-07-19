#ifndef __ETHER_SHADER_H__
#define __ETHER_SHADER_H__

EAPI int ether_shader_load(const char* vertexPath, const char* fragmentPath, Shader *id);
EAPI int ether_shader_unload(Shader id);
EAPI int ether_shader_use(Shader id);
EAPI int ether_shader_uniform_boolean(Shader id, const char* name, int value);
EAPI int ether_shader_uniform_integer(Shader id, const char* name, int value);
EAPI int ether_shader_uniform_float(Shader id, const char* name, float value);

#endif /* __ETHER_SHADER_H__ */
