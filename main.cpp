#include "memory.h"
#include <thread>
#include "overlay.h"
#include "modules.h"
#include <unistd.h>
#include <stdio.h>
#include "settings.h"
#include <filesystem>
#include "configManager.hpp"

const char *apex_proc = "R5Apex.exe";
const char *EAC_proc = "EasyAntiCheat_launcher.exe";
//this should be a global variable but i'm lazy and didnt make a fucntion to update it and check if the process is still running or something
uint64_t gBase = 0;


/*
This is my apex cheat its not meant to be actually used but its more of a POC and something i can release and get some feedback. 
Im not sure how i should structure my code. I also would like to improve my preformance and make it more efficient.
I dont think it is that preformant rn.( especially my rendering code ), ( mainly the esp and radar )
My ent caching works fine in the firing range but im havnt properly tested it in br.
The ent caching has some code at the end for ensureing ents arent cached twice as well as removing old ents.
The item is is quite clutered and i would like to improve that and im not sure how at the moment.
I do rather like the way the default glow looks.
The cpu usage is quite high and i would like to improve that as well. Im not sure how to do that or whats causing it. ( maybe i need to reduce amounts of reads and writes not sure where tho )
The GLFW library on arch was outdated so i had to manually include it in cmake and i probably didnt do that the best as ive never used cmake before this project. ( or GLFW for that matter ) 
I also should add a catch for when vm is not running. Should just loop till vm is running.
There are for sure some bugs somewhere.
*/
int main(int argc, char **argv[])
{
	if (geteuid() != 0)
	{
		printf("Error: %s is not running as root\n", argv[0]);
		// return 5;
	}

	//thanks to github copilot for some of this code lol
	char tmp[256];
	getcwd(tmp, 256);
	// read all ini files in current directory and add them to Settings::configs vector
	std::string path = tmp;
	path = path + "/configs/";
	int i = 0;
	int z = 0;
	printf("Loading configs from %s\n", path.c_str());
	for (const auto &entry : std::filesystem::directory_iterator(path))
	{
		if (entry.path().extension() == ".ini")
		{
			configSettings tmp;
			if(entry.path().filename().string() == "default.ini")
			{
				tmp =
				{
					tmp.filename = entry.path().filename().string(),
					tmp.filePath = entry.path().string(),
					tmp.value = z,
					true};
				Settings::curConfig.currentConfigNumber = z;
			}
			else
			{
				tmp =
				{
					tmp.filename = entry.path().filename().string(),
					tmp.filePath = entry.path().string(),
					tmp.value = z,
					false};
			}
			Settings::curConfig.configs.push_back(tmp);
			z++;
		}
		i++;
	}

	std::thread(Overlay::RenderLoop).detach();
	// printf("Overlay Started\n");

	// add items from Settings::curConfig.configs to Settings::curConfig.items
	/* 	for(int i = 0; i < Settings::curConfig.configs.size(); i++)
		{
			Settings::curConfig.items[i] = Settings::curConfig.configs[i].filename.c_str();
		} */

	printf("Loaded %d configs\n", z);


	std::this_thread::sleep_for(std::chrono::seconds(1));

	std::thread(Modules::StartAllModules).detach();
	printf("Modules Started\n");
	//should i do this better? idek
	while (Settings::active)
	{

		if (Settings::apex.get_proc_status() != process_status::FOUND_READY)
		{

			if (Settings::started)
			{
				Settings::started = true;
			}

			std::this_thread::sleep_for(std::chrono::seconds(1));
			printf("Searching for apex process...\n");

			Settings::apex.open_proc(apex_proc);

			if (Settings::apex.get_proc_status() == process_status::FOUND_READY)
			{
				gBase = Settings::apex.get_proc_baseaddr();
				printf("\nApex process found\n");
				printf("Base: %lx\n", gBase);
			}
		}
		else
		{
			// check_process state
			Settings::apex.check_proc();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
	}
}
