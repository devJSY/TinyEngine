#pragma once

#include "CEditor.h"

#include <imgui_node_editor\\utilities\\builders.h>
#include <imgui_node_editor\\utilities\\widgets.h>
#include <imgui_node_editor\\imgui_node_editor.h>
#include <imgui_node_editor\\imgui_node_editor_internal.h>

#include "CTexture.h"

namespace ed = ax::NodeEditor;
namespace util = ax::NodeEditor::Utilities;

enum class PinType
{
    Flow,
    Bool,
    Int,
    Float,
    String,
    Object,
    Function,
    Delegate,
};

enum class PinKind
{
    Output,
    Input
};

enum class NodeType
{
    Blueprint,
    Simple,
    Tree,
    Comment,
    Houdini
};

struct Node;

struct Pin
{
    ed::PinId ID;
    ::Node* Node;
    std::string Name;
    PinType Type;
    PinKind Kind;

    Pin(int id, const char* name, PinType type)
        : ID(id)
        , Node(nullptr)
        , Name(name)
        , Type(type)
        , Kind(PinKind::Input)
    {
    }
};

struct Node
{
    ed::NodeId ID;
    std::string Name;
    std::vector<Pin> Inputs;
    std::vector<Pin> Outputs;
    ImColor Color;
    NodeType Type;
    ImVec2 Size;

    std::string State;
    std::string SavedState;

    Node(int id, const char* name, ImColor color = ImColor(255, 255, 255))
        : ID(id)
        , Name(name)
        , Color(color)
        , Type(NodeType::Blueprint)
        , Size(0, 0)
    {
    }
};

struct Link
{
    ed::LinkId ID;

    ed::PinId StartPinID;
    ed::PinId EndPinID;

    ImColor Color;

    Link(ed::LinkId id, ed::PinId startPinId, ed::PinId endPinId)
        : ID(id)
        , StartPinID(startPinId)
        , EndPinID(endPinId)
        , Color(255, 255, 255)
    {
    }
};

struct NodeIdLess
{
    bool operator()(const ed::NodeId& lhs, const ed::NodeId& rhs) const { return lhs.AsPointer() < rhs.AsPointer(); }
};

class CBlueprintEditor : public CEditor
{
private:
    ed::EditorContext* m_EditorContext;
    int m_NextId;
    int m_PinIconSize;
    std::vector<Node> m_Nodes;
    std::vector<Link> m_Links;
    Ptr<CTexture> m_HeaderBackground;
    Ptr<CTexture> m_SaveIcon;
    Ptr<CTexture> m_RestoreIcon;
    float m_TouchTime;
    std::map<ed::NodeId, float, NodeIdLess> m_NodeTouchTime;
    bool m_ShowOrdinals;

public:
    virtual void init() override;
    virtual void tick() override{};
    virtual void finaltick() override{};
    virtual void render(bool* open) override;

private:
    void render();

private:
    // imgui_node_editor features
    inline ImRect ImGui_GetItemRect() { return ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax()); }

    inline ImRect ImRect_Expanded(const ImRect& rect, float x, float y)
    {
        auto result = rect;
        result.Min.x -= x;
        result.Min.y -= y;
        result.Max.x += x;
        result.Max.y += y;
        return result;
    }

    bool Splitter(bool split_vertically, float thickness, float* size1, float* size2, float min_size1, float min_size2,
                  float splitter_long_axis_size = -1.0f);

    int GetNextId() { return m_NextId++; }
    // ed::NodeId GetNextNodeId()
    //{
    //     return ed::NodeId(GetNextId());
    // }
    ed::LinkId GetNextLinkId() { return ed::LinkId(GetNextId()); }
    void TouchNode(ed::NodeId id) { m_NodeTouchTime[id] = m_TouchTime; }
    float GetTouchProgress(ed::NodeId id);
    void UpdateTouch();

    Node* FindNode(ed::NodeId id);
    Link* FindLink(ed::LinkId id);
    Pin* FindPin(ed::PinId id);
    bool IsPinLinked(ed::PinId id);
    bool CanCreateLink(Pin* a, Pin* b);

    // void DrawItemRect(ImColor color, float expand = 0.0f)
    //{
    //     ImGui::GetWindowDrawList()->AddRect(
    //         ImGui::GetItemRectMin() - ImVec2(expand, expand),
    //         ImGui::GetItemRectMax() + ImVec2(expand, expand),
    //         color);
    // };

    // void FillItemRect(ImColor color, float expand = 0.0f, float rounding = 0.0f)
    //{
    //     ImGui::GetWindowDrawList()->AddRectFilled(
    //         ImGui::GetItemRectMin() - ImVec2(expand, expand),
    //         ImGui::GetItemRectMax() + ImVec2(expand, expand),
    //         color, rounding);
    // };

    void BuildNode(Node* node);
    Node* SpawnBegin();
    Node* SpawnTick();
    Node* SpawnInputActionNode();
    Node* SpawnBranchNode();
    Node* SpawnDoNNode();
    Node* SpawnOutputActionNode();
    Node* SpawnPrintStringNode();
    Node* SpawnMessageNode();
    Node* SpawnSetTimerNode();
    Node* SpawnLessNode();
    Node* SpawnWeirdNode();
    Node* SpawnTraceByChannelNode();
    Node* SpawnTreeSequenceNode();
    Node* SpawnTreeTaskNode();
    Node* SpawnTreeTask2Node();
    Node* SpawnComment();
    Node* SpawnHoudiniTransformNode();
    Node* SpawnHoudiniGroupNode();
    void BuildNodes();
    ImColor GetIconColor(PinType type);
    void DrawPinIcon(const Pin& pin, bool connected, int alpha);
    void ShowStyleEditor(bool* show = nullptr);
    void ShowLeftPane(float paneWidth);

public:
    CBlueprintEditor();
    virtual ~CBlueprintEditor();
};
