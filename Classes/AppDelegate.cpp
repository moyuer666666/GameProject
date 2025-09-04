#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;

// 设计分辨率
static cocos2d::Size designResolutionSize = cocos2d::Size(1080, 2080);
// 适配用的多套资源尺寸（此项目中未实际使用多套资源，但保留适配逻辑）
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate() {}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};
    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages()
{
    return 0; // for packages manager
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // 初始化Director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview)
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("GameProject", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height), 0.4);
#else
        glview = GLViewImpl::create("GameProject");
#endif
        director->setOpenGLView(glview);
    }

    director->setDisplayStats(false);          // 关闭左下角FPS显示
    director->setAnimationInterval(1.0f / 60); // 设置帧率

    // 设置设计分辨率和适配策略
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);

    register_all_packages();

    // 创建并运行主场景
    auto scene = HelloWorld::createScene();
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();
}