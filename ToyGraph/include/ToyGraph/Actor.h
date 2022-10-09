/*
 * 演员类。
 * 
 * 创建时间：2022年9月20日 于上海市嘉定区安亭镇
 */

#pragma once

// glm
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// stl
#include <functional>

/**
 * 演员类。
 * 
 * 所有可以加入到场景内的物品都应继承自本类（包括间接）。
 * 包括但不限于：
 *   模型、灯光、音频
 */
class Actor {

public: // 构造和析构。

    /**
     * 演员类构造函数。完成初始化工作。
     */
    Actor();

    /**
     * 演员类析构函数。完成内存释放等清理工作。
     */
    virtual ~Actor() {}

public: // 钩子成员。

    /**
     * 每帧被调用。用于处理 Actor 的变化。
     * 
     * @param deltaSeconds 帧间隔时间。单位为秒。
     */
    std::function<void (float deltaSeconds)> tick = [] (...) {};

public: // setters and getters.

    /* -------- setters and getters. -------- */

    // position
    virtual Actor& setPosition(const glm::vec3& position);
    virtual glm::vec3& getPositionRef();
    virtual const glm::vec3& getPosition();

    // yaw
    virtual Actor& setYaw(const float yaw);
    virtual float getYaw();

    // pitch
    virtual Actor& setPitch(const float pitch);
    virtual float getPitch();

    // roll
    virtual Actor& setRoll(const float roll);
    virtual float getRoll();

    
    virtual const glm::vec3& getDirectionVectorUp();

    virtual const glm::vec3& getDirectionVectorRight();
   
    virtual const glm::vec3& getDirectionVectorFront();

public: // 一般方法。
    virtual void move(float distance, const glm::vec3& direction);

protected: // 自动工具成员。外部一般不直接操作。
    glm::vec3 directionVectorUp;
    glm::vec3 directionVectorRight;
    glm::vec3 directionVectorFront;

    bool directionVectorsIsLatest = false;

protected: // 自动工具方法。外部一般不直接调用。
    void updateDirectionVectors();

protected: // 内部成员。

    /*
        成员变量非必要不要直接读写。
        推荐使用 setter 和 getter 完成读写操作。
        setter 和 getter 内会包含校验步骤。

        如果情况特殊，允许直接操作成员，但可能很危险。
    */

    /**
     * 世界坐标。
     */
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);

    /* ------------ 欧拉角度。 ------------ */

    /**
     * 俯仰角。
     */
    float pitch = 0.0f;

    /**
     * 偏航角。
     */
    float yaw = 0.0f;

    /**
     * 翻滚角。
     */
    float roll = 0.0f;

private: // 禁止内容。

    /**
     * 复制构造。禁止使用。
     */
    Actor(const Actor& actor) {};

};
