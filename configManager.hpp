#pragma once
#include <vector>
#include <string>
#include "entity.h"

struct configSettings
{
    std::string filename;
    std::string filePath;
    int value;
    bool selected;
};


class config
{
private:

public:
    int currentConfigNumber = 0;
    std::vector<configSettings> configs;
        
    void loadConfig(int currentConfig);
    void saveConfig(int currentConfig);

    //aimbot
    bool aimbotEnabled = true;
    bool rcsEnabled =true;
    float rcsX = 100.f;
    float rcsY = 100.f;
    bool visCheck = true;
    float aimbotFov = 15.f;
    float aimbotSmooth = 150.f;
    float maxAimbotDist = 100.f;
    bool targetKnockedEnts;
    int bone = 7;
    bool dynamicBone = true;
    bool friendlyFire = false;
    
    //radar
    bool radarEnabled = true;
    int radarType = 1;
    int radarBlipSize = 1;
    bool enemyOnly;//does nothing rn
    
    //esp
    bool espEnabled = false;
    float maxEspDist;
    bool boxEnabled = true;
    bool nameEnabled = true;
    bool healthEnabled = true;
    bool weaponEnabled;
    bool armorEnabled;
    bool lineEnabled = true;

    //glow
    bool playerGlowEnabled;
    bool itemGlowEnabled;
    bool dummyGlowEnabled;
    bool debugGlowEnabled;
    float itemColor[3];
    float playerColor[3];
    float dummyColor[3];
    GlowMode glowMode = {-122,-125,127,0};

    //misc
    bool forceObserverMode = false;
    int observerMode = 0;




    //item esp
    bool itemEspEnabled;
    float itemEspDist;
};




