#include "services/CardMatchService.h"

USING_NS_CC;

bool CardMatchService::isMatch(const CardModel *playfieldCard, const CardModel *baseCard)
{
    if (!playfieldCard || !baseCard)
        return false;

    // A(0)和K(12)可以互相匹配
    if ((playfieldCard->face == CFT_ACE && baseCard->face == CFT_KING) ||
        (playfieldCard->face == CFT_KING && baseCard->face == CFT_ACE))
    {
        return true;
    }

    // 其他情况判断点数是否相差1
    return abs(playfieldCard->face - baseCard->face) == 1;
}

bool CardMatchService::isCardBlocked(int targetCardId, const GameModel *model)
{
    if (!model || !model->allCards.count(targetCardId))
        return true;

    const auto &targetCard = model->allCards.at(targetCardId);
    Rect targetRect(
        targetCard->position.x - kCardLogicWidth / 2,
        targetCard->position.y - kCardLogicHeight / 2,
        kCardLogicWidth,
        kCardLogicHeight);

    for (int otherCardId : model->playfieldCardIds)
    {
        if (otherCardId == targetCardId)
            continue;

        const auto &otherCard = model->allCards.at(otherCardId);

        // 规则：如果一张牌的Y轴更小（更靠下/更靠前），它就有可能挡住目标牌
        if (otherCard->position.y < targetCard->position.y)
        {
            Rect otherRect(
                otherCard->position.x - kCardLogicWidth / 2,
                otherCard->position.y - kCardLogicHeight / 2,
                kCardLogicWidth,
                kCardLogicHeight);

            // 创建一个比实际卡牌更大的“遮挡区域”，特别是向上延伸，以弥补布局中的缝隙。
            // 即使有缝隙，这个更大的区域也能和上面的牌发生重叠。
            Rect blockingRect = otherRect;
            blockingRect.size.height += kCardLogicHeight * 0.5f; // 向上延伸半个卡牌高度

            if (targetRect.intersectsRect(blockingRect))
            {
                return true; // 目标牌的区域与下方牌的“遮挡区域”重叠，判定为被遮挡
            }
        }
    }

    return false; // 遍历完所有牌都没有发现遮挡，则判定为未被遮挡
}
