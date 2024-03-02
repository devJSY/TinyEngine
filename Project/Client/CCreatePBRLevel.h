#pragma once

class CLevel;

class CCreatePBRLevel
{
public:
    static void Init();
    static void CreatePBRLevel();

private:
    static void AddSample(CLevel* _CurLevel);
    static void AddModels(CLevel* _CurLevel);
};
