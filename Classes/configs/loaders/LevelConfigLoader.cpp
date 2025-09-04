#include "LevelConfigLoader.h"
#include "json/rapidjson.h"
#include "cocos2d.h"

USING_NS_CC;

LevelConfig* LevelConfigLoader::loadLevelConfig(int levelId)
{
    std::string filePath = FileUtils::getInstance()->fullPathForFilename(StringUtils::format("configs/level_%d.json", levelId));

    if (!FileUtils::getInstance()->isFileExist(filePath))
    {
        CCLOG("Level config file not found: %s", filePath.c_str());
        return nullptr;
    }

    std::string jsonStr = FileUtils::getInstance()->getStringFromFile(filePath);
    if (jsonStr.empty())
    {
        CCLOG("Failed to read level config file: %s", filePath.c_str());
        return nullptr;
    }

    rapidjson::Document doc;
    doc.Parse(jsonStr.c_str());

    if (doc.HasParseError())
    {
        CCLOG("JSON parse error!");
        return nullptr;
    }

    auto config = new LevelConfig();
    int cardIdCounter = 0;

    if (doc.HasMember("Playfield"))
    {
        config->playfieldCards = _parseCards(doc["Playfield"], cardIdCounter, false);
    }

    if (doc.HasMember("Stack"))
    {
        config->stackCards = _parseCards(doc["Stack"], cardIdCounter, true);
    }

    return config;
}

std::vector<CardConfigData> LevelConfigLoader::_parseCards(const rapidjson::Value& cardsArray, int& cardIdCounter, bool isStackCard)
{
    std::vector<CardConfigData> cardList;
    for (rapidjson::SizeType i = 0; i < cardsArray.Size(); i++)
    {
        const rapidjson::Value& cardData = cardsArray[i];
        CardConfigData data;
        data.cardId = cardIdCounter++;
        data.face = (CardFaceType)cardData["CardFace"].GetInt();
        data.suit = (CardSuitType)cardData["CardSuit"].GetInt();

        if (isStackCard)
        {
            data.position = Vec2::ZERO; // 堆牌区位置由view动态计算
            data.isFaceUp = false;
        }
        else
        {
            const float kLayoutScaleX = 0.95f;  // X轴轻微压缩
            const float kLayoutScaleY = 0.8f; // Y轴大幅压缩，形成堆叠效果
            const Vec2 kPlayfieldCenter(540.0f, 1000.0f); // 重新调整视觉中心点

            Vec2 originalPos(cardData["Position"]["x"].GetFloat(), cardData["Position"]["y"].GetFloat());

            // 1. 计算相对于中心点的向量
            Vec2 centeredPos = originalPos - kPlayfieldCenter;
            // 2. 独立缩放向量的X和Y分量
            centeredPos.x *= kLayoutScaleX;
            centeredPos.y *= kLayoutScaleY;
            // 3. 将缩放后的向量加回到中心点，得到新坐标
            data.position = kPlayfieldCenter + centeredPos;

            data.isFaceUp = true; // 主牌区初始都设为翻开
        }
        cardList.push_back(data);
    }
    return cardList;
}