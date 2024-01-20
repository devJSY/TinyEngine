#pragma once
#include "CEditor.h"

#include <imgui_internal.h>
#include <imgui_node_editor\\Blueprint\\tinyfiledialogs\\tinyfiledialogs.h>
#include <inttypes.h>

#include <imgui_node_editor\\Blueprint\\crude_logger.h>
#include <imgui_node_editor\\Blueprint\\crude_blueprint.h>
#include <imgui_node_editor\\Blueprint\\crude_blueprint_library.h>
#include <imgui_node_editor\\Blueprint\\blueprint_editor_document.h>
#include <imgui_node_editor\\Blueprint\\blueprint_editor_utilities.h>
#include <imgui_node_editor\\Blueprint\\blueprint_editor_icons.h>
#include <imgui_node_editor\\Blueprint\\crude_layout.h>
#include <imgui_node_editor\\Blueprint\\imgui_extras.h>
#include <imgui_node_editor\\imgui_node_editor.h>

#include <imgui_node_editor\\crude_json.h>

namespace ed = ax::NodeEditor;

using namespace crude_blueprint;
using namespace blueprint_editor;
using namespace blueprint_editor_utilities;

static ImEx::MostRecentlyUsedList Application_GetMostRecentlyOpenFileList()
{
    return ImEx::MostRecentlyUsedList("MostRecentlyOpenList");
}

static EntryPointNode* FindEntryPointNode(Blueprint& blueprint)
{
    for (auto& node : blueprint.GetNodes())
    {
        if (node->GetTypeInfo().m_Id == EntryPointNode::GetStaticTypeInfo().m_Id)
        {
            return static_cast<EntryPointNode*>(node);
        }
    }

    return nullptr;
}

using std::function;

enum class EventHandle : uint64_t
{
    Invalid
};

template <typename... Args>
struct Event
{
    using Delegate = function<void(Args...)>;

    EventHandle Add(Delegate delegate)
    {
        auto eventHandle = static_cast<EventHandle>(++m_LastHandleId);
        m_Delegates[eventHandle] = std::move(delegate);
        return eventHandle;
    }

    bool Remove(EventHandle eventHandle) { return m_Delegates.erase(eventHandle) > 0; }

    void Clear() { m_Delegates.clear(); }

    template <typename... CallArgs>
    void Invoke(CallArgs&&... args)
    {
        vector<Delegate> delegates;
        delegates.reserve(m_Delegates.size());
        for (auto& entry : m_Delegates)
            delegates.push_back(entry.second);

        for (auto& delegate : delegates)
            delegate(args...);
    }

    EventHandle operator+=(Delegate delegate) { return Add(std::move(delegate)); }
    bool operator-=(EventHandle eventHandle) { return Remove(eventHandle); }
    template <typename... CallArgs>
    void operator()(CallArgs&&... args)
    {
        Invoke(std::forward<CallArgs>(args)...);
    }

private:
    using EventHandleType = std::underlying_type_t<EventHandle>;

    map<EventHandle, Delegate> m_Delegates;
    EventHandleType m_LastHandleId = 0;
};

#pragma region Action

struct Action
{
    using OnChangeEvent = Event<Action*>;
    using OnTriggeredEvent = Event<>;

    Action() = default;
    Action(string_view name, OnTriggeredEvent::Delegate delegate = {});

    void SetName(string_view name);
    const string& GetName() const;

    void SetEnabled(bool set);
    bool IsEnabled() const;

    void Execute();

    OnChangeEvent OnChange;
    OnTriggeredEvent OnTriggered;

private:
    string m_Name;
    bool m_IsEnabled = true;
};

Action::Action(string_view name, OnTriggeredEvent::Delegate delegate)
    : m_Name(to_string(name))
{
    if (delegate)
        OnTriggered += std::move(delegate);
}

void Action::SetName(string_view name)
{
    if (m_Name == name)
        return;

    m_Name = to_string(name);

    OnChange(this);
}

const crude_blueprint::string& Action::GetName() const
{
    return m_Name;
}

void Action::SetEnabled(bool set)
{
    if (m_IsEnabled == set)
        return;

    m_IsEnabled = set;

    OnChange(this);
}

bool Action::IsEnabled() const
{
    return m_IsEnabled;
}

void Action::Execute()
{
    LOGV("Action: %s", m_Name.c_str());
    OnTriggered();
}
#pragma endregion

class CBlueprintEditor2 : public CEditor
{
private:
    ed::EditorContext* m_Editor;
    crude_logger::OverlayLogger m_OverlayLogger;

    unique_ptr<Document> m_Document;
    Blueprint* m_Blueprint = nullptr;

    CreateNodeDialog m_CreateNodeDailog;
    NodeContextMenu m_NodeContextMenu;
    PinContextMenu m_PinContextMenu;
    LinkContextMenu m_LinkContextMenu;

    Action m_File_New = {"New", [this] { File_New(); }};
    Action m_File_Open = {"Open...", [this] { File_Open(); }};
    Action m_File_SaveAs = {"Save As...", [this] { File_SaveAs(); }};
    Action m_File_Save = {"Save", [this] { File_Save(); }};
    Action m_File_Close = {"Close", [this] { File_Close(); }};
    Action m_File_Exit = {"Exit", [this] { File_Exit(); }};

    Action m_Edit_Undo = {"Undo", [this] { Edit_Undo(); }};
    Action m_Edit_Redo = {"Redo", [this] { Edit_Redo(); }};
    Action m_Edit_Cut = {"Cut", [this] { Edit_Cut(); }};
    Action m_Edit_Copy = {"Copy", [this] { Edit_Copy(); }};
    Action m_Edit_Paste = {"Paste", [this] { Edit_Paste(); }};
    Action m_Edit_Duplicate = {"Duplicate", [this] { Edit_Duplicate(); }};
    Action m_Edit_Delete = {"Delete", [this] { Edit_Delete(); }};
    Action m_Edit_SelectAll = {"Select All", [this] { Edit_SelectAll(); }};

    Action m_View_NavigateBackward = {"Navigate Backward", [this] { View_NavigateBackward(); }};
    Action m_View_NavigateForward = {"Navigate Forward", [this] { View_NavigateForward(); }};

    Action m_Blueprint_Start = {"Start", [this] { Blueprint_Start(); }};
    Action m_Blueprint_Step = {"Step", [this] { Blueprint_Step(); }};
    Action m_Blueprint_Stop = {"Stop", [this] { Blueprint_Stop(); }};
    Action m_Blueprint_Run = {"Run", [this] { Blueprint_Run(); }};

public:
    virtual void init() override;
    virtual void tick() override{};
    virtual void finaltick() override{};
    virtual void render() override;

public:
    void InstallDocumentCallbacks(ed::Config& config);
    void CreateExampleDocument();

    // ImGuiWindowFlags GetWindowFlags() const override
    //{
    //     auto flags = Application::GetWindowFlags();
    //     flags |= ImGuiWindowFlags_MenuBar;
    //     flags &= ~ImGuiWindowFlags_NoSavedSettings;
    //     return flags;
    // }

    bool CanClose() { return File_Close(); }
    // bool CanClose() override { return File_Close(); }

    void UpdateTitle();
    void UpdateActions();
    void ShowMainMenu();
    void ShowToolbar();

    // Iterate over blueprint nodes and commit them to node editor.
    void CommitBlueprintNodes(Blueprint& blueprint, DebugOverlay& debugOverlay);
    void HandleCreateAction(Document& document);
    void HandleDestroyAction(Document& document);
    void HandleContextMenuAction(Blueprint& blueprint);
    void ShowDialogs(Document& document);
    void ShowInfoTooltip(Blueprint& blueprint);
    bool File_IsOpen() { return m_Document != nullptr; }
    bool File_IsModified() { return m_Document->m_IsModified; }
    void File_MarkModified();
    void File_New();
    bool File_Open(string_view path, string* error = nullptr);
    bool File_Open();
    bool File_Close();
    bool File_SaveAsEx(string_view path);
    bool File_SaveAs();
    bool File_Save();
    void File_Exit();
    void Edit_Undo() { m_Document->Undo(); }
    void Edit_Redo() { m_Document->Redo(); }
    void Edit_Cut() {}
    void Edit_Copy() {}
    void Edit_Paste() {}
    void Edit_Duplicate() {}
    void Edit_Delete() {}
    void Edit_SelectAll() {}
    void View_NavigateBackward() {}
    void View_NavigateForward() {}
    void Blueprint_Start();
    void Blueprint_Step();
    void Blueprint_Stop();
    void Blueprint_Run();

public:
    CBlueprintEditor2();
    virtual ~CBlueprintEditor2();
};
