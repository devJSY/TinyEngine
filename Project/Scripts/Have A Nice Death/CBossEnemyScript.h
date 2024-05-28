#pragma once
#include "CEnemyScript.h"

enum class BOSS_TYPE
{
    LIFE,
    EVILPUMPKIN,
};

class CBossEnemyScript : public CEnemyScript
{
protected:
    void SpawnBossUI(BOSS_TYPE _Type);
    void DestroyBossUI();
    void DamageLifeBarUpdate();
    void EndBossBattle();

private:
    virtual void OnDetectTargetEnter(CGameObject* _TargetObj) override{}; // 부모 OnDetectTargetEnter 사용 X
    virtual void OnDetectTargetExit(CGameObject* _TargetObj) override{};  // 부모 OnDetectTargetExit 사용 X

public:
    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CBossEnemyScript* Clone() = 0;

public:
    CBossEnemyScript(UINT _ScriptType);
    virtual ~CBossEnemyScript();
};
