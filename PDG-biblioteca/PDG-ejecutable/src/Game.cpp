#include "Game.h"

Game::Game()
{
	
}

Game ::~Game()
{
	
}

void Game::play()
{
	initBaseGame(1000, 500, "Aquistapace Engine");
	engineLoop(0.1f, 0.1f, 0.1f, 1.0f);
}

void Game::initGame(Renderer* renderer)
{
	_timer = new Timer();
	_timer->start();

	_lightA = new Lightning(renderer);
	_lightA->initializeSpot(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.1f, 0.1f, 0.1f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f), glm::radians(5.0f), 0.09f, 0.032f);
	_lightA->setActiveState(false);

	_lightB = new Lightning(renderer);
	_lightB->initializePoint(glm::vec3(0.0f, 5.0f, -5.0f), vec3(0.1f, 0.1f, 0.1f), vec3(0.5f, 0.5f, 0.5f), vec3(1.0f), 0.09f, 0.032f);
	_lightB->setActiveState(false);

	_lightC = new Lightning(renderer);
	_lightC->initializeDirectional(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.1f), glm::vec3(0.5f), glm::vec3(0.5f));
	_lightC->setActiveState(false);

	_camera = new Camera(renderer);
	_camera->setPosition(glm::vec3(0.0f, 0.0f, 10.0f));
	activateFPSCamera(_camera, 0.05f);

	_customCharacter = new Model("res/Models/Pedrito/Pedrito.dae", renderer, false);
	_customCharacter->setPosition(vec3(0.0f, 0.0f, -5.0f));
	
	//Right Plane
	_planes.push_back(new Plane(glm::vec3(8,0,-5),glm::vec3(-1,0,0)));
	//Left Plane
	_planes.push_back(new Plane(glm::vec3(-8, 0, -5), glm::vec3(1, 0, 0)));
	//Top Plane
	_planes.push_back(new Plane(glm::vec3(0, 8, -5), glm::vec3(0, -1, 0)));
	//Bottom Plane
	_planes.push_back(new Plane(glm::vec3(0, -8, -5), glm::vec3(0, 1, 0)));
	//Far Plane
	_planes.push_back(new Plane(glm::vec3(0, 0, -15), glm::vec3(0, 0, 1)));
	//Near Plane
	_planes.push_back(new Plane(glm::vec3(0, 0, 5), glm::vec3(0, 0, -1)));

	Material obsidian;
	obsidian._diffuse = glm::vec3(0.18275f, 0.17f, 0.22525f);
	obsidian._specular = glm::vec3(0.332741f, 0.328634f, 0.346435f);
	obsidian._shininess = 0.3f * 128.0f;
	
	Material gold;
	gold._diffuse = glm::vec3(0.75164f, 0.60648f, 0.22648f);
	gold._specular = glm::vec3(0.628281f, 0.555802f, 0.366065f);
	gold._shininess = 51.2;

	//BottomRightFar
	_cornerCube.push_back(new Shape(ShapeTypes::cube, gold, renderer));
	_cornerCube[0]->setPosition(glm::vec3(8, -8, -15));
	//TopRightFar
	_cornerCube.push_back(new Shape(ShapeTypes::cube, gold, renderer));
	_cornerCube[1]->setPosition(glm::vec3(8, 8, -15));
	//BottomLeftFar
	_cornerCube.push_back(new Shape(ShapeTypes::cube, gold, renderer));
	_cornerCube[2]->setPosition(glm::vec3(-8, -8, -15));
	//TopLeftFar
	_cornerCube.push_back(new Shape(ShapeTypes::cube, gold, renderer));
	_cornerCube[3]->setPosition(glm::vec3(-8, 8, -15));
	//BottomRightNear
	_cornerCube.push_back(new Shape(ShapeTypes::cube, gold, renderer));
	_cornerCube[4]->setPosition(glm::vec3(8, -8, 5));
	//TopRightNear
	_cornerCube.push_back(new Shape(ShapeTypes::cube, gold, renderer));
	_cornerCube[5]->setPosition(glm::vec3(8, 8, 5));
	//BottomLeftNear
	_cornerCube.push_back(new Shape(ShapeTypes::cube, gold, renderer));
	_cornerCube[6]->setPosition(glm::vec3(-8, -8, 5));
	//TopLeftNear
	_cornerCube.push_back(new Shape(ShapeTypes::cube, gold, renderer));
	_cornerCube[7]->setPosition(glm::vec3(-8, 8, 5));
}

void Game::updateGame(CollisionManager collManager)
{
	// Input:
	if (Input::GetKeyDown(Keycode::ESCAPE))
	{
		exitApplication();
	}

	// Move Camera:
	if (Input::GetKey(Keycode::W))
	{
		_camSpeedY = _camSpeed;
	}
	else if (Input::GetKey(Keycode::S))
	{
		_camSpeedY = -_camSpeed;
	}
	else
	{
		_camSpeedY = 0;
	}
	if (Input::GetKey(Keycode::A))
	{
		_camSpeedX = _camSpeed;
	}
	else if (Input::GetKey(Keycode::D))
	{
		_camSpeedX = -_camSpeed;
	}
	else
	{
		_camSpeedX = 0;
	}



	////input
	//if (input->isKeyDown(GLFW_KEY_UP))
	//{
	//	speedY = 1;
	//}
	//else if (input->isKeyDown(GLFW_KEY_DOWN))
	//{
	//	speedY = -1;
	//}
	//else
	//{
	//	speedY = 0;
	//}

	//if (input->isKeyDown(GLFW_KEY_RIGHT))
	//{
	//	speedX = 1;
	//}
	//else if (input->isKeyDown(GLFW_KEY_LEFT))
	//{
	//	speedX = -1;
	//}
	//else
	//{
	//	speedX = 0;
	//}

	//if (input->isKeyDown(GLFW_KEY_L))
	//{
	//	speedZ = 1;
	//}
	//else if (input->isKeyDown(GLFW_KEY_O))
	//{
	//	speedZ = -1;
	//}
	//else
	//{
	//	speedZ = 0;
	//}

	//if (input->isKeyDown(GLFW_KEY_U))
	//{
	//	growSpeed = 1;
	//}
	//else if (input->isKeyDown(GLFW_KEY_J))
	//{
	//	growSpeed = -1;
	//}
	//else
	//{
	//	growSpeed = 0;
	//}

	//if (input->isKeyDown(GLFW_KEY_S))
	//{
	//	rotXSpeed = 1;
	//}
	//else if (input->isKeyDown(GLFW_KEY_W))
	//{
	//	rotXSpeed = -1;
	//}
	//else
	//{
	//	rotXSpeed = 0;
	//}

	//if (input->isKeyDown(GLFW_KEY_A))
	//{
	//	rotYSpeed = 1;
	//}
	//else if (input->isKeyDown(GLFW_KEY_D))
	//{
	//	rotYSpeed = -1;
	//}
	//else
	//{
	//	rotYSpeed = 0;
	//}

	//if (input->isKeyDown(GLFW_KEY_Q))
	//{
	//	rotZSpeed = 1;
	//}
	//else if (input->isKeyDown(GLFW_KEY_E))
	//{
	//	rotZSpeed = -1;
	//}
	//else
	//{
	//	rotZSpeed = 0;
	//}
	//if (input->isKeyDown(GLFW_KEY_KP_8))
	//{
	//	camSpeedY = 1;
	//}
	//else if (input->isKeyDown(GLFW_KEY_KP_5))
	//{
	//	camSpeedY = -1;
	//}
	//else
	//{
	//	camSpeedY = 0;
	//}
	//if (input->isKeyDown(GLFW_KEY_KP_6))
	//{
	//	camSpeedX = 1;
	//}
	//else if (input->isKeyDown(GLFW_KEY_KP_4))
	//{
	//	camSpeedX = -1;
	//}
	//else
	//{
	//	camSpeedX = 0;
	//}
	//if (input->isKeyDown(GLFW_KEY_KP_7))
	//{
	//	camSpeedZ = 1;
	//}
	//else if (input->isKeyDown(GLFW_KEY_KP_9))
	//{
	//	camSpeedZ = -1;
	//}
	//else
	//{
	//	camSpeedZ = 0;
	//}
	////
	//if (input->isKeyDown(GLFW_KEY_T))
	//{
	//	camTargetY = 1;
	//}
	//else if (input->isKeyDown(GLFW_KEY_G))
	//{
	//	camTargetY = -1;
	//}
	//else
	//{
	//	camTargetY = 0;
	//}
	//if (input->isKeyDown(GLFW_KEY_H))
	//{
	//	camTargetX = 1;
	//}
	//else if (input->isKeyDown(GLFW_KEY_F))
	//{
	//	camTargetX = -1;
	//}
	//else
	//{
	//	camTargetX = 0;
	//}
	//if (input->isKeyDown(GLFW_KEY_R)) //esta parte se puede remover
	//{
	//	camTargetZ = 1;
	//}
	//else if (input->isKeyDown(GLFW_KEY_Y))
	//{
	//	camTargetZ = -1;
	//}
	//else
	//{
	//	camTargetZ = 0;
	//}

	if (Input::GetKeyDown(Keycode::ALPHA1))
	{
		_lightA->setActiveState(!_lightA->getActiveState());
	}
	if (Input::GetKeyDown(Keycode::ALPHA2))
	{
		_lightB->setActiveState(!_lightB->getActiveState());
	}
	if (Input::GetKeyDown(Keycode::ALPHA3))
	{
		_lightC->setActiveState(!_lightC->getActiveState());
	}

	vec3 cameraMovement = vec3(_camSpeedX, _camSpeedY, _camSpeedZ) * 3.0f * _timer->getDT();
	//vec3 cameraRotation = vec3(camTargetX, camTargetY, camTargetZ) * camRotSpeed * timer->getDT();
	_camera->moveOnLocal(glm::vec3 (cameraMovement));
	//_camera->rotate(glm::vec3(cameraRotation));	
	
	vec3 playerMovement = vec3(_speedX, _speedY, _speedZ) * _timer->getDT();
	
	vec3 newPos = _customCharacter->getPosition() + playerMovement*5.0f;
	_customCharacter->setPosition(newPos);
	
	// Prueba de punto especifico en le frustum
	/*if (input->isKeyDown(GLFW_KEY_P)) 
	{
		cout << endl << "PEDRITO MIN: " << "X:" << pedrito->collectiveBBox->GetMinVector().x << "  Y:" << pedrito->collectiveBBox->GetMinVector().y << "  Z:" << pedrito->collectiveBBox->GetMinVector().z;
		cout << endl << "PEDRITO MAX: " << "X:" << pedrito->collectiveBBox->GetMaxVector().x << "  Y:" << pedrito->collectiveBBox->GetMaxVector().y << "  Z:" << pedrito->collectiveBBox->GetMaxVector().z << endl;
		cout << "Point Position:" << endl << " x:" << pedrito->getPosition().x <<
			" y:" << pedrito->getPosition().y << " z:" << pedrito->getPosition().z << endl;
		if (pedrito->collectiveBBox->isOnFrustum(_camera->getFrustum()->getPlanes(),pedrito))
		{
			cout << "InFrustum" << endl;
		}
		else
		{
			cout << "OutOfFrustum" << endl;
		}

		cout << "Point Position:" << endl << " x:" << pedrito->getPosition().x <<
			" y:" << pedrito->getPosition().y << " z:" << pedrito->getPosition().z << endl;
		if (pedrito->collectiveBBox->isOnFrustum(planes, pedrito))
		{
			cout << "InBox" << endl;
		}
		else
		{
			cout << "OutOfBox" << endl;
		}
	}*/

	vec3 newScale = _customCharacter->children[0]->children[0]->children[1]->getScale() + vec3(_growSpeed,_growSpeed,_growSpeed) * _timer->getDT();
	_customCharacter->children[0]->children[0]->children[1]->setScale(newScale); 

	vec3 newRot = _customCharacter->children[0]->children[0]->children[1]->getRotation() + vec3(_rotXSpeed, _rotYSpeed, _rotZSpeed)*10.0f * _timer->getDT();
	_customCharacter->children[0]->children[0]->children[1]->setRotation(newRot);

	_timer->updateTimer();

	_lightA->setPos(_camera->getPosition());
	_lightA->setDir(_camera->getFront());

	for (int i = 0; i < _cornerCube.size(); i++)
	{
		_cornerCube[i]->draw();
	}

	//testing bsp
	_customCharacter->Draw(_planes,_camera);
}
void Game::destroyGame()
{
	if (_timer) delete _timer;
	
	if (_customCharacter) delete _customCharacter;

	if (_camera) delete _camera;
	if (_lightB) delete _lightB;
	if (_lightA) delete _lightA;
	
	_planes.clear();
	_cornerCube.clear();
}
