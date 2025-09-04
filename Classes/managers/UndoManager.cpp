#include "managers/UndoManager.h"

void UndoManager::recordMatchAction(int cardToMoveId, const cocos2d::Vec2 &originalPos, int oldBaseCardId)
{
    UndoRecord record;
    record.type = MATCH_FROM_PLAYFIELD;
    record.movedCardId = cardToMoveId;
    record.originalPosition = originalPos;
    record.previousBaseCardId = oldBaseCardId;
    _undoStack.push_back(record);
}

void UndoManager::recordSwapAction(int cardToMoveId, int oldBaseCardId)
{
    UndoRecord record;
    record.type = SWAP_FROM_STOCKPILE;
    record.movedCardId = cardToMoveId;
    record.previousBaseCardId = oldBaseCardId;
    record.originalPosition = cocos2d::Vec2::ZERO; // 备用牌区的位置是动态的，无需记录
    _undoStack.push_back(record);
}

void UndoManager::recordDrawAction(int newBaseCardId, int oldBaseCardId)
{
    UndoRecord record;
    record.type = DRAW_FROM_STOCK;
    record.movedCardId = newBaseCardId;
    record.previousBaseCardId = oldBaseCardId;
    record.originalPosition = cocos2d::Vec2(200, 300); // 示例位置
    _undoStack.push_back(record);
}

bool UndoManager::popLastAction(UndoRecord *outRecord)
{
    if (_undoStack.empty() || !outRecord)
    {
        return false;
    }
    *outRecord = _undoStack.back();
    _undoStack.pop_back();
    return true;
}

void UndoManager::clear()
{
    _undoStack.clear();
}