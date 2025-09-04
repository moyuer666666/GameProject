#ifndef __GAME_CONTROLLER_H__
#define __GAME_CONTROLLER_H__

#include "cocos2d.h"
#include "models/GameModel.h"
#include "views/GameView.h"
#include "managers/UndoManager.h"

/**
 * @class GameController
 * @brief 游戏主控制器，协调所有模块，处理核心业务逻辑。
 * @details 负责接收View层的用户输入回调，根据游戏规则更新Model层的数据，
 *          并通知View层播放动画和更新显示。它持有Model, View和Manager的实例。
 */
class GameController
{
public:
    /**
     * @brief 构造函数。
     * @param rootNode 游戏视图将要添加到的父节点，通常是场景(Scene)。
     */
    GameController(cocos2d::Node* rootNode);

    /**
     * @brief 析构函数，负责清理模型和管理器。
     */
    ~GameController();

    /**
     * @brief 基于指定的关卡ID，开始一局新游戏。
     * @param levelId 关卡ID。
     */
    void startGame(int levelId);

private:
    /**
     * @brief 处理来自View的卡牌点击事件回调。
     * @param cardId 被点击的卡牌的ID。
     */
    void _handleCardClick(int cardId);

    /**
     * @brief 处理来自View的回退按钮点击事件回调。
     */
    void _handleUndoClick();

    /**
     * @brief 处理对主游戏区卡牌的点击逻辑。
     * @param cardId 被点击的卡牌ID。
     * @return 如果事件被处理，返回true。
     */
    bool _handlePlayfieldCardClick(int cardId);

    /**
     * @brief 处理对备用牌区卡牌的点击逻辑。
     * @param cardId 被点击的卡牌ID。
     * @return 如果事件被处理，返回true。
     */
    bool _handleStockpileCardClick(int cardId);

    /**
     * @brief 执行“从主牌区匹配”的回退操作。
     * @param record 包含回退信息的操作记录。
     */
    void _undoMatchFromPlayfield(const UndoRecord& record);

    /**
     * @brief 执行“从备用牌区交换”的回退操作。
     * @param record 包含回退信息的操作记录。
     */
    void _undoSwapFromStockpile(const UndoRecord& record);

    cocos2d::Node* _rootNode;   // 视图的根节点
    GameModel* _gameModel;      // 游戏数据模型
    GameView* _gameView;        // 游戏主视图
    UndoManager* _undoManager;  // 回退操作管理器
};

#endif // __GAME_CONTROLLER_H__