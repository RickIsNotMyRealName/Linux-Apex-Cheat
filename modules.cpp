#include "modules.h"
#include "imgui/imgui.h"
#include <thread>
#include "settings.h"
#include <GLFW/glfw3.h>
#include "offsets.h"
#include <cstdint>
#include "entity.h"
#include <iostream>
#include "struct.h"
#include <algorithm>
#include "mathUtils.h"
#include "prediction.h"
#include "aimBotSettings.h"
#include "configManager.hpp"

bool strt = false;

void Modules::StartAllModules()
{
    printf("Starting all Modules\n");
    //im not sure why i put this here but it had a purpose at one point
    while (strt == false)
    {

        printf("Starting Glow Module\n");
        std::thread glowThread = std::thread(Glow);
        glowThread.detach();

        printf("Started Ent Caching\n");
        std::thread entChachingThread = std::thread(EntChaching);
        entChachingThread.detach();

        printf("Starting No Recoil Module\n");
        std::thread NoRecoilThread = std::thread(NoRecoil);
        NoRecoilThread.detach();

        // starting best target thread
        std::thread getBestAimTargetThread = std::thread(GetBestAimTarget);
        getBestAimTargetThread.detach();

        // starting aimbot thread
        std::thread aimBotThread = std::thread(SmoothedAimBot);
        aimBotThread.detach();

        //start misc thread
        std::thread miscThread = std::thread(Misc);
        miscThread.detach();
        
        strt = true;
    }
    std::this_thread::sleep_for(std::chrono::seconds(10));
}

void Modules::GetBestAimTarget()
{
    while (Settings::active)
    {

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        uint64_t gBase = Settings::apex.get_proc_baseaddr();
        while (Settings::apex.get_proc_status() == process_status::FOUND_READY)
        {
            config curConf = Settings::curConfig;
            Settings::wepConf.updateCurrentSettings();
            uint64_t entList = gBase + ApexOffsets::cl_entitylist;

            uint64_t localEntPtr = Settings::apex.Read<uint64_t>(gBase + ApexOffsets::local_player);

            if (localEntPtr != 0)
            {
                Entity localPlayer = getEntity(localEntPtr);
                Vector localPlayerPos = localPlayer.getPosition();
                uint64_t bestTarget = 0;
                float bestTargetFov = 999.f;
                for (int i = 0; i < 15000; i++)
                {
                    uint64_t curEntPtr = Settings::apex.Read<uint64_t>(entList + ((uint64_t)i << 5));
                    if (curEntPtr == 0)
                        continue;
                    if (localEntPtr == curEntPtr)
                        continue;

                    Entity curEnt = getEntity(curEntPtr);
                    if (!curEnt.isPlayer() && !curEnt.isDummy())
                    {
                        continue;
                    }

                    // printf("1");

                    Vector entPos = curEnt.getPosition();

                    float dist = localPlayerPos.DistTo(entPos);
                    if (dist > curConf.maxAimbotDist)
                    {
                        continue;
                    }
                    if (curEnt.isKnocked() && curConf.targetKnockedEnts == false)
                    {
                        continue;
                    }

                    float fov = CalculateFov(localPlayer, curEnt);

                    if (fov > Settings::wepConf.currentSettings.aimbotFov)
                    {
                        continue;
                    }
                    if (curConf.visCheck == true && (curEnt.getLastVisTime() > Settings::aimVisTime[i]) == false)
                    {
                        continue;
                    }
                    if (fov < bestTargetFov)
                    {
                        bestTargetFov = fov;
                    }

                    Settings::aimVisTime[i] = curEnt.getLastVisTime();
                    // curEnt = curEnt.getLastVisTime();
                    bestTarget = curEntPtr;
                }
                // printf("0x%x\n", bestTarget);
                Settings::bestAimTarget = bestTarget;
            }
        }
    }
}


//credits to casual_hacker for some of this code as well as github copilot lol
void Modules::EntChaching()
{
    printf("Started Ent Caching Module\n");
    // std::vector<uint64_t> Settings::newEnts;
    //  Initialize the cached state of entities and prev_infos and ent_infos
    for (size_t i = 0; i < sdk::NUM_ENT_ENTRIES; ++i)
    {
        Settings::newEnts.push_back(0);
        Settings::ent_infos.push_back(sdk::CEntInfo{});
        Settings::prev_infos.push_back(sdk::CEntInfo{});
    }

    while (Settings::active)
    {
        int ye = 0;
        while (Settings::apex.get_proc_status() == process_status::FOUND_READY)
        {
            // printf("Started Ent Caching\n");
            uint64_t gBase = Settings::apex.get_proc_baseaddr();
            uint64_t entList = gBase + ApexOffsets::cl_entitylist;
            uint64_t localEntPtr = Settings::apex.Read<uint64_t>(gBase + ApexOffsets::local_player);

            // swap the ent_infos and prev_infos
            std::swap(Settings::ent_infos, Settings::prev_infos);

            // read entire entlist and put data into a vector of CEntInfo type
            Settings::apex.ReadArray<sdk::CEntInfo>(entList, Settings::ent_infos.data(), sdk::NUM_ENT_ENTRIES);

            // updates the entities vector with current data from ent list
            for (size_t i = 0; i < 15000; ++i)
            {
                uint64_t &entPtr = Settings::newEnts[i];
                auto &ent_info = Settings::ent_infos[i];
                auto &prev_info = Settings::prev_infos[i];

                // If the ent_infos pointer remained the same and is not null update the entitys buffer
                if ((prev_info.pEntity == ent_info.pEntity) && ent_info.pEntity != 0)
                {
                    // printf("same\n");
                    // char className[33] = {};
                    // getClassName(ent_info.pEntity, className);
                    // printf("%s\n", className);
                }
                // if the entity pointer changed to something non-null
                else if (ent_info.pEntity != 0)
                {
                    entPtr = 0;
                    Entity curEnt = getEntity(ent_info.pEntity);

                    // read classname and check if its an entity than create a new entity
                    /*                     char className[33] = {};
                                        getClassName(ent_info.pEntity, className); */
                    if (curEnt.isItem())
                    {
                        /// printf("Found Item\n");
                        entPtr = curEnt.getPtr();
                    }
                    if (curEnt.isPlayer())
                    {
                        // printf("Found Player\n");
                        entPtr = curEnt.getPtr();
                    }
                    if (curEnt.isDummy())
                    {
                        // printf("Found Dummy\n");
                        entPtr = curEnt.getPtr();
                    }
                    // printf("ent changed\n");
                    // print entity pointer
                    // printf("%x\n", ent_info.pEntity);
                    // print classname
                    // printf("%s\n", className);
                }
                // the entity became null remove from the vector
                else
                {

                    // printf("ent null\n");
                    // erase curent entity from the vector
                    entPtr = 0;
                }
            }

            std::vector<uint64_t> temp;
            for (uint64_t ptr : Settings::newEnts)
            {
                if (ptr == 0)
                {
                    continue;
                }
                temp.push_back(ptr);
                if (std::find(Settings::currentEnts.begin(), Settings::currentEnts.end(), ptr) != Settings::currentEnts.end())
                {
                }
                else
                {
                    Settings::currentEnts.push_back(ptr);
                }
            }
            // loop though currentEnts and remove any that are not in temp
            for (uint64_t ptr : Settings::currentEnts)
            {
                if (std::find(temp.begin(), temp.end(), ptr) == temp.end())
                {
                    Settings::currentEnts.erase(std::remove(Settings::currentEnts.begin(), Settings::currentEnts.end(), ptr), Settings::currentEnts.end());
                }
            }
            printf("%d\n", Settings::currentEnts.size());
        }
    }
}

void Modules::Glow()
{
    printf("Started Glow Module\n");
    while (Settings::active)
    {
        while (Settings::apex.get_proc_status() == process_status::FOUND_READY)
        {
            config curConf = Settings::curConfig;
            uint64_t entityList = Settings::apex.get_proc_baseaddr() + ApexOffsets::cl_entitylist;

            if (curConf.playerGlowEnabled || curConf.itemGlowEnabled || curConf.dummyGlowEnabled)
            {
                for (int i = 0; i < 15000; i++)
                {
                    uint64_t curEntityPtr = Settings::apex.Read<uint64_t>(entityList + ((uint64_t)i << 5));
                    if (curEntityPtr == 0)
                    {
                        continue;
                    }

                    char className[33] = {};
                    getClassName(curEntityPtr, className);
                    // std::cout << className <<"\n";
                    if (strncmp(className, "CPropSurvival", 13) == 0 && curConf.itemGlowEnabled) //&&Settings::itemGlow
                    {
                        // printf("-------------------------------------\n");
                        Item curItem = getItem(curEntityPtr);
                        if (!curItem.isGlowing())
                        {
                            curItem.enableGlow(curConf.itemColor);
                        }
                    }
                    else if (strncmp(className, "CPlayer", 7) == 0 && curConf.playerGlowEnabled)
                    {
                        Entity curPlayer = getEntity(curEntityPtr);
                        if (!curPlayer.isGlowing())
                        {
                            curPlayer.enableGlow(curConf.playerColor);
                        }
                    }
                    else if (strncmp(className, "CAI_BaseNPC", 11) == 0 && curConf.dummyGlowEnabled)
                    {
                        Entity curEntity = getEntity(curEntityPtr);
                        if (!curEntity.isGlowing())
                        {
                            curEntity.enableGlow(curConf.dummyColor);
                        }
                    }
                }
            }
        }
    }
}

void Modules::NoRecoil()
{
    QAngle oldRecoilAngle = {0, 0, 0};
    QAngle oldSwayAngles = {0, 0, 0};
    // printf("///////");
    while (Settings::active)
    {

        // printf("-------");
        while (Settings::apex.get_proc_status() == process_status::FOUND_READY)
        {
            config curConf = Settings::curConfig;
            if (curConf.rcsEnabled)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                uint64_t gBase = Settings::apex.get_proc_baseaddr();

                Settings::wepConf.updateCurrentSettings();

                uint64_t localPlayerPtr = Settings::apex.Read<uint64_t>(gBase + ApexOffsets::local_player);

                if (localPlayerPtr == 0)
                    continue;
                Entity localPlayer = getEntity(localPlayerPtr);
                QAngle CalcAngles;

                // getting original angles
                QAngle oldVAngles = localPlayer.GetViewAngles();
                CalcAngles = oldVAngles;
                // get recoil angles
                QAngle recoilAngles = localPlayer.GetRecoil();
                QAngle swayAngles = localPlayer.GetSwayAngles();

                // removing recoil angles from player angles
                CalcAngles.x = CalcAngles.x + (oldRecoilAngle.x - recoilAngles.x) * (Settings::wepConf.currentSettings.aimbotRCSX / 100.f);
                CalcAngles.y = CalcAngles.y + (oldRecoilAngle.y - recoilAngles.y) * (Settings::wepConf.currentSettings.aimbotRCSY / 100.f);

                // clamp angles so ur game doesnt crash ie. writing -200 or something
                Math::ClampAngles(CalcAngles);
                localPlayer.SetViewAngles(CalcAngles);
                // setting veiwAngles to player

                // setting old recoil angles to curenent recoil angles to the aim doesnt jump down randomly
                oldRecoilAngle = recoilAngles;
            }
        }
    }
}

// create an entity from a pointer to the entity and calculate the angles from the entity to the localPlayer than return the angles
QAngle CalculateAngles(Entity &localPlayer, uint64_t targetPtr)
{
    config curConf = Settings::curConfig;
    float curMaxFov = Settings::wepConf.currentSettings.aimbotFov;
    // printf("%f\n", Settings::wepConf.currentSettings.aimbotFov);
    Entity target = getEntity(targetPtr);

    if (!target.isAlive())
    {
        return QAngle(0, 0, 0);
    }

    Vector TargetBonePos;
    Vector localCameraPos = localPlayer.GetCamPos();

    // gets bone closest to crosshair
    if (Settings::wepConf.currentSettings.aimbotDynamicBone && localCameraPos.DistTo(target.GetCamPos()) < 2500.f)
    {
        double bestFov = 999999999999.f;
        int bestBone = curConf.bone;
        for (int i = 2; i < 11; i++)
        {
            double curBoneFov = CalculateBoneFov(localPlayer, target, i);
            if (curBoneFov < bestFov)
            {
                bestFov = curBoneFov;
                bestBone = i;
            }
        }
        TargetBonePos = target.getBonePos(bestBone);
    }
    else
    {
        TargetBonePos = target.getBonePos(curConf.bone);
    }

    QAngle calculatedAngles = QAngle(0, 0, 0);

    WeaponXEntity curWeap = WeaponXEntity();
    curWeap.update(localPlayer.ptr);

    float bulletSpeed = curWeap.get_projectile_speed();
    float bulletGravity = curWeap.get_projectile_gravity();
    float zoomFov = curWeap.get_zoom_fov();

    if (zoomFov != 0.f && zoomFov != 0.f)
    {
        curMaxFov *= zoomFov / 90.f;
    }

    if (bulletSpeed > 1.f) // try checking what happens if i set this to true
    {
        PredictCtx ctx;
        ctx.StartPos = localCameraPos;
        ctx.TargetPos = TargetBonePos;
        ctx.BulletSpeed = bulletSpeed;     // might have error?
        ctx.BulletGravity = bulletGravity; // also might have error?
        ctx.TargetVel = target.getAbsVelocity();

        if (BulletPredict(ctx))
        {
            calculatedAngles = QAngle(ctx.AimAngles.x, ctx.AimAngles.y, 0);
        }
    }
    if (calculatedAngles == QAngle(0, 0, 0))
    {
        calculatedAngles = Math::CalcAngle(localCameraPos, TargetBonePos);
    }

    QAngle curViewAngles = localPlayer.GetViewAngles();

    Math::ClampAngles(calculatedAngles);

    // finish pasting this lol

    QAngle delta = calculatedAngles - curViewAngles;

    Math::ClampAngles(delta);

    QAngle finalAngles = curViewAngles + delta / Settings::wepConf.currentSettings.aimbotSmooth;

    Math::ClampAngles(finalAngles);
    return finalAngles;
}

void Modules::SmoothedAimBot()
{
    while (Settings::active)
    {
        while (Settings::apex.get_proc_status() == process_status::FOUND_READY)
        {
            config curConf = Settings::curConfig;
            if (curConf.aimbotEnabled)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
                // printf("Aimbot\n");
                if (Settings::bestAimTarget == 0)
                {
                    continue;
                }

                uint64_t localPlayerPtr = Settings::apex.Read<uint64_t>(Settings::apex.get_proc_baseaddr() + ApexOffsets::local_player);
                if (localPlayerPtr == 0)
                {
                    continue;
                }

                Entity localPlayer = getEntity(localPlayerPtr);
                QAngle angles = QAngle(0, 0, 0);
                if (localPlayer.isZooming())
                {
                    angles = CalculateAngles(localPlayer, Settings::bestAimTarget);
                }

                if (angles.x == 0 && angles.y == 0 && angles.z == 0)
                {
                    continue;
                }
                // printf("1");
                localPlayer.SetViewAngles(angles);
            }
        }
    }
}

void Modules::Misc()
{
    while (Settings::active)
    {
        while (Settings::apex.get_proc_status() == process_status::FOUND_READY)
        {
            uint64_t gBase = Settings::apex.get_proc_baseaddr();
            uint64_t localPlayerPtr = Settings::apex.Read<uint64_t>(gBase + ApexOffsets::local_player);
            if (localPlayerPtr == 0)
            {
                continue;
            }
            Entity localPlayer = getEntity(localPlayerPtr);

            if(Settings::curConfig.forceObserverMode)
            {
                Settings::apex.Write<int>(localPlayer.ptr + ApexOffsets::OFFSET_OBSERVER_MODE, Settings::curConfig.observerMode);
            }
        }
    }
    
}