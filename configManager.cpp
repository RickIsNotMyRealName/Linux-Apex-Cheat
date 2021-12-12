#include "configManager.hpp"
#include "SimpleIni.h"
#include <iostream>
#include <string>


void config::loadConfig(int currentConfig)
{
    CSimpleIniA ini;
    ini.SetUnicode();
    currentConfigNumber = currentConfig;
    for(auto& config : configs)
    {
        if (config.value == currentConfig )
        {
            //print loading message
            printf("Loading config...\n");
            SI_Error rc = ini.LoadFile(config.filePath.c_str());
            if (rc < 0)
            {
                std::cout << "Error loading config file: " << config.filePath << std::endl;
                return;
            }
            //this was a pain in the ass to write i missspelled some of the section names and spent an hour or two trying to figure out why it wasnt working lol
            //the writing worked great first time but reading the values was fucked lol
            //aimbot
            aimbotEnabled = ini.GetBoolValue("Aimbot", "enabled");
            rcsEnabled = ini.GetBoolValue("Aimbot", "rcsEnabled");
            rcsX = ini.GetDoubleValue("Aimbot", "rcsX");
            rcsY = ini.GetDoubleValue("Aimbot", "rcsY");
            visCheck = ini.GetBoolValue("Aimbot", "visCheck");
            aimbotFov = ini.GetDoubleValue("Aimbot", "aimbotFov");
            aimbotSmooth = ini.GetDoubleValue("Aimbot", "aimbotSmooth");
            maxAimbotDist = ini.GetDoubleValue("Aimbot", "maxAimbotDist");
            targetKnockedEnts = ini.GetBoolValue("Aimbot", "targetKnockedEnts");     
            bone = ini.GetLongValue("Aimbot", "bone");
            dynamicBone = ini.GetBoolValue("Aimbot", "dynamicBone");
            friendlyFire = ini.GetBoolValue("Aimbot", "friendlyFire");
            //radar
            radarEnabled = ini.GetBoolValue("Radar", "enabled");
            radarType = ini.GetLongValue("Radar", "radarType");
            radarBlipSize = ini.GetLongValue("Radar", "radarBlipSize");
            enemyOnly = ini.GetBoolValue("Radar", "enemyOnly");
            //esp
            espEnabled = ini.GetBoolValue("ESP", "enabled");
            maxEspDist = ini.GetDoubleValue("ESP", "maxEspDist");
            boxEnabled = ini.GetBoolValue("ESP", "boxEnabled");
            nameEnabled = ini.GetBoolValue("ESP", "nameEnabled");
            healthEnabled = ini.GetBoolValue("ESP", "healthEnabled");
            weaponEnabled = ini.GetBoolValue("ESP", "weaponEnabled");
            armorEnabled = ini.GetBoolValue("ESP", "armorEnabled");
            lineEnabled = ini.GetBoolValue("ESP", "lineEnabled");
            //glow
            playerGlowEnabled = ini.GetBoolValue("Glow", "playerGlowEnabled");
            itemGlowEnabled = ini.GetBoolValue("Glow", "itemGlowEnabled");
            dummyGlowEnabled = ini.GetBoolValue("Glow", "dummyGlowEnabled");
            itemColor[0] = ini.GetDoubleValue("Glow", "itemColorR");
            itemColor[1] = ini.GetDoubleValue("Glow", "itemColorG");
            itemColor[2] = ini.GetDoubleValue("Glow", "itemColorB");
            itemColor[3] = ini.GetDoubleValue("Glow", "itemColorA");
            playerColor[0] = ini.GetDoubleValue("Glow", "playerColorR");
            playerColor[1] = ini.GetDoubleValue("Glow", "playerColorG");
            playerColor[2] = ini.GetDoubleValue("Glow", "playerColorB");
            playerColor[3] = ini.GetDoubleValue("Glow", "playerColorA");
            dummyColor[0] = ini.GetDoubleValue("Glow", "dummyColorR");
            dummyColor[1] = ini.GetDoubleValue("Glow", "dummyColorG");
            dummyColor[2] = ini.GetDoubleValue("Glow", "dummyColorB");
            dummyColor[3] = ini.GetDoubleValue("Glow", "dummyColorA");
            glowMode.GeneralGlowMode = ini.GetLongValue("Glow", "generalGlowMode");
            glowMode.BorderGlowMode = ini.GetLongValue("Glow", "borderGlowMode");
            glowMode.BorderSize = ini.GetLongValue("Glow", "borderSize");
            glowMode.TransparentLevel = ini.GetLongValue("Glow", "transparency");
            //item ESP
            itemEspEnabled = ini.GetBoolValue("Item ESP", "enabled");
            itemEspDist = ini.GetDoubleValue("Item ESP", "maxItemEspDist");
            printf("Config loaded!\n");
        }
        
    }
    
}


void config::saveConfig(int currentConfig)
{
    CSimpleIniA ini;
    ini.SetUnicode();
    currentConfigNumber = currentConfig;
    for(auto& config : configs)
    {
        if (config.value == currentConfig )
        {
            SI_Error rc = ini.LoadFile(config.filePath.c_str());
            if (rc < 0)
            {
                std::cout << "Error loading config file: " << config.filePath << std::endl;
                return;
            }
            //aimbot

            ini.SetBoolValue("Aimbot","enabled",aimbotEnabled);
            ini.SetBoolValue("Aimbot","rcsEnabled",rcsEnabled);
            ini.SetDoubleValue("Aimbot","rcsX",rcsX);
            ini.SetDoubleValue("Aimbot","rcsY",rcsY);
            ini.SetBoolValue("Aimbot","visCheck",visCheck);
            ini.SetDoubleValue("Aimbot","aimbotFov",aimbotFov);
            ini.SetDoubleValue("Aimbot","aimbotSmooth",aimbotSmooth);
            ini.SetDoubleValue("Aimbot","maxAimbotDist",maxAimbotDist);
            ini.SetBoolValue("Aimbot","targetKnockedEnts",targetKnockedEnts);
            ini.SetLongValue("Aimbot","bone",bone);
            ini.SetBoolValue("Aimbot","dynamicBone",dynamicBone);
            ini.SetBoolValue("Aimbot","friendlyFire",friendlyFire);
            ini.SetBoolValue("Radar","enabled",radarEnabled);
            ini.SetLongValue("Radar","radarType",radarType);
            ini.SetLongValue("Radar","radarBlipSize",radarBlipSize);
            ini.SetBoolValue("Radar","enemyOnly",enemyOnly);
            ini.SetBoolValue("ESP","enabled",espEnabled);
            ini.SetDoubleValue("ESP","maxEspDist",maxEspDist);
            ini.SetBoolValue("ESP","boxEnabled",boxEnabled);
            ini.SetBoolValue("ESP","nameEnabled",nameEnabled);
            ini.SetBoolValue("ESP","healthEnabled",healthEnabled);
            ini.SetBoolValue("ESP","weaponEnabled",weaponEnabled);
            ini.SetBoolValue("ESP","armorEnabled",armorEnabled);
            ini.SetBoolValue("ESP","lineEnabled",lineEnabled);
            ini.SetBoolValue("Glow","playerGlowEnabled",playerGlowEnabled);
            ini.SetBoolValue("Glow","itemGlowEnabled",itemGlowEnabled);
            ini.SetBoolValue("Glow","dummyGlowEnabled",dummyGlowEnabled);
            ini.SetDoubleValue("Glow","itemColorR",itemColor[0]);
            ini.SetDoubleValue("Glow","itemColorG",itemColor[1]);
            ini.SetDoubleValue("Glow","itemColorB",itemColor[2]);
            ini.SetDoubleValue("Glow","itemColorA",itemColor[3]);
            ini.SetDoubleValue("Glow","playerColorR",playerColor[0]);
            ini.SetDoubleValue("Glow","playerColorG",playerColor[1]);
            ini.SetDoubleValue("Glow","playerColorB",playerColor[2]);
            ini.SetDoubleValue("Glow","playerColorA",playerColor[3]);
            ini.SetDoubleValue("Glow","dummyColorR",dummyColor[0]);
            ini.SetDoubleValue("Glow","dummyColorG",dummyColor[1]);
            ini.SetDoubleValue("Glow","dummyColorB",dummyColor[2]);
            ini.SetDoubleValue("Glow","dummyColorA",dummyColor[3]);
            ini.SetDoubleValue("Glow","generalGlowMode",glowMode.GeneralGlowMode);
            ini.SetDoubleValue("Glow","borderGlowMode",glowMode.BorderGlowMode);
            ini.SetDoubleValue("Glow","borderSize",glowMode.BorderSize);
            ini.SetDoubleValue("Glow","transparency",glowMode.TransparentLevel);
            ini.SetBoolValue("Item ESP","enabled",itemEspEnabled);
            ini.SetDoubleValue("Item ESP","maxItemEspDist",itemEspDist);

            std::string data;
            rc = ini.Save(data);
            if(rc < 0)
            {
                std::cout << "Error saving config file: " << config.filePath << std::endl;
            }
            rc = ini.SaveFile(config.filePath.c_str()); //save file
            if(rc < 0)
            {
                std::cout << "Error saving config file: " << config.filePath << std::endl;
            }
            
        }
        
    }
    
}