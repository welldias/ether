#include <cglm\cglm.h>

#include "ether.h"

namespace ether {

	const float Camera::DEFAULT_CAMERA_YAW         = -90.0f;
	const float Camera::DEFAULT_CAMERA_PITCH       =   0.0f;
	const float Camera::DEFAULT_CAMERA_SPEED       =   2.5f;
	const float Camera::DEFAULT_CAMERA_SENSITIVITY =   0.1f;
	const float Camera::DEFAULT_CAMERA_ZOOM        =  45.0f;

	Camera::Camera() {

		front.SetForward();

		position.Zero();
		worldUp.SetYup();

		// default camera
		movementSpeed = DEFAULT_CAMERA_SPEED;
		mouseSensitivity = DEFAULT_CAMERA_SENSITIVITY;
		zoom = DEFAULT_CAMERA_ZOOM;
		yaw = DEFAULT_CAMERA_YAW;
		pitch = DEFAULT_CAMERA_PITCH;

		Update();
	}

	Camera::~Camera() {
	}

	void Camera::Configure(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) {
		
		front.SetForward();

		position.Set(posX, posY, posZ);
		worldUp.Set(upX, upY, upZ);
		movementSpeed = DEFAULT_CAMERA_SPEED;
		mouseSensitivity = DEFAULT_CAMERA_SENSITIVITY;
		zoom = DEFAULT_CAMERA_ZOOM;
		this->yaw = yaw;
		this->pitch = pitch;

		Update();
	}

	// returns the view matrix calculated using Euler Angles and the LookAt Matrix
	void Camera::ViewMatrix(Matrix4& dest) {
		LookAt((position + front), dest);
	}

	/* This funcion is based on glm_lookat() function of cglm project */
	void Camera::Perspective(float aspect, float nearVal, float farVal, Matrix4& dest) {

		glm_perspective(MathUtil::Radian(zoom), aspect, nearVal, farVal, dest.m);

#if 0
		float f, fn;

		dest.Zero();

		f = 1.0f / tanf(MathUtil::Radian(zoom) * 0.5f);
		fn = 1.0f / (nearVal - farVal);

		dest(0, 0) = f / aspect;
		dest(1, 1) = f;
		dest(2, 2) = (nearVal + farVal) * fn;
		dest(2, 3) = -1.0f;
		dest(3, 2) = 2.0f * nearVal * farVal * fn;
#endif
	}



	/* This funcion is based on glm_lookat() function of cglm project */
	void Camera::LookAt(Vector3 center, Matrix4& dest) {

		glm_lookat(position.v, center.v, up.v, dest.m);
#if 0
		Vector3 f, u, s;

		//glm_vec3_sub(center, eye, f);
		f = center - position;

		//glm_vec3_normalize(f);
		f.Normalize();

		//glm_vec3_crossn(f, up, s);
		s = f.Crossproduct(up).Normalize();

		//glm_vec3_cross(s, f, u);
		u = s.Crossproduct(f);

		dest(0, 0) =  s[0];
		dest(0, 1) =  u[0];
		dest(0, 2) = -f[0];
		dest(1, 0) =  s[1];
		dest(1, 1) =  u[1];
		dest(1, 2) = -f[1];
		dest(2, 0) =  s[2];
		dest(2, 1) =  u[2];
		dest(2, 2) = -f[2];
		dest(3, 0) = -s.Dotproduct(position); // glm_vec3_dot(s, eye);
		dest(3, 1) = -u.Dotproduct(position); // glm_vec3_dot(u, eye);
		dest(3, 2) =  f.Dotproduct(position); // glm_vec3_dot(f, eye);
		dest(0, 3) = dest(1,3) = dest(2, 3) = 0.0f;
		dest(3, 3) = 1.0f;
#endif
	}

	void Camera::MoveForward(float deltaTime) {

		//TODO: talvez seja melhor já passar o velocity do que o deltaTime
		float velocity = movementSpeed * deltaTime;

		Vector3 vecAux(front);
		position += vecAux.Scale(velocity);

	}
	void Camera::MoveBackward(float deltaTime) {
		//TODO: talvez seja melhor já passar o velocity do que o deltaTime
		float velocity = movementSpeed * deltaTime;

		Vector3 vecAux(front);
		position -= vecAux.Scale(velocity);
	}

	void Camera::MoveLeft(float deltaTime) {
		//TODO: talvez seja melhor já passar o velocity do que o deltaTime
		float velocity = movementSpeed * deltaTime;

		Vector3 vecAux(right);
		position -= vecAux.Scale(velocity);
	}

	void Camera::MoveRight(float deltaTime) {
		//TODO: talvez seja melhor já passar o velocity do que o deltaTime
		float velocity = movementSpeed * deltaTime;

		Vector3 vecAux(right);
		position += vecAux.Scale(velocity);
	}

	void Camera::PointTo(float x, float y, bool bound)
	{
		yaw   += x * mouseSensitivity;
		pitch += y * mouseSensitivity;

		// make sure that when pitch is out of bounds, screen doesn't get flipped
		if (bound) {
			if (pitch > 89.0f) pitch = 89.0f;
			if (pitch < -89.0f) pitch = -89.0f;
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
		vecAux[0] = cos(MathUtil::Radian(yaw)) * cos(MathUtil::Radian(pitch));
		vecAux[1] = sin(MathUtil::Radian(pitch));
		vecAux[2] = sin(MathUtil::Radian(yaw)) * cos(MathUtil::Radian(pitch));

		front = vecAux.Normalize();

		// also re-calculate the Right and Up vector
		right = front.Crossproduct(worldUp).Normalize();

		// normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		up = right.Crossproduct(front).Normalize();
	}
}
