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
        void Update();

    private:
        void LookAt(Vector3 center, Matrix4& dest);

    public:
        // camera Attributes
        Vector3 Position;
        Vector3 Front;
        Vector3 Up;
        Vector3 Right;
        Vector3 WorldUp;

        // euler Angles
        float Yaw;
        float Pitch;
        float Rotation;

        // camera options
        float movementSpeed;
        float mouseSensitivity;
        float zoom;

    public:
        static const float DefaultCameraYaw;
        static const float DefaultCameraPitch;
        static const float DefaultCameraSpeed;
        static const float DefaultCameraSensitivity;
        static const float DefaultCameraZoom;

	};
}

#endif // __ETHER_CAMERA_H__