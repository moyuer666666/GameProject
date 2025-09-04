#ifndef __CARD_MODEL_H__
#define __CARD_MODEL_H__

#include "configs/models/LevelConfig.h" // 复用枚举
#include "cocos2d.h"

/**
 * @class CardModel
 * @brief 存储单张卡牌的运行时数据。
 * @details 该模型包含了卡牌的所有动态属性，如位置、是否正面朝上等。
 *          它将在游戏存档时被序列化。
 */
class CardModel
{
public:
    int id;                 // 卡牌的唯一标识符
    CardFaceType face;      // 卡牌的点数 (A, 2, K, etc.)
    CardSuitType suit;      // 卡牌的花色 (红桃, 黑桃, etc.)
    cocos2d::Vec2 position; // 卡牌在主游戏区的当前位置
    bool isFaceUp;          // 卡牌当前是否正面朝上
};

#endif // __CARD_MODEL_H__