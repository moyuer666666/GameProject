#include "GameController.h"
#include "configs/loaders/LevelConfigLoader.h"
#include "services/GameModelFromLevelGenerator.h"
#include "services/CardMatchService.h"
#include <algorithm>

USING_NS_CC;

GameController::GameController(Node* rootNode)
    : _rootNode(rootNode), _gameModel(nullptr), _gameView(nullptr), _undoManager(nullptr)
{
}

GameController::~GameController()
{
    // _gameView是Node，会被父节点自动管理，无需手动删除
    CC_SAFE_DELETE(_gameModel);
    CC_SAFE_DELETE(_undoManager);
}

void GameController::startGame(int levelId)
{
    auto levelConfig = LevelConfigLoader::loadLevelConfig(levelId);
    if (!levelConfig) return;

    // 清理旧数据（如果存在）
    CC_SAFE_DELETE(_gameModel);
    if (_gameView) _gameView->removeFromParent();

    _gameModel = GameModelFromLevelGenerator::generateGameModel(levelConfig);
    delete levelConfig; // levelConfig完成使命后即可删除

    _undoManager = new UndoManager();

    _gameView = GameView::create(_gameModel);
    _rootNode->addChild(_gameView);

    // 绑定回调
    _gameView->setCardClickCallback(CC_CALLBACK_1(GameController::_handleCardClick, this));
    _gameView->setUndoClickCallback(CC_CALLBACK_0(GameController::_handleUndoClick, this));
}

void GameController::_handleCardClick(int cardId)
{
    if (_gameView->isAnimationRunning() || cardId == _gameModel->baseCardId)
    {
        return;
    }

    if (_handlePlayfieldCardClick(cardId)) return;
    if (_handleStockpileCardClick(cardId)) return;
}

bool GameController::_handlePlayfieldCardClick(int cardId)
{
    auto& playfieldIds = _gameModel->playfieldCardIds;
    auto it = std::find(playfieldIds.begin(), playfieldIds.end(), cardId);

    if (it != playfieldIds.end())
    {
        // 规则：只有未被遮挡的牌才能进行匹配
        if (CardMatchService::isCardBlocked(cardId, _gameModel))
        {
            CCLOG("Card %d is blocked!", cardId);
            return true; // 事件被识别但被阻止，返回true
        }

        CardModel* clickedCard = _gameModel->allCards.at(cardId);
        CardModel* baseCard = _gameModel->allCards.at(_gameModel->baseCardId);

        if (CardMatchService::isMatch(clickedCard, baseCard))
        {
            int oldBaseCardId = _gameModel->baseCardId;
            _undoManager->recordMatchAction(cardId, clickedCard->position, oldBaseCardId);

            // 更新数据模型
            _gameModel->wastePileCardIds.push_back(oldBaseCardId);
            _gameModel->baseCardId = cardId;
            playfieldIds.erase(it);

            // 更新视图
            _gameView->playMatchAnimation(cardId, oldBaseCardId, [this]() {
                _gameView->updateCardPositions();
                });
        }
        return true; // 主牌区逻辑处理完毕
    }
    return false; // 不是主牌区的牌
}

bool GameController::_handleStockpileCardClick(int cardId)
{
    auto& stockpileIds = _gameModel->stockpileCardIds;

    // 规则：只有备用牌区的最右边一张才能被点击
    if (!stockpileIds.empty() && cardId == stockpileIds.back())
    {
        int oldBaseCardId = _gameModel->baseCardId;
        _undoManager->recordSwapAction(cardId, oldBaseCardId);

        // 更新数据模型
        stockpileIds.pop_back();
        _gameModel->wastePileCardIds.push_back(oldBaseCardId);
        _gameModel->baseCardId = cardId;

        // 更新视图
        _gameView->playMatchAnimation(cardId, oldBaseCardId, [this]() {
            _gameView->updateCardPositions();
            });
        return true; // 备用牌区逻辑处理完毕
    }
    return false; // 不是备用牌区的牌
}


void GameController::_handleUndoClick()
{
    if (_gameView->isAnimationRunning()) return;

    UndoRecord record;
    if (_undoManager->popLastAction(&record))
    {
        switch (record.type)
        {
        case MATCH_FROM_PLAYFIELD:
            _undoMatchFromPlayfield(record);
            break;
        case SWAP_FROM_STOCKPILE:
            _undoSwapFromStockpile(record);
            break;
        case DRAW_FROM_STOCK:
            // 当前无此操作
            break;
        }
    }
}

void GameController::_undoMatchFromPlayfield(const UndoRecord& record)
{
    // 更新数据模型
    _gameModel->baseCardId = record.previousBaseCardId;
    _gameModel->wastePileCardIds.pop_back();
    _gameModel->playfieldCardIds.push_back(record.movedCardId);

    // 更新视图
    _gameView->playUndoAnimation(record.movedCardId, record.previousBaseCardId, record.originalPosition, [this]() {
        _gameView->updateCardPositions();
        });
}

void GameController::_undoSwapFromStockpile(const UndoRecord& record)
{
    // 更新数据模型
    _gameModel->baseCardId = record.previousBaseCardId;
    _gameModel->wastePileCardIds.pop_back();
    _gameModel->stockpileCardIds.push_back(record.movedCardId);

    // 更新视图
    _gameView->playUndoSwapAnimation(record.movedCardId, record.previousBaseCardId, [this]() {
        _gameView->updateCardPositions();
        });
}