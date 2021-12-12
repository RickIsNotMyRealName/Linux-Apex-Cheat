#pragma once


//thers probably a better way to do this but i'm lazy and dumb lol
struct SMGSettings
{
    float fov = 15.f;
    float smooth = 150.f;
    float hitchance = 100.f;
    float RCSX = 100;
    float RCSY = 100;
    bool enabled = true;
    bool silent = false;
    bool friendly = false;
    int bone = 2;
    bool dynamicBone = true;
};
struct ARSettings
{
    float fov = 15.f;
    float smooth = 150.f;
    float hitchance = 100.f;
    float RCSX = 100;
    float RCSY = 100;
    bool enabled = true;
    bool silent = false;
    bool friendly = false;
    int bone = 2;
    bool dynamicBone = true;
};
struct SniperSettings
{
    float fov = 15.f;
    float smooth = 150.f;
    float hitchance = 100.f;
    float RCSX = 100;
    float RCSY = 100;
    bool enabled = true;
    bool silent = false;
    bool friendly = false;
    int bone = 8;
    bool dynamicBone = false;
};
struct PistolSettings
{
    float fov = 15.f;
    float smooth = 150.f;
    float hitchance = 100.f;
    float RCSX = 100;
    float RCSY = 100;
    bool enabled = true;
    bool silent = false;
    bool friendly = false;
    int bone = 7;
    bool dynamicBone = true;
};
struct MarksmanSettings
{
    float fov = 15.f;
    float smooth = 150.f;
    float hitchance = 100.f;
    float RCSX = 100;
    float RCSY = 100;
    bool enabled = true;
    bool silent = false;
    bool friendly = false;
    int bone = 7;
    bool dynamicBone = true;
};
struct ShotgunSettings
{
    float fov = 15.f;
    float smooth = 150.f;
    float hitchance = 100.f;
    float RCSX = 100;
    float RCSY = 100;
    bool enabled = true;
    bool silent = false;
    bool friendly = false;
    int bone = 7;
    bool dynamicBone = true;
};
struct LMGSettings
{
    float fov = 15.f;
    float smooth = 150.f;
    float hitchance = 100.f;
    float RCSX = 100;
    float RCSY = 100;
    bool enabled = true;
    bool silent = false;
    bool friendly = false;
    int bone = 2;
    bool dynamicBone = false;
};
struct CurrentSettings
{
    float aimbotFov = 15.f;
    float aimbotSmooth = 150.f;
    float aimbotHitchance = 100.f;
    float aimbotRCSX = 100;
    float aimbotRCSY = 100;
    bool enabled = true;
    bool aimbotSilent = false;
    bool aimbotFriendly = false;
    int aimbotBone = 2;
    bool aimbotDynamicBone = true;
};

class WeaponConfig
{

    
public:
    WeaponConfig() {this->updateCurrentSettings(); }
    CurrentSettings currentSettings;
    SMGSettings smg;
    LMGSettings lmg;
    ARSettings ar;
    SniperSettings sniper;
    PistolSettings pistol;
    MarksmanSettings marksman;
    ShotgunSettings shotgun;

    void updateCurrentSettings();
    CurrentSettings getCurrentSettings();

};

