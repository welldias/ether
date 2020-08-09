#include "ether.h"

namespace ether {

	const float Camera::DefaultCameraYaw          = -90.0f;
	const float Camera::DefaultCameraPitch        =   0.0f;
	const float Camera::DefaultCameraSpeed        =   2.5f;
	const float Camera::DefaultCameraSensitivity  =   0.1f;
	const float Camera::DefaultCameraZoom         =  45.0f;

	Camera::Camera() {

		Front.SetForward();

		Position.Zero();
		WorldUp.SetYup();

		// default camera
		movementSpeed = DefaultCameraSpeed;
		mouseSensitivity = DefaultCameraSensitivity;
		zoom = DefaultCameraZoom;
		Yaw = DefaultCameraYaw;
		Pitch = DefaultCameraPitch;

		Update();
	}

	Camera::~Camera() {
	}

	void Camera::Configure(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) {
		
		Front.SetForward();

		Position.Set(posX, posY, posZ);
		WorldUp.Set(upX, upY, upZ);
		movementSpeed = DefaultCameraSpeed;
		mouseSensitivity = DefaultCameraSensitivity;
		zoom = DefaultCameraZoom;
		this->Yaw = yaw;
		this->Pitch = pitch;

		Update();
	}

	// returns the view matrix calculated using Euler Angles and the LookAt Matrix
	void Camera::ViewMatrix(Matrix4& dest) {
		LookAt((Position + Front), dest);
	}

	/* This funcion is based on glm_lookat() function of cglm project */
	void Camera::Perspective(float aspect, float nearVal, float farVal, Matrix4& dest) {

		glm_perspective(MathUtil::Radian(zoom), aspect, nearVal, farVal, dest.m);
	}

	/* This funcion is based on glm_lookat() function of cglm project */
	void Camera::LookAt(Vector3 center, Matrix4& dest) {
		glm_lookat(Position.v, center.v, Up.v, dest.m);
	}

	void Camera::MoveForward(float deltaTime) {

		//TODO: talvez seja melhor já passar o velocity do que o deltaTime
		float velocity = movementSpeed * deltaTime;

		Vector3 vecAux(Front);
		Position += vecAux.Scale(velocity);

	}
	void Camera::MoveBackward(float deltaTime) {
		//TODO: talvez seja melhor já passar o velocity do que o deltaTime
		float velocity = movementSpeed * deltaTime;

		Vector3 vecAux(Front);
		Position -= vecAux.Scale(velocity);
	}

	void Camera::MoveLeft(float deltaTime) {
		//TODO: talvez seja melhor já passar o velocity do que o deltaTime
		float velocity = movementSpeed * deltaTime;

		Vector3 vecAux(Right);
		Position -= vecAux.Scale(velocity);
	}

	void Camera::MoveRight(float deltaTime) {
		//TODO: talvez seja melhor já passar o velocity do que o deltaTime
		float velocity = movementSpeed * deltaTime;

		Vector3 vecAux(Right);
		Position += vecAux.Scale(velocity);
	}

	void Camera::PointTo(float x, float y, bool bound)
	{
		Yaw   += x * mouseSensitivity;
		Pitch += y * mouseSensitivity;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (bound) {
			if (Pitch > 89.0f) Pitch = 89.0f;
			if (Pitch < -89.0f) Pitch = -89.0f;
		}

		Update();
	}

	// processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void Camera::Zoom(float offset)
	{
		zoom -= offset;

		if (zoom < 1.0f)  zoom = 1.0f;
		if (zoom > 45.0f) zoom = 45.0f;
	}

	// calculates the front vector from the Camera's (updated) Euler Angles
	void Camera::Update() {
		Vector3 vecAux;

		// calculate the new Front vector
		vecAux[0] = cos(MathUtil::Radian(Yaw)) * cos(MathUtil::Radian(Pitch));
		vecAux[1] = sin(MathUtil::Radian(Pitch));
		vecAux[2] = sin(MathUtil::Radian(Yaw)) * cos(MathUtil::Radian(Pitch));

		Front = vecAux.Normalize();

		// also re-calculate the Right and Up vector
		Right = Front.Crossproduct(WorldUp).Normalize();

		// normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = Right.Crossproduct(Front).Normalize();
	}
}
