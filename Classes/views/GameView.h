#ifndef __GAME_VIEW_H__
#define __GAME_VIEW_H__

#include "cocos2d.h"
#include <map>
#include <functional>
#include "models/GameModel.h"
#include "views/CardView.h"

/**
 * @class GameView
 * @brief 游戏主视图，负责整个游戏界面的显示和动画。
 * @details 作为Node的子类，它管理所有CardView实例，并提供接口来播放
 *          卡牌移动、匹配和回退的动画。它通过回调与Controller进行交互，
 *          并持有const类型的Model指针以获取显示所需的数据。
 */
class GameView : public cocos2d::Node
{
public:
    /**
     * @brief 创建GameView实例的静态工厂方法。
     * @param gameModel const类型的游戏数据模型指针。
     * @return 成功则返回自动释放的GameView实例，否则返回nullptr。
     */
    static GameView* create(const GameModel* gameModel);

    /**
     * @brief 初始化方法。
     * @param gameModel const类型的游戏数据模型指针。
     * @return 初始化成功返回true，否则返回false。
     */
    virtual bool init(const GameModel* gameModel);

    /**
     * @brief 当节点进入活动场景时调用，用于首次更新卡牌位置。
     */
    virtual void onEnter() override;

    // --- 回调注册 ---
    /**
     * @brief 注册卡牌被点击时的回调函数。
     * @param callback 回调函数，参数为被点击的卡牌ID。
     */
    void setCardClickCallback(const std::function<void(int)>& callback);

    /**
     * @brief 注册回退按钮被点击时的回调函数。
     * @param callback 无参数的回调函数。
     */
    void setUndoClickCallback(const std::function<void()>& callback);

    // --- 动画接口 ---
    /**
     * @brief 播放卡牌匹配成功的动画。
     * @param movedCardId 移动到废牌堆的卡牌ID。
     * @param oldBaseCardId 被覆盖的旧底牌ID。
     * @param onComplete 动画完成后的回调函数。
     */
    void playMatchAnimation(int movedCardId, int oldBaseCardId, const std::function<void()>& onComplete);

    /**
     * @brief 播放回退“匹配”操作的动画。
     * @param cardToMoveBackId 从底牌堆移回主牌区的卡牌ID。
     * @param cardToRestoreId 从废牌堆恢复到底牌堆的卡牌ID。
     * @param targetPos 卡牌移回主牌区的目标位置。
     * @param onComplete 动画完成后的回调函数。
     */
    void playUndoAnimation(int cardToMoveBackId, int cardToRestoreId, const cocos2d::Vec2& targetPos, const std::function<void()>& onComplete);

    /**
     * @brief 播放回退“交换”操作的动画。
     * @param cardToMoveBackId 从底牌堆移回备用牌区的卡牌ID。
     * @param cardToRestoreId 从废牌堆恢复到底牌堆的卡牌ID。
     * @param onComplete 动画完成后的回调函数。
     */
    void playUndoSwapAnimation(int cardToMoveBackId, int cardToRestoreId, const std::function<void()>& onComplete);

    // --- 视图更新接口 ---
    /**
     * @brief 根据Model的数据，重新计算并设置所有卡牌的位置和可见性。
     */
    void updateCardPositions();

    /**
     * @brief 查询当前是否有动画正在播放。
     * @return 如果有动画在播放，返回true。
     */
    bool isAnimationRunning() const { return _animationRunning; }

private:
    /**
     * @brief 初始化UI元素，如回退按钮。
     */
    void _initUI();

    /**
     * @brief 根据模型数据更新主牌区卡牌的视图状态。
     */
    void _updatePlayfieldCards();

    /**
     * @brief 根据模型数据更新备用牌区卡牌的视图状态。
     */
    void _updateStockpileCards();

    /**
     * @brief 根据模型数据更新废牌堆和底牌的视图状态。
     */
    void _updateWasteAndBaseCards();

    /**
     * @brief 播放回退动画的通用实现。
     * @param cardToMoveBackId 要移回的牌ID。
     * @param cardToRestoreId 要恢复的旧底牌ID。
     * @param moveBackTargetPos 移回牌的目标位置。
     * @param onComplete 动画完成回调。
     */
    void _playUndoAnimation(int cardToMoveBackId, int cardToRestoreId, const cocos2d::Vec2& moveBackTargetPos, const std::function<void()>& onComplete);

    /**
     * @brief 设置动画是否正在运行的标志。
     * @param running 动画状态。
     */
    void _setAnimationRunning(bool running);

    // --- 成员变量 ---
    const GameModel* _gameModel; // 数据模型（只读）
    std::map<int, CardView*> _cardViews; // 管理所有卡牌视图
    bool _animationRunning = false; // 动画播放状态锁

    std::function<void(int)> _onCardClickCallback; // 卡牌点击回调
    std::function<void()> _undoClickCallback;      // 回退点击回调

    // --- UI常量 ---
    const cocos2d::Vec2 kBaseCardPos = cocos2d::Vec2(620, 290); // 底牌堆位置 (最右)
    const cocos2d::Vec2 kStockpilePos = cocos2d::Vec2(220, 290);  // 备用牌堆起始位置 (最左)
    const float kStockpileSpacing = 130.0f;                     // 备用牌堆的牌间距，小于卡牌宽度以实现重叠
};

#endif // __GAME_VIEW_H__