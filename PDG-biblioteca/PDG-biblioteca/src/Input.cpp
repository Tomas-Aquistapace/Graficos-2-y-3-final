#include "Input.h"

static GLFWwindow* _window;
static Camera* _camera;

static void null_mouse_callback(GLFWwindow* _window, double xpos, double ypos);
static void mouse_callback(GLFWwindow* _window, double xpos, double ypos);
void UpdateValuesFPS();

float _yaw = -90.f;
float _pitch = 0.0f;
float _lastX = 800.f / 2;
float _lastY = 600.f / 2;

bool _firstMouse = true;

float _sensitivity = 1.5f;

static int usedInputs = 0;
static const int _inputsListSize = 12;
static int _inputs[_inputsListSize];

// ---------------------------------

void Input::SetWindow(GLFWwindow* window)
{
	_window = window;
}

bool Input::GetKey(Keycode key)
{
	return (glfwGetKey(_window, key) == GLFW_PRESS);
}

bool Input::GetKeyDown(Keycode key)
{
	if (glfwGetKey(_window, key) == GLFW_PRESS)
	{
		for (int i = 0; i < _inputsListSize; i++)
			if (key == _inputs[i])
				return false;

		_inputs[usedInputs] = key;
		usedInputs++;

		if (usedInputs >= _inputsListSize)
			usedInputs = 0;

		return true;
	}
	return false;
}

bool Input::GetKeyUp(Keycode key)
{
	return (glfwGetKey(_window, key) == GLFW_RELEASE);
}

void Input::ActivateFPSCamera(GLFWwindow* _window, Camera* camera, float sensitivity)
{
	glfwSetCursorPosCallback(_window, mouse_callback);
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	_camera = camera;

	_sensitivity = sensitivity;
}

void Input::DeactivateFPSCamera(GLFWwindow* _window)
{
	glfwSetCursorPosCallback(_window, null_mouse_callback);
	glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Input::CheckClearInputList()
{
	for (int i = 0; i < _inputsListSize; i++)
	{
		if (_inputs[i] != -1)
			if (glfwGetKey(_window, _inputs[i]) == GLFW_RELEASE)
				_inputs[i] = -1;
	}
}

// ---------------------------------

void null_mouse_callback(GLFWwindow* _window, double xpos, double ypos) { }

void mouse_callback(GLFWwindow* _window, double xpos, double ypos)
{
	if (_firstMouse)
	{
		_lastX = xpos;
		_lastY = ypos;
		_firstMouse = false;
	}

	float xoffset = xpos - _lastX;
	float yoffset = _lastY - ypos; // reversed since y-coordinates go from bottom to top
	_lastX = xpos;
	_lastY = ypos;

	xoffset *= _sensitivity;
	yoffset *= _sensitivity;

	_yaw += xoffset;
	_pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (_pitch > 89.0f)
		_pitch = 89.0f;
	if (_pitch < -89.0f)
		_pitch = -89.0f;

	UpdateValuesFPS();
}

void UpdateValuesFPS()
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(_yaw)) * cos(glm::radians(_pitch));
	direction.y = sin(glm::radians(_pitch));
	direction.z = sin(glm::radians(_yaw)) * cos(glm::radians(_pitch));

	_camera->setFront(glm::normalize(direction));
	_camera->setSide(glm::normalize(glm::cross(glm::vec3(0, 1, 0), _camera->getFront())));
	_camera->setUp(glm::normalize(glm::cross(_camera->getFront(), _camera->getSide())));
}