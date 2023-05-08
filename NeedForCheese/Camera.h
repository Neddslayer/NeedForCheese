#ifndef CAMERA_H
typedef struct Vector2
{
    float x;                // Vector x component
    float y;                // Vector y component
} Vector2;

typedef struct Camera2D
{
    Vector2 offset;         // Camera offset (displacement from target)
    Vector2 target;         // Camera target (rotation and zoom origin)
    float rotation;         // Camera rotation in degrees
    float zoom;             // Camera zoom (scaling), should be 1.0f by default
} Camera2D;

#endif // !CAMERA_H
