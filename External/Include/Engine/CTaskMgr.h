#pragma once

enum class TASK_TYPE
{
    // Param1 : Layer Index, Param2 : Object Adress
    CREATE_OBJECT,

    // Param1 : Object Adress
    DELETE_OBJECT,

    // Param1 : LEVEL_TYPE
    LEVEL_CHANGE,

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

    // Param1 : Parent Object, Param2 : Component Name
    ADD_COMPONENT,

    // Param1 : Object, Param2 : Layer Index
    CHANGE_LAYER,
};

struct FTask
{
    TASK_TYPE Type;
    UINT_PTR Param_1;
    UINT_PTR Param_2;
};

class CTaskMgr : public CSingleton<CTaskMgr>
{
    SINGLE(CTaskMgr);

private:
    vector<FTask> m_vecTask;

public:
    void tick();

    void AddTask(const FTask& _Task) { m_vecTask.push_back(_Task); }

private:
    void CREATE_OBJECT(const FTask& _Task);
    void DELETE_OBJECT(const FTask& _Task);
    void LEVEL_CHANGE(const FTask& _Task);
    void ADD_CHILD(const FTask& _Task);
    void DISCONNECT_PARENT(const FTask& _Task);
    void WINDOW_RESIZE(const FTask& _Task);
    void DELETE_ASSET(const FTask& _Task);
    void SCREENSHOT(const FTask& _Task);
    void MOUSE_COLOR_PICKING(const FTask& _Task);
    void MOUSE_RAY_PICKING(const FTask& _Task);
    void ADD_COMPONENT(const FTask& _Task);
    void CHANGE_LAYER(const FTask& _Task);
};
