#ifndef __CARD_VIEW_H__
#define __CARD_VIEW_H__

#include "cocos2d.h"
#include <functional>
#include "models/CardModel.h"

/**
 * @class CardView
 * @brief 负责单张卡牌的显示和用户输入。
 * @details 继承自cocos2d::Node，作为一个容器，组合了底板、花色、数字等精灵。
 *          它根据传入的CardModel来构建视图，并能响应用户的触摸事件，
 *          通过回调函数通知Controller。
 */
class CardView : public cocos2d::Node
{
public:
    /**
     * @brief 静态工厂方法，根据数据模型创建卡牌视图。
     * @param model 包含卡牌信息的const指针。
     * @return 成功则返回自动释放的CardView实例，否则返回nullptr。
     */
    static CardView* create(const CardModel* model);

    /**
     * @brief 初始化方法，根据数据模型构建卡牌的各个视觉元素。
     * @param model 包含卡牌信息的const指针。
     * @return 初始化成功返回true，否则返回false。
     */
    virtual bool init(const CardModel* model);

    /**
     * @brief 设置卡牌被点击时的回调函数。
     * @param callback 回调函数，参数为本张卡牌的ID。
     */
    void setCardClickCallback(const std::function<void(int)>& callback);

    /**
     * @brief 获取本张卡牌的ID。
     * @return 卡牌ID。
     */
    int getCardId() const { return _cardId; }

private:
    /**
     * @brief 创建并设置卡牌的底板精灵。
     */
    void _setupBaseSprite();

    /**
     * @brief 根据模型数据创建并布置卡牌上的所有元素（数字、花色）。
     * @param model 卡牌数据模型。
     */
    void _setupElements(const CardModel* model);

    /**
     * @brief 初始化并注册触摸事件监听器。
     */
    void _setupTouchListener();

    /**
     * @brief 触摸事件的回调函数。
     * @param touch 触摸对象。
     * @param event 事件对象。
     * @return 如果触摸点在本卡牌内，则处理事件并返回true，否则返回false。
     */
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

    /**
     * @brief 根据花色类型获取对应的资源路径。
     * @param suit 卡牌花色。
     * @return 对应的图片文件路径。
     */
    std::string _getSuitTexturePath(CardSuitType suit);

    /**
     * @brief 根据点数、花色和尺寸获取对应的资源路径。
     * @param face 卡牌点数。
     * @param suit 卡牌花色（用于决定颜色）。
     * @param sizePrefix 资源尺寸前缀 (例如 "big", "small")。
     * @return 对应的图片文件路径。
     */
    std::string _getNumberTexturePath(CardFaceType face, CardSuitType suit, const std::string& sizePrefix);

    int _cardId; // 本卡牌的唯一ID
    std::function<void(int)> _onCardClickCallback; // 点击事件回调
};

#endif // __CARD_VIEW_H__