#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "controllers/GameController.h"

/**
 * @class HelloWorld
 * @brief 游戏的主场景。
 * @details 作为游戏的入口场景，它的主要职责是创建背景、UI面板，
 *          并初始化和持有游戏主控制器(GameController)。
 */
class HelloWorld : public cocos2d::Scene
{
public:
    /**
     * @brief 静态工厂方法，用于创建场景实例。
     * @return 自动释放的场景实例。
     */
    static cocos2d::Scene *createScene();

    /**
     * @brief 场景的初始化方法。
     * @return 初始化成功返回true，否则返回false。
     */
    virtual bool init();

    // Cocos2d-x宏，用于快速实现create方法
    CREATE_FUNC(HelloWorld);

private:
    // 游戏主控制器实例
    GameController *_gameController;
};

#endif // __HELLOWORLD_SCENE_H__