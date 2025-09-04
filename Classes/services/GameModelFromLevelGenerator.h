#ifndef __GAME_MODEL_GENERATOR_H__
#define __GAME_MODEL_GENERATOR_H__

#include "models/GameModel.h"
#include "configs/models/LevelConfig.h"

/**
 * @class GameModelFromLevelGenerator
 * @brief 根据关卡配置生成游戏数据模型。
 * @details 这是一个静态服务类，负责将静态的LevelConfig数据转换成
 *          游戏运行时所需的动态GameModel。它处理了卡牌对象的创建和
 *          初始牌堆的分配逻辑。
 */
class GameModelFromLevelGenerator
{
public:
    /**
     * @brief 根据指定的关卡配置生成一个完整的GameModel。
     * @param config 关卡配置数据。
     * @return 成功则返回新创建的GameModel实例，调用者需负责释放内存；
     *         如果config为nullptr，则返回nullptr。
     */
    static GameModel* generateGameModel(const LevelConfig* config);

private:
    /**
     * @brief 从配置数据创建卡牌模型。
     * @param gameModel 目标游戏模型。
     * @param cardConfigs 卡牌配置列表。
     * @param targetPile 目标牌堆的ID列表。
     * @param isFaceUp 卡牌是否正面朝上。
     * @param positionOffset 卡牌位置的Y轴偏移量。
     */
    static void _createCardsFromConfig(GameModel* gameModel, const std::vector<CardConfigData>& cardConfigs, std::vector<int>& targetPile, bool isFaceUp, float positionOffsetY);

    /**
     * @brief 初始化底牌堆和备用牌堆。
     * @param gameModel 游戏模型。
     */
    static void _initializePiles(GameModel* gameModel);

    // 定义底部UI区域的高度作为游戏区卡牌的Y轴偏移量
    static constexpr float kPlayfieldOffsetY = 580.0f;
};

#endif // __GAME_MODEL_GENERATOR_H__