#ifndef __ETHER_SHADER_H__
#define __ETHER_SHADER_H__

EAPI int ether_shader_load(const char* vertexPath, const char* fragmentPath, EtherShader *id);
EAPI int ether_shader_unload(EtherShader id);
EAPI int ether_shader_use(EtherShader id);
EAPI int ether_shader_uniform_boolean(EtherShader id, const char* name, int value);
EAPI int ether_shader_uniform_integer(EtherShader id, const char* name, int value);
EAPI int ether_shader_uniform_float(EtherShader id, const char* name, float value);
EAPI int ether_shader_uniform_vec2(EtherShader id, const char* name, const vec2 value);
EAPI int ether_shader_uniform_vecf2(EtherShader id, const char* name, float x, float y);
EAPI int ether_shader_uniform_vec3(EtherShader id, const char* name, const vec3 value);
EAPI int ether_shader_uniform_vecf3(EtherShader id, const char* name, float x, float y, float z);
EAPI int ether_shader_uniform_vec4(EtherShader id, const char* name, const vec4 value);
EAPI int ether_shader_uniform_vecf4(EtherShader id, const char* name, float x, float y, float z, float w);
EAPI int ether_shader_uniform_mat2(EtherShader id, const char* name, const mat2 mat);
EAPI int ether_shader_uniform_mat3(EtherShader id, const char* name, const mat3 mat);
EAPI int ether_shader_uniform_mat4(EtherShader id, const char* name, const mat4 mat);

#endif /* __ETHER_SHADER_H__ */
