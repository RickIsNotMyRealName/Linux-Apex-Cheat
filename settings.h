#pragma once
#include "imgui/imgui.h"
#include "memory.h"
#include "entity.h"
#include <vector>
#include "struct.h"
#include "offsets.h"
#include "aimBotSettings.h"
#include "configManager.hpp"

namespace Settings
{
    inline Memory apex;
    inline bool active = true;
    inline bool started = false;
    inline bool showMenu = false;
    inline bool doOverlay = false;
    inline bool overlayTesting = true;
    inline bool waitingForOverlay = true;
    inline float aimVisTime[15000];
    
    // Cached state of process local entity data
	inline std::vector<sdk::CEntInfo> ent_infos;
    inline std::vector<sdk::CEntInfo> prev_infos;
    inline std::vector<Entity*> entities;
    inline std::vector<uint64_t> newEnts;
    inline std::vector<uint64_t> currentEnts;

    //inline std::vector<Entity> items;
    //inline std::vector<Entity> players;

    //aim
    inline WeaponConfig wepConf;

    inline uint64_t bestAimTarget = 0;
    inline uint64_t lastAimTarget = 0;

    //radar
    
    inline bool fullMap = false;
    inline bool debugRadar = false;
    inline Vector radarPos = {50.f, 50.f, 0.f};
    inline Vector radarSize = {240.f, 240.f, 0.f};
    inline float scale = 0.33333333f;
    inline float zoom = 0.f;
    inline bool brMap = true;
    inline bool areanaMap = false;
    inline Vector2D radarPixelOffset = {0,0};
    inline int radarPixelOffsetX = 0;
    inline int radarPixelOffsetY = 0;
    inline std::string mapName = "";



    //config
    inline config curConfig;
    inline const char* items[] = { "Aimbot", "Triggerbot", "Visuals", "Misc", "Colors" };//filled with junk on start

}