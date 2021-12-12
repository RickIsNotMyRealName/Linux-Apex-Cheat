#pragma once

//need to optimize bullet prediction and ent position prediction for aimbot
namespace Modules
{
    void StartAllModules();
    void SmoothedAimBot(); //not sure if i should do silent aim and smoothed in the same thread
    //sometimes jumps down i did fix it mostly but its not perfect
    void NoRecoil();
    //need to update this to work with ent caching
    void GetBestAimTarget();
    void ItemEsp();
    void Glow();
    void EntChaching();
    void Misc();
}