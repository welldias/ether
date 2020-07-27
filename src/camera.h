#ifndef __ETHER_CAMERA_H__
#define __ETHER_CAMERA_H__

namespace ether {

	class Camera
	{
    public:
        Camera();
        ~Camera();

        void Configure(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
        void ViewMatrix(Matrix4& dest);

        void MoveForward(float deltaTime);
        void MoveBackward(float deltaTime);
        void MoveLeft(float deltaTime);
        void MoveRight(float deltaTime);
        void PointTo(float x, float y, bool bound);
        void Zoom(float offset);
        void Perspective(float aspect, float nearVal, float farVal, Matrix4& dest);

        inline float Zoom() const { return zoom; }

    private:
        void Update();
        void LookAt(Vector3 center, Matrix4& dest);

    private:
        // camera Attributes
        Vector3 position;
        Vector3 front;
        Vector3 up;
        Vector3 right;
        Vector3 worldUp;

        // euler Angles
        float yaw;
        float pitch;
        float rot;

        // camera options
        float movementSpeed;
        float mouseSensitivity;
        float zoom;

    private:
        static const float DEFAULT_CAMERA_YAW;
        static const float DEFAULT_CAMERA_PITCH;
        static const float DEFAULT_CAMERA_SPEED;
        static const float DEFAULT_CAMERA_SENSITIVITY;
        static const float DEFAULT_CAMERA_ZOOM;

	};
}

#endif // __ETHER_CAMERA_H__