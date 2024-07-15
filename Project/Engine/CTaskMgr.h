#pragma once

enum class TASK_TYPE
{
    // Param1 : Layer Index, Param2 : Object Address
    CREATE_OBJECT,

    // Param1 : Object Address
    DELETE_OBJECT,

    // Param1 : Level, Param2 : LEVEL_STATE
    CHANGE_LEVELSTATE,

    // Param1 : Parent Object, Param2 : Child Object
    // Param1 : Parent Object, Param2 : Child Object, Param3 : Bone Socket
    ADD_CHILD,

    // Param1 : Width , Param2 : Height
    WINDOW_RESIZE,

    // Param1 : Asset_TYPE, Param2 : Asset Address
    DELETE_ASSET,

    // No Param
    SCREENSHOT,

    // Param1 : MouseX , Param2 : MouseY
    MOUSE_COLOR_PICKING,

    // Param1 : MouseX , Param2 : MouseY
    MOUSE_COLLISION2D_PICKING,

    // Param1 : Object, Param2 : COMPONENT_TYPE
    ADD_COMPONENT,

    // Param1 : Object, Param2 : COMPONENT_TYPE
    REMOVE_COMPONENT,

    // Param1 : Object, Param2 : Script Address
    REMOVE_SCRIPT,

    // Param1 : Object, Param2 : Layer Index
    CHANGE_LAYER,

    // Param1 : Object
    CLONE_OBJECT,

    // Param1 : Object,
    // Param2 : Event Type - 0 : Respawn
    PHYSICS2D_EVNET,

    // Param1 : Object,
    // Param2 : Event Type - 0 : Respawn
    PHYSICS_EVNET,

    // Param1 : Origin Mesh, Param2 : Animation Mesh
    APPEND_ANIMATION,

    // Param1 : Mesh Data,
    SET_MODEL,

    // Param1 : Level Address
    CHANGE_LEVEL,
};

struct tTask
{
    TASK_TYPE Type;
    DWORD_PTR Param_1;
    DWORD_PTR Param_2;
    DWORD_PTR Param_3;
};

class CTaskMgr : public CSingleton<CTaskMgr>
{
    SINGLE(CTaskMgr);

private:
    std::queue<tTask> m_queueTask;

public:
    void tick();

    void AddTask(const tTask& _Task) { m_queueTask.push(_Task); }

private:
    void CREATE_OBJECT(const tTask& _Task);
    void DELETE_OBJECT(const tTask& _Task);
    void CHANGE_LEVELSTATE(const tTask& _Task);
    void ADD_CHILD(const tTask& _Task);
    void WINDOW_RESIZE(const tTask& _Task);
    void DELETE_ASSET(const tTask& _Task);
    void SCREENSHOT(const tTask& _Task);
    void MOUSE_COLOR_PICKING(const tTask& _Task);
    void MOUSE_COLLISION2D_PICKING(const tTask& _Task);
    void ADD_COMPONENT(const tTask& _Task);
    void REMOVE_COMPONENT(const tTask& _Task);
    void REMOVE_SCRIPT(const tTask& _Task);
    void CHANGE_LAYER(const tTask& _Task);
    void CLONE_OBJECT(const tTask& _Task);
    void PHYSICS2D_EVNET(const tTask& _Task);
    void PHYSICS_EVNET(const tTask& _Task);
    void APPEND_ANIMATION(const tTask& _Task);
    void SET_MODEL(const tTask& _Task);

    // 제일 마지막에 실행되야하는 이벤트
    void CHANGE_LEVEL(const tTask& _Task);
};
