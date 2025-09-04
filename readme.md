- 未来怎么新加一个卡牌和一个新类型的回退功能
  - 可以直接查看 `6. 可扩展性指南`

- 便于体验程序我将程序打包到了 `程序.zip` 压缩包内，直接运行exe文件即可，不用重新编译，展示视频在同级目录下

# 游戏项目架构与模块设计文档

## 1. 项目概述

本文档旨在详细阐述一个基于Cocos2d-x引擎开发的纸牌游戏的核心架构、模块划分和代码实现。该游戏的核心玩法是从游戏区或备用区选择一张牌，与底牌进行配对消除。项目采用了经典的 **MVC（Model-View-Controller)** 设计模式，实现了数据、视图和逻辑的有效分离，为项目的可维护性和未来功能扩展奠定了坚实的基础。

该项目包含以下核心功能：
- 从JSON文件加载关卡配置。
- 动态生成和布局游戏牌局。
- 响应玩家点击，处理卡牌匹配或交换逻辑。
- 播放流畅的卡牌移动和匹配动画。
- 支持多步“回退”(Undo)操作，增强了游戏的可玩性。

## 2. 项目结构

项目文件按其在MVC架构中的角色和功能进行了清晰的组织，主要划分为以下目录：

- **controllers/**: 控制器层，负责核心业务逻辑。
  - `GameController.h/.cpp`: 游戏主控制器。
- **models/**: 模型层，定义游戏的核心数据结构。
  - `GameModel.h`: 游戏运行时的动态数据。
  - `CardModel.h`: 单张卡牌的动态数据。
  - `UndoModel.h`: 回退操作的数据模型。
- **views/**: 视图层，负责所有UI显示和动画。
  - `GameView.h/.cpp`: 游戏主视图，管理所有卡牌。
  - `CardView.h/.cpp`: 单张卡牌的视图。
- **managers/**: 管理器，处理特定子系统。
  - `UndoManager.h/.cpp`: 回退栈管理器。
- **services/**: 服务层，提供无状态的静态工具函数。
  - `GameModelFromLevelGenerator.h/.cpp`: 从关卡配置生成游戏数据模型。
  - `CardMatchService.h/.cpp`: 提供卡牌匹配和遮挡判断的规则。
- **configs/**: 配置层，负责数据的加载与解析。
  - `loaders/LevelConfigLoader.h/.cpp`: 从JSON文件加载关卡配置。
  - `models/LevelConfig.h`: 关卡配置的数据结构。
- **scenes/**: 场景文件。
  - `HelloWorldScene.h/.cpp`: 游戏主场景，程序的入口。

## 3. 核心架构：MVC设计模式分析

本项目严格遵循MVC（Model-View-Controller）设计模式，将用户界面（View）、业务逻辑（Controller）和数据（Model）三者分离，显著提高了代码的可维护性和可扩展性。



#### **Model (模型层)**
- **职责**: 存储游戏的所有状态数据，不包含任何业务逻辑。
- **核心组件**:
  - `GameModel`: 存储了游戏中所有牌堆（主牌区、抽牌堆、备用牌堆、废牌堆）的ID列表，以及当前底牌的ID。它是整个游戏状态的快照。
  - `CardModel`: 定义了单张卡牌的运行时属性，如ID、花色、点数、位置和是否正面朝上。
  - `UndoModel`: 定义了`UndoRecord`结构体，用于精确记录每一次可回退操作的必要信息。
- **特点**: 模型层是纯粹的数据容器。当Controller需要改变游戏状态时，它会直接修改`GameModel`中的数据。

#### **View (视图层)**
- **职责**: 负责将模型数据显示给用户，并捕获用户的输入事件（如点击）。
- **核心组件**:
  - `GameView`: 作为游戏的主视图，它持有一个**只读的(const)** `GameModel`指针。它根据模型数据创建和布局所有的`CardView`实例，并提供播放动画的接口。
  - `CardView`: 代表单张卡牌的视图，负责渲染卡牌的视觉元素，并监听用户的触摸事件。
  - `HelloWorldScene`: 作为Cocos2d-x的场景，是所有视图的根节点。
- **特点**:
  - **单向数据流**: 视图层只能读取模型数据来更新显示，绝不能直接修改模型。
  - **事件通知**: 当用户与UI交互时（例如点击一张牌），视图层会通过**回调函数**（Callback）通知控制器层，将事件处理权交给Controller。

#### **Controller (控制器层)**
- **职责**: 作为模型和视图之间的协调者，负责处理所有的业务逻辑。
- **核心组件**:
  - `GameController`: 它是游戏的大脑。它持有`GameModel`的实例（可读写）和`GameView`的实例。
- **工作流程**:
  1. **接收输入**: 通过`GameView`注册的回调函数（如 `_handleCardClick`）接收用户操作。
  2. **处理逻辑**: 根据游戏规则（借助`CardMatchService`等服务）判断操作是否有效。
  3. **更新模型**: 如果操作有效，`GameController`会修改`GameModel`的数据（例如，将一张牌从主牌区移动到废牌堆）。同时，它会调用`UndoManager`来记录这次操作，以便后续回退。
  4. **更新视图**: 在模型更新后，`GameController`会调用`GameView`的接口（如 `playMatchAnimation` 或 `updateCardPositions`）来通知视图层刷新界面，播放相应的动画。

## 4. 模块详解

#### `GameController`
游戏的中枢神经。它通过`startGame`方法初始化游戏，加载关卡，创建`GameModel`和`GameView`。它通过`_handleCardClick`和`_handleUndoClick`等私有方法处理来自视图的事件，并协调模型、视图和管理器完成整个操作流程。

#### `GameModel` & `CardModel`
纯粹的数据类。`GameModel`的析构函数负责释放所有动态分配的`CardModel`对象，避免内存泄漏。这些类被设计为可序列化的，为未来的存档/读档功能打下基础。

#### `GameView` & `CardView`
视图的实现。`GameView`使用一个`std::map<int, CardView*>`来管理所有的卡牌视图，实现了通过卡牌ID快速查找对应视图。它通过`_animationRunning`标志位作为动画锁，防止在动画播放期间响应用户输入，保证了视觉效果和逻辑的一致性。

#### `UndoManager`
一个典型的栈管理器。它使用`std::vector<UndoRecord>`作为栈结构，提供了`record...Action`系列方法来压入记录，以及`popLastAction`方法来弹出记录。每次成功的玩家操作都会被记录，而每次回退都会消耗一条记录。

#### `LevelConfigLoader` & `GameModelFromLevelGenerator`
这两个类共同承担了游戏初始化的责任。`LevelConfigLoader`作为独立的工具类，负责将JSON配置文件解析为中间数据结构`LevelConfig`。随后，`GameModelFromLevelGenerator`服务类则负责将`LevelConfig`转换为游戏运行时所需的、包含完整动态信息的`GameModel`。这种两步走的分离使得数据解析和游戏逻辑生成解耦。

#### `CardMatchService`
一个无状态的服务类，提供了静态的游戏规则判断方法。`isMatch`定义了卡牌匹配的规则（点数相邻或A-K配对），而`isCardBlocked`则通过几何计算判断一张牌是否被其他牌遮挡，这是游戏的核心玩法限制之一。将这些纯逻辑函数抽离成服务，使`GameController`的逻辑更清晰。

## 5. 游戏流程

1.  **启动**: `AppDelegate` 创建并运行 `HelloWorldScene`。
2.  **初始化**: `HelloWorldScene::init` 创建 `GameController` 实例，并调用 `_gameController->startGame(1)` 启动第一关。
3.  **加载关卡**:
    - `GameController::startGame` 调用 `LevelConfigLoader::loadLevelConfig` 读取并解析 `level_1.json` 文件。
    - 接着，调用 `GameModelFromLevelGenerator::generateGameModel` 将配置数据转换为 `GameModel` 实例，完成所有卡牌对象的创建和初始牌堆的分配。
4.  **创建视图**: `GameController` 创建 `GameView` 实例，并将 `GameModel` 的只读指针传递给它。`GameView`根据模型数据创建所有 `CardView`。
5.  **绑定回调**: `GameController` 将自己的成员函数（如 `_handleCardClick`）作为回调注册到 `GameView` 中。
6.  **玩家操作 (以点击主牌区匹配为例)**:
    - 玩家点击一张 `CardView`。
    - `CardView::onTouchBegan` 触发，调用 `_onCardClickCallback` 回调，参数为被点击的卡牌ID。
    - `GameController::_handleCardClick` 被调用。它首先检查动画是否正在播放。
    - `_handlePlayfieldCardClick` 方法被调用。
    - 它使用 `CardMatchService::isCardBlocked` 检查该牌是否可被点击。
    - 使用 `CardMatchService::isMatch` 检查该牌是否能与当前底牌匹配。
    - 如果可以匹配：
        - 调用 `_undoManager->recordMatchAction` 记录本次操作。
        - **更新模型**: 修改 `_gameModel` 的数据（`baseCardId` 改变，`wastePileCardIds` 和 `playfieldCardIds` 更新）。
        - **更新视图**: 调用 `_gameView->playMatchAnimation` 播放卡牌飞向底牌堆的动画。
        - 动画结束后，在回调中调用 `_gameView->updateCardPositions()` 刷新整个牌局的显示。

## 6. 可扩展性指南

### 如何添加一个新类型的回退功能？

假设我们要添加一个新功能：“使用道具将一张主牌区最上面的牌直接移到废牌堆”，这个操作也需要支持回退。

**第1步：定义新的操作类型 (Model层)**

在 `UndoModel.h` 文件中，为 `UndoActionType` 枚举添加一个新成员：

```cpp
enum UndoActionType
{
    MATCH_FROM_PLAYFIELD,
    SWAP_FROM_STOCKPILE,
    DRAW_FROM_STOCK,
    USE_TOOL_ON_PLAYFIELD // 新增的操作类型
};
```
同时，检查 `UndoRecord` 结构体是否需要额外字段来记录此操作。在此例中，`movedCardId`, `originalPosition`, 和 `previousBaseCardId` 字段已经足够，无需修改。

**第2步：添加新的记录方法 (Manager层)**

在 `UndoManager.h` 和 `UndoManager.cpp` 中，添加一个用于记录此新操作的方法：

```cpp
// In UndoManager.h
void recordToolAction(int cardToMoveId, const cocos2d::Vec2& originalPos, int oldBaseCardId);

// In UndoManager.cpp
void UndoManager::recordToolAction(int cardToMoveId, const cocos2d::Vec2 &originalPos, int oldBaseCardId)
{
    UndoRecord record;
    record.type = USE_TOOL_ON_PLAYFIELD;
    record.movedCardId = cardToMoveId;
    record.originalPosition = originalPos;
    record.previousBaseCardId = oldBaseCardId; // 假设道具也将旧底牌移入废牌堆
    _undoStack.push_back(record);
}
```

**第3步：实现新功能的逻辑与回退 (Controller层)**

在 `GameController.cpp` 中：
1.  **实现新功能**: 创建一个处理使用道具的函数，例如 `_handleUseToolClick`。在这个函数中，当你修改 `_gameModel` 之前，调用刚刚创建的记录方法：
    ```cpp
    void GameController::_handleUseToolClick(int targetCardId)
    {
        // ... (找到目标牌 card, 检查是否符合道具使用条件)
        
        // 1. 记录操作以便回退
        _undoManager->recordToolAction(targetCardId, card->position, _gameModel->baseCardId);
        
        // 2. 更新数据模型
        _gameModel->wastePileCardIds.push_back(_gameModel->baseCardId);
        _gameModel->baseCardId = targetCardId;
        // ... (从 playfieldCardIds 中移除 targetCardId)

        // 3. 播放动画，更新视图
        _gameView->playMatchAnimation(targetCardId, oldBaseCardId, ...);
    }
    ```

2.  **实现回退逻辑**: 在 `_handleUndoClick` 的 `switch` 语句中，添加对新操作类型的处理：
    ```cpp
    void GameController::_handleUndoClick()
    {
        // ...
        if (_undoManager->popLastAction(&record))
        {
            switch (record.type)
            {
            // ... (existing cases)
            case USE_TOOL_ON_PLAYFIELD:
                // 与 _undoMatchFromPlayfield 逻辑相同，可以复用或新建一个函数
                _undoMatchFromPlayfield(record); 
                break;
            }
        }
    }
    ```
    由于这个新操作的回退逻辑与“匹配”操作完全一致，我们可以直接复用 `_undoMatchFromPlayfield` 方法。如果回退逻辑不同，则需要创建一个新的私有方法 `_undoToolAction(const UndoRecord& record)` 来专门处理。

**第4步：(可选) 添加新的回退动画 (View层)**

如果新操作的回退需要一种独特的动画效果，可以在 `GameView.h/.cpp` 中添加一个新的动画接口，例如 `playUndoToolAnimation(...)`，并在 `GameController` 的回退逻辑中调用它。

### 如何添加一个新类型的卡牌？

假设我们要添加一种“百搭牌”(Joker)，它可以和任何牌匹配。

**第1步：扩展卡牌定义 (Model/Config层)**

在 `LevelConfig.h` 中，扩展 `CardFaceType` 枚举：

```cpp
enum CardFaceType
{
    // ...
    CFT_KING,      // K (值为12)
    CFT_JOKER,     // 新增的百搭牌
    CFT_NUM_CARD_FACE_TYPES
};
```

**第2步：更新关卡加载逻辑 (Config层)**

确保 `LevelConfigLoader.cpp` 能够正确解析JSON中代表百搭牌的数据。你需要在关卡配置文件（如`level_1.json`）中定义这种新牌。

**第3步：更新卡牌匹配逻辑 (Service层)**

在 `CardMatchService.cpp` 中，修改 `isMatch` 函数的逻辑，加入对百搭牌的判断：

```cpp
bool CardMatchService::isMatch(const CardModel *playfieldCard, const CardModel *baseCard)
{
    if (!playfieldCard || !baseCard)
        return false;

    // 规则：百搭牌可以和任何牌匹配
    if (playfieldCard->face == CFT_JOKER || baseCard->face == CFT_JOKER)
    {
        return true;
    }

    // A(0)和K(12)可以互相匹配
    if ((playfieldCard->face == CFT_ACE && baseCard->face == CFT_KING) ||
        (playfieldCard->face == CFT_KING && baseCard->face == CFT_ACE))
    {
        return true;
    }

    // 其他情况判断点数是否相差1
    return abs(playfieldCard->face - baseCard->face) == 1;
}
```

**第4步：添加新卡牌的视觉资源 (View层)**

在 `CardView.cpp` 中，你需要更新 `_getNumberTexturePath` 方法来处理 `CFT_JOKER`，让它返回正确的纹理路径。同时，你需要将百搭牌的图片资源添加到项目中。

```cpp
std::string CardView::_getNumberTexturePath(...)
{
    // ...
    else if (face == CFT_KING)  faceStr = "K";
    else if (face == CFT_JOKER) faceStr = "Joker"; // 处理百搭牌
    else return "";

    return "cards/number/" + sizePrefix + "_" + color + "_" + faceStr + ".png";
}
```

通过以上步骤，新功能就被无缝地集成到了现有的MVC架构中，每一步的修改都清晰地对应到一个特定的层次，不会引起混乱。