#include "GameView.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

// 定义全局动画时间和卡牌缩放比例
const float kAnimationDuration = 0.3f;
const float kCardScale = 0.9f;

GameView *GameView::create(const GameModel *gameModel)
{
    auto pRet = new (std::nothrow) GameView();
    if (pRet && pRet->init(gameModel))
    {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    return nullptr;
}

bool GameView::init(const GameModel *gameModel)
{
    if (!Node::init())
        return false;
    _gameModel = gameModel;

    // 根据模型数据创建所有卡牌的视图
    for (const auto &pair : _gameModel->allCards)
    {
        auto cardView = CardView::create(pair.second);
        cardView->setScale(kCardScale);
        this->addChild(cardView);
        _cardViews[pair.first] = cardView;
    }

    _initUI();

    return true;
}

void GameView::_initUI()
{
    auto undoButton = ui::Button::create();
    undoButton->setTitleText("回退");
    undoButton->setTitleFontSize(48);
    undoButton->setTitleColor(Color3B::WHITE);
    undoButton->setPosition(Vec2(880, 290));
    undoButton->addClickEventListener([this](Ref *)
                                      {
        if (_undoClickCallback) _undoClickCallback(); });
    this->addChild(undoButton);
}

void GameView::onEnter()
{
    Node::onEnter();
    updateCardPositions();
}

void GameView::updateCardPositions()
{
    // 先隐藏所有牌，再根据牌堆状态决定显示哪些
    for (const auto &pair : _cardViews)
    {
        pair.second->setVisible(false);
    }

    _updatePlayfieldCards();
    _updateStockpileCards();
    _updateWasteAndBaseCards();
}

void GameView::_updatePlayfieldCards()
{
    for (int cardId : _gameModel->playfieldCardIds)
    {
        if (_cardViews.count(cardId))
        {
            auto cardView = _cardViews[cardId];
            cardView->setPosition(_gameModel->allCards.at(cardId)->position);
            // Y坐标越小（越靠下）的牌，Z值应越高（显示在前面）
            cardView->setLocalZOrder(2080 - cardView->getPosition().y);
            cardView->setVisible(true);
        }
    }
}

void GameView::_updateStockpileCards()
{
    for (size_t i = 0; i < _gameModel->stockpileCardIds.size(); ++i)
    {
        int cardId = _gameModel->stockpileCardIds[i];
        if (_cardViews.count(cardId))
        {
            auto cardView = _cardViews[cardId];
            cardView->setPosition(kStockpilePos + Vec2(i * kStockpileSpacing, 0));
            cardView->setLocalZOrder(10 + i);
            cardView->setVisible(true);
        }
    }
}

void GameView::_updateWasteAndBaseCards()
{
    // 布局废牌堆：将所有废牌都放在底牌位置，但Z值较低，确保被当前底牌盖住
    for (size_t i = 0; i < _gameModel->wastePileCardIds.size(); ++i)
    {
        int cardId = _gameModel->wastePileCardIds[i];
        if (_cardViews.count(cardId))
        {
            auto cardView = _cardViews[cardId];
            cardView->setPosition(kBaseCardPos);
            cardView->setLocalZOrder(50 + i); // Z值递增，但低于当前底牌
            cardView->setVisible(true);
        }
    }

    // 布局当前底牌
    if (_gameModel->baseCardId != -1 && _cardViews.count(_gameModel->baseCardId))
    {
        auto cardView = _cardViews[_gameModel->baseCardId];
        cardView->setPosition(kBaseCardPos);
        cardView->setLocalZOrder(100); // 确保当前底牌在最上面
        cardView->setVisible(true);
    }
}

void GameView::playMatchAnimation(int movedCardId, int oldBaseCardId, const std::function<void()> &onComplete)
{
    if (!_cardViews.count(movedCardId) || !_cardViews.count(oldBaseCardId))
        return;

    _setAnimationRunning(true);
    auto cardToMove = _cardViews[movedCardId];
    auto oldBaseCardView = _cardViews[oldBaseCardId];

    cardToMove->setLocalZOrder(200);     // 移动的牌Z值最高，飞到最前面
    oldBaseCardView->setLocalZOrder(99); // 旧底牌Z值降低，准备被覆盖

    auto actions = Sequence::create(
        MoveTo::create(kAnimationDuration, kBaseCardPos),
        CallFunc::create(onComplete),
        CallFunc::create([this]()
                         { _setAnimationRunning(false); }),
        nullptr);
    cardToMove->runAction(actions);
}

void GameView::playUndoAnimation(int cardToMoveBackId, int cardToRestoreId, const Vec2 &targetPos, const std::function<void()> &onComplete)
{
    _playUndoAnimation(cardToMoveBackId, cardToRestoreId, targetPos, onComplete);
}

void GameView::playUndoSwapAnimation(int cardToMoveBackId, int cardToRestoreId, const std::function<void()> &onComplete)
{
    // 计算备用牌堆的下一个位置
    Vec2 targetPos = kStockpilePos + Vec2((_gameModel->stockpileCardIds.size() - 1) * kStockpileSpacing, 0);
    _playUndoAnimation(cardToMoveBackId, cardToRestoreId, targetPos, onComplete);
}

void GameView::_playUndoAnimation(int cardToMoveBackId, int cardToRestoreId, const Vec2 &moveBackTargetPos, const std::function<void()> &onComplete)
{
    if (!_cardViews.count(cardToMoveBackId) || !_cardViews.count(cardToRestoreId))
        return;

    _setAnimationRunning(true);
    auto cardToMoveBack = _cardViews[cardToMoveBackId];
    auto cardToRestore = _cardViews[cardToRestoreId];

    cardToRestore->setVisible(true); // 恢复的牌可能之前被隐藏
    cardToMoveBack->setLocalZOrder(200);
    cardToRestore->setLocalZOrder(199);

    // 移回的牌飞向目标位置
    cardToMoveBack->runAction(MoveTo::create(kAnimationDuration, moveBackTargetPos));

    // 恢复的牌飞回底牌位置，动画结束后调用回调
    auto restoreActions = Sequence::create(
        MoveTo::create(kAnimationDuration, kBaseCardPos),
        CallFunc::create(onComplete),
        CallFunc::create([this]()
                         { _setAnimationRunning(false); }),
        nullptr);
    cardToRestore->runAction(restoreActions);
}

void GameView::setCardClickCallback(const std::function<void(int)> &callback)
{
    _onCardClickCallback = callback;
    for (auto const &pair : _cardViews)
    {
        pair.second->setCardClickCallback(_onCardClickCallback);
    }
}

void GameView::setUndoClickCallback(const std::function<void()> &callback)
{
    _undoClickCallback = callback;
}

void GameView::_setAnimationRunning(bool running)
{
    _animationRunning = running;
}