#include "Camera.h"


Camera::Camera(Renderer* rend)
{
	_renderer = rend;
	_position = glm::vec3(-1.0f, 0.0f, 0.0f);
	_target = glm::vec3(0.0f, 0.0f, 0.0f);

	//_renderer = rend;
	//_position = glm::vec3(0.0f, 0.0f, 10.0f);
	//_target = glm::vec3(0.0f, 0.0f, 0.0f);
	//_direction = glm::normalize(_target-_position);
	//_front = _direction;
	//_up = glm::vec3(0.0f, 1.0f, 0.0f);
	//_right = glm::normalize(glm::cross(_up,_front));
	//_up = glm::normalize(glm::cross(_right, _front));

	_frustum = new Frustum(this, 2.0f, 45.0f, 0.1f, 100.0f);
}

Camera::~Camera()
{

}

void Camera::setTransform()
{
	_renderer->updateView(_position,_front,_up);
}

void Camera::setPosition(glm::vec3 position) 
{
	_position = position;
	setTransform();
}

glm::vec3 Camera::getPosition() 
{
	return _position;
}

void Camera::setTarget(glm::vec3 target) 
{
	_target = target;
	setTransform();
}

void Camera::setFront(glm::vec3 front) 
{
	_front = front;
	setTransform();
}

glm::vec3 Camera::getTarget() 
{
	return _target;
}

glm::vec3 Camera::getFront()
{
	return _front;
}

Frustum* Camera::getFrustum()
{
	return _frustum;
}

glm::vec3 Camera::getUp() 
{
	return _up;
}

glm::vec3 Camera::getSide()
{
	return _right;
}

void Camera::setUp(glm::vec3 up)
{
	_up = up;
	setTransform();
}

void Camera::setSide(glm::vec3 side)
{
	_right = side;
	setTransform();
}

void Camera::moveOnWorld(glm::vec3 movement)
{
	_position += movement;
	setTransform();
}

void Camera::moveOnLocal(glm::vec3 movement)
{
	glm::vec3 dir = (movement.x * _right) + (movement.y * _front) + (movement.z * _up);

	_position += dir;
	setTransform();
}

void Camera::rotate(glm::vec3 movement)
{
	yaw += movement.x;
	pitch += movement.y;
	roll += movement.z;

	_front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	_front.y = sin(glm::radians(pitch));
	_front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	_front = glm::normalize(_front);
	
	_up.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch + 90.0f));
	_up.y = sin(glm::radians(pitch + 90.0f));
	_up.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch + 90.0f));

	_right = glm::cross(_front, _up);

	setTransform();
}