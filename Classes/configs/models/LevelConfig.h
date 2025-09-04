#ifndef __LEVEL_CONFIG_H__
#define __LEVEL_CONFIG_H__

#include <vector>
#include <string>
#include "cocos2d.h"

/**
 * @enum CardSuitType
 * @brief 定义卡牌的四种花色类型。
 */
enum CardSuitType
{
    CST_NONE = -1, // 无效或未定义的花色
    CST_CLUBS,     // 梅花
    CST_DIAMONDS,  // 方块
    CST_HEARTS,    // 红桃
    CST_SPADES,    // 黑桃
    CST_NUM_CARD_SUIT_TYPES
};

/**
 * @enum CardFaceType
 * @brief 定义卡牌的点数类型，从A到K。
 */
enum CardFaceType
{
    CFT_NONE = -1, // 无效或未定义的点数
    CFT_ACE,       // A (值为0)
    CFT_TWO,       // 2
    CFT_THREE,     // 3
    CFT_FOUR,      // 4
    CFT_FIVE,      // 5
    CFT_SIX,       // 6
    CFT_SEVEN,     // 7
    CFT_EIGHT,     // 8
    CFT_NINE,      // 9
    CFT_TEN,       // 10
    CFT_JACK,      // J
    CFT_QUEEN,     // Q
    CFT_KING,      // K (值为12)
    CFT_NUM_CARD_FACE_TYPES
};

/**
 * @struct CardConfigData
 * @brief 单张卡牌的配置数据结构。
 * @details 用于从JSON配置文件中加载并临时存储一张卡牌的静态布局信息。
 */
struct CardConfigData
{
    int cardId;             // 卡牌唯一标识符
    CardFaceType face;      // 卡牌点数
    CardSuitType suit;      // 卡牌花色
    cocos2d::Vec2 position; // 卡牌在游戏中的初始位置坐标
    bool isFaceUp;          // 卡牌初始时是否正面朝上显示
};

/**
 * @class LevelConfig
 * @brief 关卡配置数据模型类。
 * @details 存储从配置文件加载的单个关卡的所有静态数据，
 *          包括主牌区和备用牌堆的卡牌布局信息。
 *          该对象在生成GameModel后即可销毁。
 */
class LevelConfig
{
public:
    // 主牌区所有卡牌的初始配置列表
    std::vector<CardConfigData> playfieldCards;
    // 备用牌堆（逻辑上的抽牌堆）所有卡牌的配置列表
    std::vector<CardConfigData> stackCards;
};

#endif // __LEVEL_CONFIG_H__