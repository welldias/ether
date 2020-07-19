#include <glad/glad.h> 

#include <Ether.h>

static const char* SHADER_TYPE_VERTEX   = "VERTEX";
static const char* SHADER_TYPE_FRAGMENT = "FRAGMENT";
static const char* SHADER_TYPE_PROGRAM  = "PROGRAM";

static int _ether_shader_check_compile_erros(EtherShader id, const char* type);
static int _ether_shader_file_loader(const char* fileName, char** shaderSource);

int ether_shader_load(const char* vertexPath, const char* fragmentPath, EtherShader* id)
{
    int result = -1;
    char* vertextCode = NULL;
    char* fragmentCode = NULL;
    
    unsigned int vertex;
    unsigned int fragment;

    if (vertexPath == NULL || fragmentPath == NULL || id == NULL)
        goto stop_return;
    if (_ether_shader_file_loader(vertexPath, &vertextCode) <= 0)
        goto stop_return;
    if(_ether_shader_file_loader(fragmentPath, &fragmentCode) <= 0)
        goto stop_return;

    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, (const GLchar* const*)&vertextCode, NULL);
    glCompileShader(vertex);
    if(_ether_shader_check_compile_erros(vertex, SHADER_TYPE_VERTEX) != 0)
        goto stop_return;

    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, (const GLchar* const*)&fragmentCode, NULL);
    glCompileShader(fragment);
    if (_ether_shader_check_compile_erros(fragment, SHADER_TYPE_FRAGMENT) != 0)
        goto stop_return;

    *id = glCreateProgram();
    glAttachShader(*id, vertex);
    glAttachShader(*id, fragment);
    glLinkProgram(*id);
    if (_ether_shader_check_compile_erros(*id, SHADER_TYPE_PROGRAM) != 0)
        goto stop_return;

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    result = 0;

stop_return:
    if (vertextCode == NULL) free(vertextCode);
    if (fragmentCode == NULL) free(fragmentCode);
    
    return result;
}

int ether_shader_unload(EtherShader id)
{
    glDeleteProgram(id);
    return 0;
}

int ether_shader_use(EtherShader id)
{
    glUseProgram(id);
    return 0;
}

int ether_shader_uniform_boolean(EtherShader id, const char* name, int value)
{
    if (name == NULL)
        return -1;

    glUniform1i(glGetUniformLocation(id, name), value);
    return 0;
}

int ether_shader_uniform_integer(EtherShader id, const char* name, int value)
{
    if (name == NULL)
        return -1;

    glUniform1i(glGetUniformLocation(id, name), value);
    return 0;
}

int ether_shader_uniform_float(EtherShader id, const char* name, float value)
{
    if (name == NULL)
        return -1;

    glUniform1f(glGetUniformLocation(id, name), value);
    return 0;
}

int _ether_shader_check_compile_erros(EtherShader id, const char* type)
{
    int success = FALSE;
    char infoLog[512];

    memset(infoLog, 0, sizeof(infoLog));

    if(strcmp(SHADER_TYPE_PROGRAM, type) != 0)
    {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(id, sizeof(infoLog), NULL, infoLog);
            printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n* %s\n", type, infoLog);
            return -1;
        }
    }
    else
    {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(id, sizeof(infoLog), NULL, infoLog);
            printf("ERROR::PROGRAM_LINKING_ERROR of type: %s\n* %s\n", type, infoLog);
            return -1;
        }
    }

    return 0;
}

int _ether_shader_file_loader(const char* fileName, char **shaderSource)
{
    errno_t fileError;
    FILE* shaderFile = NULL;
    long size = 0;
    char* contentData = NULL;

    if (fileName == NULL || shaderSource == NULL)
        return -1;

    fileError = fopen_s(&shaderFile, fileName, "rb");
    if (fileError != 0)
        return -1;
    /* I hate YOU Microsoft */
    if(shaderFile == NULL)
        return -1;

    fseek(shaderFile, 0, SEEK_END);
    size = ftell(shaderFile);
    rewind(shaderFile);
    
    *shaderSource = contentData = (char*)malloc((size + (size_t)1));
    if (contentData == NULL)
    {
        fclose(shaderFile);
        return -1;
    }

    if (fread(contentData, size, 1, shaderFile) != 1)
    {
        free(contentData);
        fclose(shaderFile);
        return -1;
    }

    contentData[size] = 0;
    fclose(shaderFile);

    return size;
}
