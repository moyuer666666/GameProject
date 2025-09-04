#ifndef __GAME_MODEL_H__
#define __GAME_MODEL_H__

#include <vector>
#include <map>
#include "models/CardModel.h"

/**
 * @class GameModel
 * @brief 存储整个游戏运行时的核心数据。
 * @details 这是游戏状态的核心，包含了所有卡牌的实时数据和各个牌堆的状态。
 *          它不包含任何业务逻辑，只负责存储数据。
 *          整个对象将在游戏暂停或退出时被序列化以支持存档恢复功能。
 */
class GameModel
{
public:
    /**
     * @brief 析构函数，负责清理所有动态分配的CardModel对象。
     */
    ~GameModel()
    {
        for (auto const& pair : allCards)
        {
            delete pair.second;
        }
        allCards.clear();
    }

    // 使用ID快速查找卡牌，存储游戏中所有卡牌的实例
    std::map<int, CardModel*> allCards;

    // 主牌区当前存在的牌的ID列表
    std::vector<int> playfieldCardIds;

    // 抽牌堆的牌的ID列表 (逻辑存在，不可见，栈顶在vector的末尾)
    std::vector<int> drawPileCardIds;

    // 备用牌堆的牌的ID列表 (左下角可见的牌堆，最右侧的牌在vector的末尾)
    std::vector<int> stockpileCardIds;

    // 废牌堆的ID列表, 存放被覆盖的旧底牌 (栈顶在vector的末尾)
    std::vector<int> wastePileCardIds;

    // 底牌堆当前的牌的ID（当前唯一用于匹配的牌）
    int baseCardId = -1;
};

#endif // __GAME_MODEL_H__