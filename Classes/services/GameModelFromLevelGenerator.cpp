#include "services/GameModelFromLevelGenerator.h"

USING_NS_CC;

GameModel *GameModelFromLevelGenerator::generateGameModel(const LevelConfig *config)
{
    if (!config)
        return nullptr;

    GameModel *gameModel = new GameModel();

    // 创建主牌区卡牌
    _createCardsFromConfig(gameModel, config->playfieldCards, gameModel->playfieldCardIds, true, kPlayfieldOffsetY);

    // 创建抽牌堆卡牌
    _createCardsFromConfig(gameModel, config->stackCards, gameModel->drawPileCardIds, false, 0);

    // 初始化底牌堆和备用牌堆
    _initializePiles(gameModel);

    return gameModel;
}

void GameModelFromLevelGenerator::_createCardsFromConfig(GameModel *gameModel, const std::vector<CardConfigData> &cardConfigs, std::vector<int> &targetPile, bool isFaceUp, float positionOffsetY)
{
    for (const auto &cardConfig : cardConfigs)
    {
        CardModel *card = new CardModel();
        card->id = cardConfig.cardId;
        card->face = cardConfig.face;
        card->suit = cardConfig.suit;
        card->position = cardConfig.position + Vec2(0, positionOffsetY);
        card->isFaceUp = isFaceUp;

        gameModel->allCards[card->id] = card;
        targetPile.push_back(card->id);
    }
}

void GameModelFromLevelGenerator::_initializePiles(GameModel *gameModel)
{
    // 从抽牌堆发三张牌到UI区
    if (gameModel->drawPileCardIds.size() >= 3)
    {
        // 1. 发第一张作为初始底牌
        gameModel->baseCardId = gameModel->drawPileCardIds.back();
        gameModel->drawPileCardIds.pop_back();
        gameModel->allCards[gameModel->baseCardId]->isFaceUp = true;

        // 2. 发两张到备用牌区
        int stockpileTop = gameModel->drawPileCardIds.back();
        gameModel->drawPileCardIds.pop_back();
        gameModel->allCards[stockpileTop]->isFaceUp = true;

        int stockpileBottom = gameModel->drawPileCardIds.back();
        gameModel->drawPileCardIds.pop_back();
        gameModel->allCards[stockpileBottom]->isFaceUp = true;

        gameModel->stockpileCardIds.push_back(stockpileBottom);
        gameModel->stockpileCardIds.push_back(stockpileTop);
    }
    // 如果不够三张，但至少有一张，则只发底牌
    else if (!gameModel->drawPileCardIds.empty())
    {
        gameModel->baseCardId = gameModel->drawPileCardIds.back();
        gameModel->drawPileCardIds.pop_back();
        gameModel->allCards[gameModel->baseCardId]->isFaceUp = true;
    }
}