#ifndef _APP_DELEGATE_H_
#define _APP_DELEGATE_H_

#include "cocos2d.h"

/**
 * @class AppDelegate
 * @brief Cocos2d-x应用程序的代理类。
 * @details 这是应用程序的入口点，负责初始化Director、OpenGL视图（GLView）
 *          以及启动第一个场景。它还处理应用程序进入后台和返回前台的生命周期事件。
 */
class AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    /**
     * @brief 初始化OpenGL上下文属性。
     */
    virtual void initGLContextAttrs();

    /**
     * @brief 实现Director和Scene的初始化代码。
     * @return true 初始化成功，应用继续运行。
     * @return false 初始化失败，应用终止。
     */
    virtual bool applicationDidFinishLaunching();

    /**
     * @brief 当应用程序进入后台时被调用。
     * @param application 应用程序实例指针。
     */
    virtual void applicationDidEnterBackground();

    /**
     * @brief 当应用程序返回前台时被调用。
     * @param application 应用程序实例指针。
     */
    virtual void applicationWillEnterForeground();
};

#endif // _APP_DELEGATE_H_