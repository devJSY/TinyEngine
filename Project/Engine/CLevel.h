#pragma once
#include "CEntity.h"
#include "CLayer.h"
#include "CGameObject.h"

class CLevel : public CEntity
{
private:
    CLayer* m_arrLayer[LAYER_MAX];
    LEVEL_STATE m_State;
    int m_StepFrames;

public:
    virtual void begin();
    virtual void tick();
    virtual void finaltick();

    // _bChildMove : 오브젝트의 자식오브젝트도 같은 레이어에 넣을지 여부
    void AddObject(CGameObject* _Object, int _LayerIdx, bool _bChildMove = true);
    void AddObject(CGameObject* _Object, const wstring& _strLayerName, bool _bChildMove = true);

    CLayer* GetLayer(int _iLayerIdx) const { return m_arrLayer[_iLayerIdx]; }
    CLayer* GetLayer(const wstring& _strLayerName) const;

public:
    void ChangeState(LEVEL_STATE _NextState);
    LEVEL_STATE GetState() const { return m_State; }
    void Step(int _Frames = 1);

public:
    CGameObject* FindObjectByName(const wstring& _strName);

    template <typename T>
    CGameObject* FindObjectOfType();

    template <typename T>
    vector<CGameObject*> FindObjectsOfType();

    int FindLayerIndexByName(const wstring& _strName);
    wstring FindLayerNameByIndex(int _Index);

private:
    void clear();

    CLONE(CLevel);

public:
    CLevel();
    CLevel(const CLevel& origin);
    virtual ~CLevel();

    friend class CLevelMgr;
};

template <typename T>
inline CGameObject* CLevel::FindObjectOfType()
{
    for (UINT i = 0; i < LAYER_MAX; ++i)
    {
        if (nullptr == m_arrLayer[i])
            continue;

        const vector<CGameObject*>& vecParent = m_arrLayer[i]->GetParentObjects();

        for (size_t j = 0; j < vecParent.size(); ++j)
        {
            list<CGameObject*> queue;
            queue.push_back(vecParent[j]);

            while (!queue.empty())
            {
                CGameObject* pObject = queue.front();
                queue.pop_front();

                if (nullptr == pObject)
                    continue;

                const vector<CGameObject*>& vecChild = pObject->GetChildObject();
                for (size_t k = 0; k < vecChild.size(); ++k)
                {
                    queue.push_back(vecChild[k]);
                }

                if (nullptr != pObject->GetComponent<T>() || nullptr != pObject->GetScript<T>())
                {
                    return pObject;
                }
            }
        }
    }

    return nullptr;
}

template <typename T>
inline vector<CGameObject*> CLevel::FindObjectsOfType()
{
    vector<CGameObject*> vecObj;

    for (UINT i = 0; i < LAYER_MAX; ++i)
    {
        if (nullptr == m_arrLayer[i])
            continue;

        const vector<CGameObject*>& vecParent = m_arrLayer[i]->GetParentObjects();

        for (size_t j = 0; j < vecParent.size(); ++j)
        {
            list<CGameObject*> queue;
            queue.push_back(vecParent[j]);

            while (!queue.empty())
            {
                CGameObject* pObject = queue.front();
                queue.pop_front();

                if (nullptr == pObject)
                    continue;

                const vector<CGameObject*>& vecChild = pObject->GetChildObject();
                for (size_t k = 0; k < vecChild.size(); ++k)
                {
                    queue.push_back(vecChild[k]);
                }

                if (nullptr != pObject->GetComponent<T>() || nullptr != pObject->GetScript<T>())
                {
                    vecObj.push_back(pObject);
                }
            }
        }
    }

    return vecObj;
}
