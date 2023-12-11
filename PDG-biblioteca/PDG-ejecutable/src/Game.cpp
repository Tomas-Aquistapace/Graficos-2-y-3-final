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
	_lightB->initializePoint(glm::vec3(0.0f, 5.0f, 0.0f), vec3(0.1f, 0.1f, 0.1f), vec3(0.5f, 0.5f, 0.5f), vec3(1.0f), 0.09f, 0.032f);
	_lightB->setActiveState(false);

	_lightC = new Lightning(renderer);
	_lightC->initializeDirectional(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.1f), glm::vec3(0.5f), glm::vec3(0.5f));
	_lightC->setActiveState(false);

	_camera = new Camera(renderer);
	_camera->setPosition(glm::vec3(0.0f, 0.0f, 15.0f));
	activateFPSCamera(_camera, 0.05f);

	_customCharacter = new Model("res/Models/Pedrito/Pedrito.dae", renderer, false);
	_customCharacter->setPosition(vec3(0.0f, 0.0f, 0.0f));
	
	//Right Plane
	_planes.push_back(new Plane(glm::vec3(8, 0, 0),glm::vec3(-1,0,0)));
	//Left Plane
	_planes.push_back(new Plane(glm::vec3(-8, 0, 0), glm::vec3(1, 0, 0)));
	//Top Plane
	_planes.push_back(new Plane(glm::vec3(0, 8, 0), glm::vec3(0, -1, 0)));
	//Bottom Plane
	_planes.push_back(new Plane(glm::vec3(0, -8, 0), glm::vec3(0, 1, 0)));
	//Far Plane
	_planes.push_back(new Plane(glm::vec3(0, 0, -10), glm::vec3(0, 0, 1)));
	//Near Plane
	_planes.push_back(new Plane(glm::vec3(0, 0, 10), glm::vec3(0, 0, -1)));

	Material obsidian;
	obsidian._diffuse = glm::vec3(0.18275f, 0.17f, 0.22525f);
	obsidian._specular = glm::vec3(0.332741f, 0.328634f, 0.346435f);
	obsidian._shininess = 0.3f * 128.0f;
	
	Material gold;
	gold._diffuse = glm::vec3(0.75164f, 0.60648f, 0.22648f);
	gold._specular = glm::vec3(0.628281f, 0.555802f, 0.366065f);
	gold._shininess = 51.2;

	//Far Plane
	_planesShapes.push_back(new Shape(ShapeTypes::rectangle, obsidian, renderer));
	_planesShapes[0]->setPosition(glm::vec3(0, 0, -10));
	_planesShapes[0]->setScale(glm::vec3(20, 20, 20));
	//Right Plane
	_planesShapes.push_back(new Shape(ShapeTypes::rectangle, obsidian, renderer));
	_planesShapes[1]->setPosition(glm::vec3(8, 0, 0));
	_planesShapes[1]->setRotation(glm::vec3(0, 90, 0));
	_planesShapes[1]->setScale(glm::vec3(20, 20, 20));
	//Left Plane
	_planesShapes.push_back(new Shape(ShapeTypes::rectangle, obsidian, renderer));
	_planesShapes[2]->setPosition(glm::vec3(-8, 0, 0));
	_planesShapes[2]->setRotation(glm::vec3(0, 90, 0));
	_planesShapes[2]->setScale(glm::vec3(20, 20, 20));

	//BottomRightFar
	_cornerCube.push_back(new Shape(ShapeTypes::cube, gold, renderer));
	_cornerCube[0]->setPosition(glm::vec3(8, -8, -10));
	//TopRightFar
	_cornerCube.push_back(new Shape(ShapeTypes::cube, gold, renderer));
	_cornerCube[1]->setPosition(glm::vec3(8, 8, -10));
	//BottomLeftFar
	_cornerCube.push_back(new Shape(ShapeTypes::cube, gold, renderer));
	_cornerCube[2]->setPosition(glm::vec3(-8, -8, -10));
	//TopLeftFar
	_cornerCube.push_back(new Shape(ShapeTypes::cube, gold, renderer));
	_cornerCube[3]->setPosition(glm::vec3(-8, 8, -10));
	//BottomRightNear
	_cornerCube.push_back(new Shape(ShapeTypes::cube, gold, renderer));
	_cornerCube[4]->setPosition(glm::vec3(8, -8, 10));
	//TopRightNear
	_cornerCube.push_back(new Shape(ShapeTypes::cube, gold, renderer));
	_cornerCube[5]->setPosition(glm::vec3(8, 8, 10));
	//BottomLeftNear
	_cornerCube.push_back(new Shape(ShapeTypes::cube, gold, renderer));
	_cornerCube[6]->setPosition(glm::vec3(-8, -8, 10));
	//TopLeftNear
	_cornerCube.push_back(new Shape(ShapeTypes::cube, gold, renderer));
	_cornerCube[7]->setPosition(glm::vec3(-8, 8, 10));
}

void Game::updateGame(CollisionManager collManager)
{
	// ----------------- Close Window -----------------------

	if (Input::GetKeyDown(Keycode::ESCAPE))
	{
		exitApplication();
	}

	// ----------------- Camera Movement -----------------------

	if (Input::GetKey(Keycode::W))
	{
		_camSpeedY = 1;
	}
	else if (Input::GetKey(Keycode::S))
	{
		_camSpeedY = -1;
	}
	else
	{
		_camSpeedY = 0;
	}

	if (Input::GetKey(Keycode::A))
	{
		_camSpeedX = 1;
	}
	else if (Input::GetKey(Keycode::D))
	{
		_camSpeedX = -1;
	}
	else
	{
		_camSpeedX = 0;
	}

	if (Input::GetKey(Keycode::LEFT_SHIFT))
	{
		_camSpeedZ = 1;
	}
	else if (Input::GetKey(Keycode::LEFT_CONTROL))
	{
		_camSpeedZ = -1;
	}
	else
	{
		_camSpeedZ = 0;
	}

	// ----------------- Character Movement -----------------------

	if (Input::GetKey(Keycode::RIGHT_SHIFT))
	{
		_speedY = 1;
	}
	else if (Input::GetKey(Keycode::RIGHT_CONTROL))
	{
		_speedY = -1;
	}
	else
	{
		_speedY = 0;
	}

	if (Input::GetKey(Keycode::RIGHT))
	{
		_speedX = 1;
	}
	else if (Input::GetKey(Keycode::LEFT))
	{
		_speedX = -1;
	}
	else
	{
		_speedX = 0;
	}

	if (Input::GetKey(Keycode::UP))
	{
		_speedZ = 1;
	}
	else if (Input::GetKey(Keycode::DOWN))
	{
		_speedZ = -1;
	}
	else
	{
		_speedZ = 0;
	}

	// ----------------- Character Rotation -----------------------

	if (Input::GetKey(Keycode::U))
	{
		_rotXSpeed = 1;
	}
	else if (Input::GetKey(Keycode::J))
	{
		_rotXSpeed = -1;
	}
	else
	{
		_rotXSpeed = 0;
	}

	if (Input::GetKey(Keycode::I))
	{
		_rotYSpeed = 1;
	}
	else if (Input::GetKey(Keycode::K))
	{
		_rotYSpeed = -1;
	}
	else
	{
		_rotYSpeed = 0;
	}

	if (Input::GetKey(Keycode::O))
	{
		_rotZSpeed = 1;
	}
	else if (Input::GetKey(Keycode::L))
	{
		_rotZSpeed = -1;
	}
	else
	{
		_rotZSpeed = 0;
	}

	// ----------------- Activate Planes ---------------------

	if (Input::GetKeyDown(Keycode::P))
	{
		_planesActives = !_planesActives;
	}

	// ----------------- Check Frustrum ---------------------

	if (Input::GetKeyDown(Keycode::T))
	{
		if (_customCharacter->collectiveBBox->isOnFrustum(_camera->getFrustum()->getPlanes(), _customCharacter))
		{
			cout << "InFrustum" << endl;
		}
		else
		{
			cout << "OutOfFrustum" << endl;
		}

		if (_customCharacter->collectiveBBox->isOnFrustum(_planes, _customCharacter))
		{
			cout << "InBox" << endl;
		}
		else
		{
			cout << "OutOfBox" << endl;
		}
	}

	// ----------------- Activate Frustrum -----------------------
	
	if (Input::GetKeyDown(Keycode::F))
	{
		_testFrustrum = !_testFrustrum;

		cout << endl;
		cout << "_testFrustrum: " << _testFrustrum << endl;
		cout << endl;
	}

	// ----------------- Update & Draw -----------------------

	vec3 cameraMovement = vec3(_camSpeedX, _camSpeedY, _camSpeedZ) * _camSpeed * _timer->getDT();
	_camera->moveOnLocal(glm::vec3 (cameraMovement));	

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

	_lightA->setPos(_camera->getPosition());
	_lightA->setDir(_camera->getFront());
	
	vec3 playerMovement = vec3(_speedX, _speedY, _speedZ) * _timer->getDT();
	vec3 newPos = _customCharacter->getPosition() + playerMovement * _charMoveSpeed;
	_customCharacter->setPosition(newPos);
	
	vec3 newRot = _customCharacter->children[0]->children[0]->children[1]->getRotation() + vec3(_rotXSpeed, _rotYSpeed, _rotZSpeed) * _charRotationSpeed * _timer->getDT();
	_customCharacter->children[0]->children[0]->children[1]->setRotation(newRot);

	_timer->updateTimer();

	if (_planesActives)
	{
		for (int i = 0; i < _planesShapes.size(); i++)
		{
			_planesShapes[i]->draw();
		}
	}

	for (int i = 0; i < _cornerCube.size(); i++)
	{
		_cornerCube[i]->draw();
	}

	if (_testFrustrum)
	{
		_customCharacter->Draw(_camera->getFrustum()->getPlanes());
	}
	else
	{
		_customCharacter->Draw(_planes, _camera);
	}
}

void Game::destroyGame()
{
	if (_timer) delete _timer;
	
	if (_customCharacter) delete _customCharacter;

	if (_camera) delete _camera;
	if (_lightB) delete _lightB;
	if (_lightA) delete _lightA;
	
	_planes.clear();
	
	for (int i = 0; i < _planesShapes.size(); i++)
	{
		if (_planesShapes[i]) delete _planesShapes[i];
	}
	_planesShapes.clear();

	for (int i = 0; i < _cornerCube.size(); i++)
	{
		if(_cornerCube[i]) delete _cornerCube[i];
	}
	_planesShapes.clear();
}