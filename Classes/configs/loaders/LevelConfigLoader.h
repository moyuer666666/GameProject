#ifndef __LEVEL_CONFIG_LOADER_H__
#define __LEVEL_CONFIG_LOADER_H__

#include "configs/models/LevelConfig.h"
#include <string>
#include "json/document.h" // For rapidjson::Value

/**
 * @class LevelConfigLoader
 * @brief 负责从JSON文件加载关卡配置。
 * @details 这是一个静态工具类，提供加载和解析关卡配置文件的功能。
 *          它遵循了utils层的规范，不涉及业务逻辑且完全独立。
 */
class LevelConfigLoader
{
public:
    /**
     * @brief 加载指定ID的关卡配置。
     * @param levelId 关卡ID。
     * @return 成功则返回加载并解析后的LevelConfig对象，调用者需负责释放内存；
     *         如果失败则返回nullptr。
     */
    static LevelConfig* loadLevelConfig(int levelId);

private:
    /**
     * @brief 从JSON节点中解析卡牌数据。
     * @param cardsArray rapidjson的Value对象，应为一个包含卡牌信息的数组。
     * @param cardIdCounter [in/out] 卡牌ID计数器，用于生成唯一的卡牌ID。
     * @param isStackCard 标记是否为备用牌堆的牌，用于设置默认属性。
     * @return 解析后的卡牌配置数据列表。
     */
    static std::vector<CardConfigData> _parseCards(const rapidjson::Value& cardsArray, int& cardIdCounter, bool isStackCard);
};

#endif // __LEVEL_CONFIG_LOADER_H__