#ifndef __CARD_MATCH_SERVICE_H__
#define __CARD_MATCH_SERVICE_H__

#include "models/GameModel.h"
#include "cocos2d.h"

/**
 * @class CardMatchService
 * @brief 提供卡牌匹配和游戏逻辑相关的无状态服务。
 * @details 这是一个静态工具类，负责处理游戏的核心规则判断，如两张牌是否匹配、
 *          某张牌是否被遮挡等。它自身不持有任何数据，仅对传入的数据进行加工和判断。
 */
class CardMatchService
{
public:
    // 定义卡牌在游戏逻辑中的尺寸，用于遮挡判断
    // 这些值应与CardView的资源和缩放匹配 (资源约为200x280, 缩放0.7)
    static constexpr float kCardLogicWidth = 200.0f * 0.7f;
    static constexpr float kCardLogicHeight = 280.0f * 0.7f;

    /**
     * @brief 判断桌面牌是否能和底牌匹配。
     * @param playfieldCard 要匹配的桌面牌。
     * @param baseCard 当前的底牌。
     * @return 如果点数相邻或为A和K，返回true；否则返回false。
     */
    static bool isMatch(const CardModel* playfieldCard, const CardModel* baseCard);

    /**
     * @brief 判断一张主牌区的牌是否被其他牌遮挡。
     * @details 如果有任何其他牌的Y坐标比它小（更靠下/更靠前），并且其扩大的“遮挡区域”
     *          与目标牌重叠，则认为它被遮挡。
     * @param targetCardId 要检查的牌的ID。
     * @param model 包含所有牌信息的游戏模型。
     * @return 如果被遮挡，返回true。
     */
    static bool isCardBlocked(int targetCardId, const GameModel* model);
};

#endif // __CARD_MATCH_SERVICE_H__