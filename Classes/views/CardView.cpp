#include "CardView.h"

USING_NS_CC;

// 定义UI元素的缩放比例和边距，方便统一调整
const float kBigNumberScale = 0.9f;
const float kSmallElementScale = 0.8f;
const float kPaddingX = 25.0f;
const float kPaddingY = 30.0f;

CardView* CardView::create(const CardModel* model)
{
    CardView* pRet = new (std::nothrow) CardView();
    if (pRet && pRet->init(model))
    {
        pRet->autorelease();
        return pRet;
    }
    delete pRet;
    return nullptr;
}

bool CardView::init(const CardModel* model)
{
    if (!Node::init() || !model) return false;

    _cardId = model->id;

    _setupBaseSprite();
    _setupElements(model);
    _setupTouchListener();

    return true;
}

void CardView::_setupBaseSprite()
{
    auto baseSprite = Sprite::create("cards/card_general.png");
    if (!baseSprite) return;

    auto cardSize = baseSprite->getContentSize();
    this->setContentSize(cardSize);
    this->setAnchorPoint(Vec2(0.5f, 0.5f));
    baseSprite->setPosition(Vec2(cardSize.width / 2, cardSize.height / 2));
    this->addChild(baseSprite, 0);
}

void CardView::_setupElements(const CardModel* model)
{
    auto cardSize = this->getContentSize();

    // 1. 设置大的居中数字/字母
    std::string bigNumberPath = _getNumberTexturePath(model->face, model->suit, "big");
    auto bigNumberSprite = Sprite::create(bigNumberPath);
    if (bigNumberSprite)
    {
        bigNumberSprite->setPosition(Vec2(cardSize.width / 2, cardSize.height / 2));
        bigNumberSprite->setScale(kBigNumberScale);
        this->addChild(bigNumberSprite, 1);
    }

    // 2. 设置左上角的小数字和右上角的小花色
    std::string smallNumberPath = _getNumberTexturePath(model->face, model->suit, "small");
    std::string suitPath = _getSuitTexturePath(model->suit);

    auto tl_number = Sprite::create(smallNumberPath);
    if (tl_number)
    {
        tl_number->setScale(kSmallElementScale);
        float scaledWidth = tl_number->getContentSize().width * tl_number->getScale();
        float scaledHeight = tl_number->getContentSize().height * tl_number->getScale();
        tl_number->setPosition(
            Vec2(kPaddingX + scaledWidth / 2, cardSize.height - (kPaddingY + scaledHeight / 2))
        );
        this->addChild(tl_number, 2);
    }

    auto tr_suit = Sprite::create(suitPath);
    if (tr_suit)
    {
        tr_suit->setScale(kSmallElementScale);
        float scaledWidth = tr_suit->getContentSize().width * tr_suit->getScale();
        float scaledHeight = tr_suit->getContentSize().height * tr_suit->getScale();
        tr_suit->setPosition(
            Vec2(cardSize.width - (kPaddingX + scaledWidth / 2), cardSize.height - (kPaddingY + scaledHeight / 2))
        );
        this->addChild(tr_suit, 2);
    }
}

void CardView::_setupTouchListener()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(CardView::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void CardView::setCardClickCallback(const std::function<void(int)>& callback)
{
    _onCardClickCallback = callback;
}

bool CardView::onTouchBegan(Touch* touch, Event* event)
{
    // 将触摸点转换到本节点的坐标系下
    Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
    Rect rect = Rect(0, 0, getContentSize().width, getContentSize().height);

    if (rect.containsPoint(locationInNode))
    {
        if (_onCardClickCallback)
        {
            _onCardClickCallback(_cardId);
            return true; // 消费掉本次触摸
        }
    }
    return false;
}

std::string CardView::_getSuitTexturePath(CardSuitType suit)
{
    switch (suit)
    {
    case CST_CLUBS:    return "cards/suits/club.png";
    case CST_DIAMONDS: return "cards/suits/diamond.png";
    case CST_HEARTS:   return "cards/suits/heart.png";
    case CST_SPADES:   return "cards/suits/spade.png";
    default:           return "";
    }
}

std::string CardView::_getNumberTexturePath(CardFaceType face, CardSuitType suit, const std::string& sizePrefix)
{
    std::string color = (suit == CST_HEARTS || suit == CST_DIAMONDS) ? "red" : "black";
    std::string faceStr;
    if (face >= CFT_TWO && face <= CFT_TEN)
        faceStr = std::to_string(static_cast<int>(face) + 1);
    else if (face == CFT_ACE)   faceStr = "A";
    else if (face == CFT_JACK)  faceStr = "J";
    else if (face == CFT_QUEEN) faceStr = "Q";
    else if (face == CFT_KING)  faceStr = "K";
    else return "";

    return "cards/number/" + sizePrefix + "_" + color + "_" + faceStr + ".png";
}