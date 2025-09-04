#ifndef __UNDO_MANAGER_H__
#define __UNDO_MANAGER_H__

#include <vector>
#include "models/UndoModel.h"
#include "cocos2d.h"

/**
 * @class UndoManager
 * @brief 管理游戏的回退操作栈。
 * @details 作为Controller的成员，持有回退记录(UndoRecord)的栈，
 *          并提供记录和提取操作的接口，用于实现游戏的回退功能。
 *          此类不应被设计为单例。
 */
class UndoManager
{
public:
    UndoManager() = default;

    /**
     * @brief 记录一次从主牌区的匹配操作。
     * @param cardToMoveId 被移动的牌的ID。
     * @param originalPos 牌在主牌区的原始位置。
     * @param oldBaseCardId 被替换掉的旧底牌ID。
     */
    void recordMatchAction(int cardToMoveId, const cocos2d::Vec2& originalPos, int oldBaseCardId);

    /**
     * @brief 记录一次从备用牌区的交换操作。
     * @param cardToMoveId 被移动的牌的ID。
     * @param oldBaseCardId 被替换掉的旧底牌ID。
     */
    void recordSwapAction(int cardToMoveId, int oldBaseCardId);

    /**
     * @brief 记录一次从备用牌堆的抽牌操作。(当前未使用)
     * @param newBaseCardId 新抽出的底牌ID。
     * @param oldBaseCardId 被替换掉的旧底牌ID。
     */
    void recordDrawAction(int newBaseCardId, int oldBaseCardId);

    /**
     * @brief 弹出最后一次操作记录。
     * @param outRecord [out] 用于接收操作记录的指针。
     * @return 如果栈不为空，返回true，并将记录写入outRecord；否则返回false。
     */
    bool popLastAction(UndoRecord* outRecord);

    /**
     * @brief 清空所有回退记录，通常用于新开一局游戏。
     */
    void clear();

private:
    // 存储所有可回退操作的栈
    std::vector<UndoRecord> _undoStack;
};

#endif // __UNDO_MANAGER_H__