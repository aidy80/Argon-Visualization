#ifndef CAMERA_H
#define CAMERA_H

#include "Algebra.h"

class Camera {
public:
    Camera();
    ~Camera();
    void Reset();
    void Orient(Point& eye, Point& focus, Vector& up);
    void Orient(Point& eye, Vector& look, Vector& up);
    void SetViewAngle (double viewAngle);
    void SetNearPlane (double nearPlane);
    void SetFarPlane (double farPlane);
    void SetScreenSize (int screenWidth, int screenHeight);

    Matrix GetProjectionMatrix();
    Matrix GetModelViewMatrix();

    void RotateV(double angle);
    void RotateU(double angle);
    void RotateW(double angle);
    void Rotate(Point p, Vector axis, double degree);
    void Translate(const Vector &v);

    Point GetEyePoint();
    Vector GetLookVector();
    Vector GetUpVector();
    double GetViewAngle();
    double GetNearPlane();
    double GetFarPlane();
    int GetScreenWidth();
    int GetScreenHeight();

    double GetFilmPlaneDepth();
    double GetScreenWidthRatio();

private:
    void printMat(Matrix m);
    void printVec(Vector v);

    Matrix get_uvw2xyz();
    Matrix get_xyz2uvw();
    Matrix xyz2uvw;
    Matrix uvw2xyz;
    Vector u;
    Vector v;
    Vector w;
    Vector get_u();
    Vector get_v();
    Vector get_w();

    Matrix modelView;
    Point cameraPosition;
    Vector look;
    Vector up;
    double viewAngle;
    double nearPlane;
    double farPlane;
    int screenWidth;
    int screenHeight;
};

#endif
