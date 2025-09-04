#ifndef __UNDO_MODEL_H__
#define __UNDO_MODEL_H__

#include "cocos2d.h"

/**
 * @enum UndoActionType
 * @brief 定义可回退操作的类型。
 */
enum UndoActionType
{
    MATCH_FROM_PLAYFIELD, // 从主牌区匹配是一次操作
    SWAP_FROM_STOCKPILE,  // 从备用牌区换牌是一次操作
    DRAW_FROM_STOCK       // 从备用牌堆抽牌 (此功能当前未使用)
};

/**
 * @struct UndoRecord
 * @brief 存储一次可回退操作的完整信息。
 * @details 用于UndoManager记录玩家的每一步有效操作，以便能够精确地恢复到上一个状态。
 *          该结构将在游戏存档时被序列化。
 */
struct UndoRecord
{
    UndoActionType type;            // 本次操作的类型
    int movedCardId;                // 被移动的牌的ID
    cocos2d::Vec2 originalPosition; // 移动牌的原始位置 (仅对主牌区牌有效)
    int previousBaseCardId;         // 本次操作前的底牌ID，用于恢复
};

#endif // __UNDO_MODEL_H__