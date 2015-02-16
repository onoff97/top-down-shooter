#include "GameState.h"
#include "Camera2d.h"
#include "Player.h"
#include "AppDelegate.h"

USING_NS_CC;

Scene* GameState::createScene()
{
	auto scene = Scene::create();
	auto layer = GameState::create();
	scene->addChild(layer);
	return scene;
}

bool GameState::init()
{
	if (!Layer::init())
	{
		return false;
	}

	// Load map from .tmx
	map = TMXTiledMap::create("Map2.tmx");
	mapHitboxes = map->getObjectGroup("Walls");

	camera = Node::create();
	camera->setPosition(Vec2(0, 0));
	camera->addChild(map);

	// Create player
	player = new Player(this, 200, 250);
	camera->addChild(player->baseNode);
	
	// Add camera node to layer
	addChild(camera);

	// Add input handling
	auto keyboard = EventListenerKeyboard::create();
	keyboard->onKeyPressed = CC_CALLBACK_2(GameState::onKeyPressed, this);
	keyboard->onKeyReleased = CC_CALLBACK_2(GameState::onKeyReleased, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboard, this);

	auto mouse = EventListenerMouse::create();
	mouse->onMouseDown = CC_CALLBACK_1(GameState::onMouseDown, this);
	mouse->onMouseMove = CC_CALLBACK_1(GameState::onMouseMove, this);
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(mouse, this);

	// Add update method to schedule with some interval in seconds
	this->schedule(schedule_selector(GameState::update), 1.0f / 60);

	return true;
}

void GameState::update(float dt)
{
	player->update(dt);
	// Camera origin is lower left, 5.0f came through testing
	camera->setPositionX(-(player->x + player->distX / 5.0f - 960 / 2));
	camera->setPositionY(-(player->y - player->distY / 5.0f - 640 / 2));
}

void GameState::addEntity(Entity *entity)
{
	entities.push_back(entity);
}

void GameState::onKeyPressed(EventKeyboard::KeyCode keyCode, Event *e)
{
	// Add keycode to list if it doesn't already contain it
	if (!isKeyDown(keyCode))
		keysDown.push_back(keyCode);
}

void GameState::onKeyReleased(EventKeyboard::KeyCode keyCode, Event *e)
{
	// Remove keycode from list
	keysDown.erase(std::remove(keysDown.begin(), keysDown.end(), keyCode), keysDown.end());
}

void GameState::onMouseDown(Event *e)
{

}

void GameState::onMouseMove(Event *e)
{
	Size window = Director::getInstance()->getWinSizeInPixels();
	EventMouse *mouseEvent = dynamic_cast<EventMouse*>(e);
	mouseX = mouseEvent->getCursorX();
	mouseY = mouseEvent->getCursorY() + window.height;
}

bool GameState::isKeyDown(EventKeyboard::KeyCode keyCode)
{
	return std::find(keysDown.begin(), keysDown.end(), keyCode) != keysDown.end();
}