#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

bool HelloWorld::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 1. 创建棕色游戏区背景
    auto playfieldBg = LayerColor::create(Color4B(193, 154, 107, 255));
    this->addChild(playfieldBg, -2); // Z-Order为-2，放在最底层

    // 2. 创建紫色UI区背景
    auto uiPanel = LayerColor::create(Color4B(128, 90, 140, 255), visibleSize.width, 580); // UI区域高度为580
    uiPanel->setPosition(origin);
    this->addChild(uiPanel, -1); // Z-Order为-1, 在游戏区背景之上

    // 3. 创建并启动游戏控制器
    _gameController = new GameController(this);
    _gameController->startGame(1); // 启动第一关

    return true;
}