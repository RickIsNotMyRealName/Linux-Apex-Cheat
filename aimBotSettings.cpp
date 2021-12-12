#include "aimBotSettings.h"
#include "entity.h"
#include "offsets.h"
#include "settings.h"
#include <iostream>

// 1 =  ar, 2 = SMG, 3 = LMG, 4 = Marksman, 5 = Sniper, 6 = Shotgun, 7 = Pistol
CurrentSettings WeaponConfig::getCurrentSettings()
{
    if (Settings::apex.get_proc_status() == process_status::FOUND_READY)
    {
        config curConf = Settings::curConfig;
        CurrentSettings settings = Settings::wepConf.currentSettings;

        uint64_t gBase = Settings::apex.get_proc_baseaddr();
        uint64_t localEntPtr = Settings::apex.Read<uint64_t>(gBase + ApexOffsets::local_player);

        Entity localPlayer = Entity(localEntPtr);

        WeaponXEntity curWeap = WeaponXEntity();
        curWeap.update(localPlayer.ptr);
        int weaponId = curWeap.get_m_weaponNameIndex();
        //printf("Weapon ID: %d\n", weaponId);
        // set current weapon settings based on weapon category
        int cat = 0;
        try
        {
            cat = CatagoryList.at(weaponId);
            if(cat == -1)
            {
                printf("Error: Weapon ID not found in CatagoryList\n");
                printf("Weapon ID: %d\n", weaponId);
            }
        }
        catch(const std::exception& e)
        {
            //getting and error here sometimes 
            cat = -1;
            printf("Weapon ID: %d\n", weaponId);
            std::cerr << e.what() << '\n';
        }
        
       
        if(cat == 1)
        {
            settings.enabled = ar.enabled;
            settings.aimbotFov = ar.fov;
            settings.aimbotSmooth = ar.smooth;
            settings.aimbotSilent = ar.silent;
            settings.aimbotRCSX = ar.RCSX;
            settings.aimbotRCSY = ar.RCSY;
            settings.aimbotHitchance = ar.hitchance;
            settings.aimbotFriendly = ar.friendly;
            settings.aimbotBone = ar.bone;
            settings.aimbotDynamicBone = ar.dynamicBone;
            //printf("ar\n");
        }
        else if(cat == 2)
        {
            settings.enabled = smg.enabled;
            settings.aimbotFov = smg.fov;
            settings.aimbotSmooth = smg.smooth;
            settings.aimbotSilent = smg.silent;
            settings.aimbotRCSX = smg.RCSX;
            settings.aimbotRCSY = smg.RCSY;
            settings.aimbotHitchance = smg.hitchance;
            settings.aimbotFriendly = smg.friendly;
            settings.aimbotBone = smg.bone;
            settings.aimbotDynamicBone = smg.dynamicBone;
            //printf("smg\n");
        }
        else if(cat == 3)
        {
            settings.enabled = lmg.enabled;
            settings.aimbotFov = lmg.fov;
            settings.aimbotSmooth = lmg.smooth;
            settings.aimbotSilent = lmg.silent;
            settings.aimbotRCSX = lmg.RCSX;
            settings.aimbotRCSY = lmg.RCSY;
            settings.aimbotHitchance = lmg.hitchance;
            settings.aimbotFriendly = lmg.friendly;
            settings.aimbotBone = lmg.bone;
            settings.aimbotDynamicBone = lmg.dynamicBone;
            //printf("lmg\n");
        }
        else if(cat == 4)
        {
            settings.enabled = marksman.enabled;
            settings.aimbotFov = marksman.fov;
            settings.aimbotSmooth = marksman.smooth;
            settings.aimbotSilent = marksman.silent;
            settings.aimbotRCSX = marksman.RCSX;
            settings.aimbotRCSY = marksman.RCSY;
            settings.aimbotHitchance = marksman.hitchance;
            settings.aimbotFriendly = marksman.friendly;
            settings.aimbotBone = marksman.bone;
            settings.aimbotDynamicBone = marksman.dynamicBone;
            //printf("marksman\n");
        }
        else if(cat == 5)
        {
            settings.enabled = sniper.enabled;
            settings.aimbotFov = sniper.fov;
            settings.aimbotSmooth = sniper.smooth;
            settings.aimbotSilent = sniper.silent;
            settings.aimbotRCSX = sniper.RCSX;
            settings.aimbotRCSY = sniper.RCSY;
            settings.aimbotHitchance = sniper.hitchance;
            settings.aimbotFriendly = sniper.friendly;
            settings.aimbotBone = sniper.bone;
            settings.aimbotDynamicBone = sniper.dynamicBone;
            //printf("sniper\n");
        }
        else if(cat == 6)
        {
            settings.enabled = shotgun.enabled;
            settings.aimbotFov = shotgun.fov;
            settings.aimbotSmooth = shotgun.smooth;
            settings.aimbotSilent = shotgun.silent;
            settings.aimbotRCSX = shotgun.RCSX;
            settings.aimbotRCSY = shotgun.RCSY;
            settings.aimbotHitchance = shotgun.hitchance;
            settings.aimbotFriendly = shotgun.friendly;
            settings.aimbotBone = shotgun.bone;
            settings.aimbotDynamicBone = shotgun.dynamicBone;
            //printf("shotgun\n");
        }
        else if(cat == 7)
        {
            settings.enabled = pistol.enabled;
            settings.aimbotFov = pistol.fov;
            settings.aimbotSmooth = pistol.smooth;
            settings.aimbotSilent = pistol.silent;
            settings.aimbotRCSX = pistol.RCSX;
            settings.aimbotRCSY = pistol.RCSY;
            settings.aimbotHitchance = pistol.hitchance;
            settings.aimbotFriendly = pistol.friendly;
            settings.aimbotBone = pistol.bone;
            settings.aimbotDynamicBone = pistol.dynamicBone;
            //printf("Pistol\n");
        }
        else
        {
            settings.enabled = curConf.aimbotEnabled;
            settings.aimbotFov = curConf.aimbotFov;
            settings.aimbotSmooth = curConf.aimbotSmooth;
            settings.aimbotSilent = false;
            settings.aimbotRCSX = curConf.rcsX;
            settings.aimbotRCSY = curConf.rcsY;
            settings.aimbotHitchance = 100;
            settings.aimbotFriendly = curConf.friendlyFire;
            settings.aimbotBone = curConf.bone;
            settings.aimbotDynamicBone = curConf.dynamicBone;
            //printf("FIX ERROR Case\n");
        }
        if (settings.enabled == false || cat == -1)
        {
            settings.enabled = curConf.aimbotEnabled;
            settings.aimbotFov = curConf.aimbotFov;
            settings.aimbotSmooth = curConf.aimbotSmooth;
            settings.aimbotSilent = false;
            settings.aimbotRCSX = curConf.rcsX;
            settings.aimbotRCSY = curConf.rcsY;
            settings.aimbotHitchance = 100;
            settings.aimbotFriendly = curConf.friendlyFire;
            settings.aimbotBone = curConf.bone;
            settings.aimbotDynamicBone = curConf.dynamicBone;
            //printf("[Aimbot] No weapon found, defaults set\n");
            return settings;
        }
        else
        {
            //printf("[Aimbot] Weapon found, settings loaded\n");
            return settings;
        }
    }
    else
    {
        return currentSettings;
    }
}

void WeaponConfig::updateCurrentSettings()
{
    currentSettings = getCurrentSettings();
}