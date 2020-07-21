#include <glad/glad.h> 

#include <GLFW\glfw3.h>

#include <Ether.h>
#include <ether_glfw_driver.h>
#include <stdio.h>

#define VEC3_BACK     ((vec3){0,  0, -1})
#define VEC3_DOWN     ((vec3){0, -1,  0})
#define VEC3_FORWARD  ((vec3){0,  0,  1})
#define VEC3_LEFT     ((vec3){-1, 0,  0})
#define VEC3_RIGHT    ((vec3){1,  0,  0})
#define VEC3_UP       ((vec3){0,  1,  0})

int global_totalVestices = 0;
int global_totalTriangles = 0;

float* global_vertices = NULL;
unsigned int* global_triangles = NULL;

/* converting rgb color to opengl format */
//float out = (1.0f / 255) * byte_in;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void build_planet_mash(int resolution, vec3 localUp);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Ether Opengl", NULL, NULL);
    if (window == NULL)
    {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD\n");
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED: %s\n", infoLog);
    }
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: %s\n", infoLog);
    }
    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED: %s\n", infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
#if FALSE
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };

    size_t countVertices = sizeof(vertices);
    size_t countIndices = sizeof(indices);

#else
    build_planet_mash(4, VEC3_FORWARD);
    
    size_t countVertices = sizeof(float) * global_totalVestices * 3;
    size_t countIndices  = sizeof(int) * global_totalTriangles;

    float * vertices = global_vertices;
    unsigned int* indices = global_triangles;

    /*
    {
        int i = 0;
        int x = 0;
        for (x = 0; x < global_totalVestices; x++)
        {
            i = x * 3;
            printf("x:%02.02f y:%02.02f z:%02.02f\n", *(vertices + i), *(vertices + i + 1), *(vertices + i + 2));
        }

        for (x = 0; x < (global_totalTriangles/3); x++)
        {
            i = x * 3;
            printf("i:%d i:%d i:%d\n", *(indices + i), *(indices + i + 1), *(indices + i + 2));
        }

    }
    */
#endif

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, countVertices, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, countIndices, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep the EBO bound.
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);

    // uncomment this call to draw in wireframe polygons.
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time 

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void build_planet_mash(int resolution, vec3 localUp)
{
    global_totalVestices = resolution * resolution;
    global_totalTriangles = (resolution - 1) * (resolution - 1) * 6;

    global_vertices = (float*)malloc(sizeof(float) * global_totalVestices * 3);
    global_triangles = (unsigned int*)malloc(sizeof(int) * global_totalTriangles);

    int i = 0;
    int t = 0;

    //vec3 axisA = { localUp[1], localUp[2], localUp[0] };
    vec3 axisA = { localUp[1], localUp[2], localUp[0] };
    vec3 axisB;

    glm_vec3_cross(localUp, axisA, axisB);

    vec2 percent;
    vec3 aux;
    vec3 pointOnUnitCube;

    for (int y = 0; y < resolution; y++) {
        for (int x = 0; x < resolution; x++) {
            i = x + y * resolution;

            glm_vec3_zero(pointOnUnitCube);
            glm_vec3_zero(aux);

            percent[0] = (float)x / (resolution - 1);
            percent[1] = (float)y / (resolution - 1);

            glm_vec3_scale(axisA, ((percent[0] - 0.5f) * 2), aux);
            glm_vec3_add(aux, pointOnUnitCube, pointOnUnitCube);

            glm_vec3_scale(axisB, ((percent[1] - 0.5f) * 2), aux);
            glm_vec3_add(aux, pointOnUnitCube, pointOnUnitCube);

            glm_vec3_add(localUp, pointOnUnitCube, pointOnUnitCube);
            //glm_vec3_normalize(pointOnUnitCube);

            glm_vec3_copy(pointOnUnitCube, global_vertices + (i * 3));

            if (x != resolution - 1 && y != resolution - 1)
            {
                global_triangles[t] = i;
                global_triangles[t + 1] = i + resolution + 1;
                global_triangles[t + 2] = i + resolution;

                global_triangles[t + 3] = i;
                global_triangles[t + 4] = i + 1;
                global_triangles[t + 5] = i + resolution + 1;

                t += 6;
            }
        }
    }
}