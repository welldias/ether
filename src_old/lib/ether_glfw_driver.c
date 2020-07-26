#include <glad/glad.h> 
#include <GLFW\glfw3.h>

#include <Ether.h>

#include "ether_glfw_driver.h"

static const char*        glfwWindowText = "Ether Opengl";
static const char*        glfwInfoDescription = "GLFW Open Source multi-platform library for OpenGL";
static const char*        glfwDescriptionError = NULL;
static int                glfwCodeError = 0;
static int                glfwSystemStarted = FALSE;
static const unsigned int glfwScreenWidth  = 800;
static const unsigned int glfwScreenHeight = 600;

static GLFWwindow* glfwMainWindow = NULL;

static int  _ether_glfw_driver_video_setup();
static int  _ether_glfw_driver_create_window();
static int  _ether_glfw_driver_video_shutdown();
static void _ether_glfw_driver_video_error_callback(int error, const char* description);
static void _ether_glfw_driver_video_window_size_callback(GLFWwindow* window, int width, int height);
static EtherDisplayStatus _ether_glfw_driver_status();


int ether_glfw_driver_display(EtherDisplayCommand cmd, int lparm1, void* pparm1) 
{
	switch (cmd)
	{
	case ETHER_DISPLAY_GET_VERSION:
		return 0;
	case ETHER_DISPLAY_GET_DESCRIPTION:
	case ETHER_DISPLAY_INIT:
	case ETHER_DISPLAY_CLEAR:
	case ETHER_DISPLAY_QUIT:
	case ETHER_DISPLAY_POINT:
	case ETHER_DISPLAY_LINE:
	case ETHER_DISPLAY_CLOSED_LINE:
	case ETHER_DISPLAY_POLY:
	case ETHER_DISPLAY_BOX:
	case ETHER_DISPLAY_BEGIN_FRAME:
	case ETHER_DISPLAY_END_FRAME:
	case ETHER_DISPLAY_TEXT:
	case ETHER_DISPLAY_TEXT_POSITION:
	case ETHER_DISPLAY_GET_TEXTWIDTH:
	case ETHER_DISPLAY_GET_TEXTHEIGHT:
	case ETHER_DISPLAY_SET_RASTER:
	case ETHER_DISPLAY_GET_RASTER:
	case ETHER_DISPLAY_CAN_GOURAUD:
	case ETHER_DISPLAY_UPDATE_PALETTE:
	case ETHER_DISPLAY_SET_SHADING:
	default: break;
	}
	return 0;
}

int ether_glfw_driver_video(EtherVideoCommand cmd, int lparm1, void* pparm1) 
{
	switch (cmd)
	{
	case ETHER_VIDEO_GET_STATUS:
		return (int)_ether_glfw_driver_status();
	case ETHER_VIDEO_GET_VERSION:
	{
		return GLFW_VERSION_MAJOR;
	}
	case ETHER_VIDEO_GET_DESCRIPTION:
	{
		if (lparm1 <= 0 || pparm1 == NULL)
			return 0;

		char* p = (char*)pparm1;

		const char* glfwVersion = glfwGetVersionString();
		if (glfwVersion == NULL) {
			strcpy_s(p, strnlen_s(glfwInfoDescription, lparm1), glfwInfoDescription);
			return 0;
		}
		
		sprintf_s(p, lparm1, "GlFW %s", glfwVersion);
		return 0;
	}
	case ETHER_VIDEO_SETUP:
		return _ether_glfw_driver_video_setup();
	case ETHER_VIDEO_SHUTDOWN:
		return _ether_glfw_driver_video_shutdown();
	case ETHER_VIDEO_GET_ERROR:
	{
		if (glfwDescriptionError != NULL) {
			char* p = (char*)pparm1;
			strcpy_s(p, strnlen_s(glfwDescriptionError, lparm1), glfwDescriptionError);
		}
		return glfwCodeError;
	}
	case ETHER_VIDEO_GET_MODE:
		return 0;
	case ETHER_VIDEO_SET_DRAW_PAGE:
		break;
	case ETHER_VIDEO_SET_VIEW_PAGE:
		break;
	case ETHER_VIDEO_GET_NPAGES:
		return 1;
	case ETHER_VIDEO_HAS_PALETTE:
		return 1;
	case ETHER_VIDEO_SET_PALETTE:
		return 0;
	case ETHER_VIDEO_SET_NTSC:
		break;
	case ETHER_VIDEO_CHECK_RETRACE:
		break;
	case ETHER_VIDEO_GET_RASTER:
	{
		*((EtherRaster**)pparm1) = NULL;
		break;
	}
	case ETHER_VIDEO_BLIT:
		break;
	case ETHER_VIDEO_CURSOR_HIDE:
		break;
	case ETHER_VIDEO_CURSOR_SHOW:
		break;
	case ETHER_VIDEO_CURSOR_RESET:
		break;
	case ETHER_VIDEO_CURSOR_MOVE:
		break;
	case ETHER_VIDEO_CURSOR_SET_APPEARANCE:
		break;
	default:
		break;
	}

	return 0;
}

int _ether_glfw_driver_video_setup()
{
	if (glfwSystemStarted)
		return 0;

	glfwSetErrorCallback(_ether_glfw_driver_video_error_callback);

	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwSystemStarted = TRUE;

	return _ether_glfw_driver_create_window();
}

int _ether_glfw_driver_create_window()
{
	glfwMainWindow = glfwCreateWindow(glfwScreenWidth, glfwScreenHeight, glfwWindowText, NULL, NULL);
	if (glfwMainWindow == NULL)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(glfwMainWindow);
	glfwSetFramebufferSizeCallback(glfwMainWindow, _ether_glfw_driver_video_window_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		return -1;

	return 0;
}

int _ether_glfw_driver_video_shutdown()
{
	glfwSystemStarted = FALSE;

	glfwTerminate();

	glfwMainWindow = NULL;

	return 1;
}

void _ether_glfw_driver_video_error_callback(int error, const char* description)
{
	glfwDescriptionError = description;
	glfwCodeError = error;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void  _ether_glfw_driver_video_window_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

EtherDisplayStatus _ether_glfw_driver_status()
{
	if (glfwMainWindow == NULL)
		return ETHER_DISPLAY_STOPPED;

	if (glfwGetKey(glfwMainWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		return ETHER_DISPLAY_STOPPED;

	glfwSwapBuffers(glfwMainWindow);
	glfwPollEvents();

	return ETHER_DISPLAY_RUNNING;
}
