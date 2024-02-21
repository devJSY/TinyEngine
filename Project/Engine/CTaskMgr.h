#pragma once

enum class TASK_TYPE
{
    // Param1 : Layer Index, Param2 : Object Adress
    CREATE_OBJECT,

    // Param1 : Object Adress
    DELETE_OBJECT,

    // Param1 : Level Adress
    LEVEL_CHANGE,

    // Param1 : Level, Param2 : LEVEL_STATE
    CHANGE_LEVELSTATE,

    // Param1 : Parent Object, Param2 : Child Object
    ADD_CHILD,

    // Param1 : Parent Object, Param2 : Child Object
    DISCONNECT_PARENT,

    // Param1 : Width , Param2 : Height
    WINDOW_RESIZE,

    // wParam : Asset_TYPE, lParam : Resource Adress
    DELETE_ASSET,

    // No Param
    SCREENSHOT,

    // Param1 : MouseX , Param2 : MouseY
    MOUSE_COLOR_PICKING,

    // Param1 : MouseX , Param2 : MouseY
    MOUSE_RAY_PICKING,

    // Param1 : Object, Param2 : COMPONENT_TYPE
    ADD_COMPONENT,

    // Param1 : Object, Param2 : COMPONENT_TYPE
    REMOVE_COMPONENT,

    // Param1 : Object, Param2 : Layer Index
    LAYER_CHANGE,

    // Param1 : Object
    CLONE_OBJECT
};

struct tTask
{
    TASK_TYPE Type;
    UINT_PTR Param_1;
    UINT_PTR Param_2;
};

class CTaskMgr : public CSingleton<CTaskMgr>
{
    SINGLE(CTaskMgr);

private:
    vector<tTask> m_vecTask;

public:
    void tick();

    void AddTask(const tTask& _Task) { m_vecTask.push_back(_Task); }

private:
    void CREATE_OBJECT(const tTask& _Task);
    void DELETE_OBJECT(const tTask& _Task);
    void LEVEL_CHANGE(const tTask& _Task);
    void CHANGE_LEVELSTATE(const tTask& _Task);
    void ADD_CHILD(const tTask& _Task);
    void DISCONNECT_PARENT(const tTask& _Task);
    void WINDOW_RESIZE(const tTask& _Task);
    void DELETE_ASSET(const tTask& _Task);
    void SCREENSHOT(const tTask& _Task);
    void MOUSE_COLOR_PICKING(const tTask& _Task);
    void MOUSE_RAY_PICKING(const tTask& _Task);
    void ADD_COMPONENT(const tTask& _Task);
    void REMOVE_COMPONENT(const tTask& _Task);
    void LAYER_CHANGE(const tTask& _Task);
    void CLONE_OBJECT(const tTask& _Task);
};
