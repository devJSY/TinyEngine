#pragma once
#pragma once
#include "CLevel.h"

class CPBRLevel : public CLevel
{
private:
    void AddSample();
    void AddModels();

public:
    CPBRLevel();
    virtual ~CPBRLevel();
};
