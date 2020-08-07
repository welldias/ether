#ifndef _ETHER_H_
#define _ETHER_H_

#include <glad/glad.h> 
#include <GLFW\glfw3.h>

#include <cglm\cglm.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include <ImGuiFileBrowser.h>

#include <string>
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cmath>
#include <functional>
#include <chrono>

#include "exception.h"
#include "property.h"
#include "vector2.h"
#include "vector3.h"
#include "matrix4.h"
#include "rgba.h"
#include "obj_file.h"
#include "texture_loader.h"
#include "noise.h"
#include "mesh.h"
#include "math_util.h"
#include "light.h"
#include "camera.h"
#include "vbo.h"
#include "vao.h"
#include "shader.h"
#include "shader_program.h"
#include "display.h"
#include "render_ui.h"
#include "render.h"
#include "engine.h"

#endif //_ETHER_H_