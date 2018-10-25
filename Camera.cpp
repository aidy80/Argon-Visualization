#include "Camera.h"
#include <iostream>

Camera::Camera() {
    cameraPosition = Point(0, 0, 0);
    look = Vector(0, 0, 0);
    up = Vector(0, 0, 0);
    viewAngle = 0;
    nearPlane = 1;
    farPlane = 2;
    screenWidth = 1;
    screenHeight = 1;
    u = get_u();
    v = get_v();
    w = get_w();
    xyz2uvw = get_xyz2uvw();
    uvw2xyz = get_uvw2xyz();
}

Camera::~Camera() {
}

void Camera::Reset() {
    cameraPosition = Point(2, 2, 2);
    look = Vector(-1, -1, -1);
    up = Vector(0, 1, 0);
    viewAngle = 45;
    nearPlane = 1;
    farPlane = 20;
    screenWidth = 500;
    screenHeight = 500;
    u = get_u();
    v = get_v();
    w = get_w();
    xyz2uvw = get_xyz2uvw();
    uvw2xyz = get_uvw2xyz();

    Vector trans(-cameraPosition[0], -cameraPosition[1], -cameraPosition[2]);
	Matrix translation = trans_mat(trans);

    modelView = xyz2uvw * translation;
}

void Camera::Orient(Point& eye, Point& focus, Vector& up) {
    this->cameraPosition = eye;
    Vector newLook(focus[0] - eye[0], focus[1] - eye[1], focus[2] - eye[2]);
    this->look = newLook;
    this->up = up;
    u = get_u();
    v = get_v();
    w = get_w();
    xyz2uvw = get_xyz2uvw();
    uvw2xyz = get_uvw2xyz();
    Vector trans(-cameraPosition[0], -cameraPosition[1], -cameraPosition[2]);
	Matrix translation = trans_mat(trans);

    modelView = xyz2uvw * translation;
}


void Camera::Orient(Point& eye, Vector& look, Vector& up) {
    this->cameraPosition = eye;
    this->look = look;
    this->up = up;
    u = get_u();
    v = get_v();
    w = get_w();
    xyz2uvw = get_xyz2uvw();
    uvw2xyz = get_uvw2xyz();
    Vector trans(-cameraPosition[0], -cameraPosition[1], -cameraPosition[2]);
	Matrix translation = trans_mat(trans);

    modelView = xyz2uvw * translation;
}

Matrix Camera::GetProjectionMatrix() {
    Vector scale_Vec(1 / (tan((viewAngle / 2) * (PI/180)) * farPlane), 
                     1 / (tan((viewAngle / 2) * (PI/180)) * farPlane *
                                                        GetScreenWidthRatio()), 
                                                                 1 / farPlane);
    Matrix scaleMat = scale_mat(scale_Vec);

    double c = - nearPlane / farPlane;
    Matrix unhinge(1, 0, 0, 0,
                   0, 1, 0, 0,
                   0, 0, -1 / (c+1), c / (c + 1),
                   0, 0, -1, 0);

	return unhinge * scaleMat;
}


void Camera::SetViewAngle (double viewAngle) {
    this->viewAngle = viewAngle;
}

void Camera::SetNearPlane (double nearPlane) {
    this->nearPlane = nearPlane;
}

void Camera::SetFarPlane (double farPlane) {
    this->farPlane = farPlane;
}

void Camera::SetScreenSize (int screenWidth, int screenHeight) {
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
}

Matrix Camera::GetModelViewMatrix() {
    return modelView;
}

void Camera::RotateV(double angle) {
    Rotate(cameraPosition, v, -angle);
}

void Camera::RotateU(double angle) {
    Rotate(cameraPosition, u, -angle);
}

void Camera::RotateW(double angle) {
    Rotate(cameraPosition, w, angle);
}

void Camera::Translate(const Vector &v) {
    cameraPosition = cameraPosition + v;
}

void Camera::Rotate(Point p, Vector axis, double degrees) {
    modelView = modelView * rot_mat(p, axis, degrees * (PI/180)); 
}

Point Camera::GetEyePoint() {
	return cameraPosition;
}

Vector Camera::GetLookVector() {
	return look;
}

Vector Camera::GetUpVector() {
	return up;
}

double Camera::GetViewAngle() {
	return viewAngle;
}

double Camera::GetNearPlane() {
	return nearPlane;
}

double Camera::GetFarPlane() {
	return farPlane;
}

int Camera::GetScreenWidth() {
	return screenWidth;
}

int Camera::GetScreenHeight() {
	return screenHeight;
}

double Camera::GetFilmPlaneDepth() {
	return farPlane - nearPlane;
}

double Camera::GetScreenWidthRatio() {
	return screenHeight / screenWidth;
}

Matrix Camera::get_uvw2xyz() {
    return Matrix(
        u[0], v[0], w[0], 0,
        u[1], v[1], w[1], 0,
        u[2], v[2], w[2], 0,
        0, 0, 0, 1
    );
}

Matrix Camera::get_xyz2uvw() {
    return Matrix(
        u[0], u[1], u[2], 0,
        v[0], v[1], v[2], 0,
        w[0], w[1], w[2], 0,
        0, 0, 0, 1
    );
}

Vector Camera::get_u() {
    Vector u = cross(up, get_w()); 
    u.normalize();
    return u;
}

Vector Camera::get_v() {
    Vector v = cross(get_w(), get_u());
    v.normalize();
    return v;
}

Vector Camera::get_w() {
    Vector w = look * -1;
    w.normalize();
    return w;
}

void Camera::printMat (Matrix m) {
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            std::cout << m(i, j) << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n";
}

void Camera::printVec(Vector v) {
    for(int i = 0; i < 3; i++) {
        std::cout << v[i] << " ";
    }
    std::cout << "\n\n";
}
