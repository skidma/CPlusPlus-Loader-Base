// #information
// [+]  this is only for beginners who know practically nothing at all in C++ so i tried to make a simple as possible.
// [+]  with that being said, don't try to sell this. currently is using public VERY known drivers that probably won't 
// [+]  get you off of the battle bus although the permanent spoofing may still work i'm not really sure you can give 
// [+]  it a try if you want.

// #Notes
// [>]  this was made in less than an hour so expect issues i didn't really do any testing
// [>]  if you're having trouble contact me on tele @systembootloop
// [>]  i'll also update this source frequently and will create an archive for different versions in the future.

// #Source
// [*]  Source Version -> [v1.2]
// [*]  Last Updated -> [10/30/2025]
// [*]  Created by Themida

// #Loader
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <thread>
#include "includes.hpp"
#include "drv.hpp"
#include <windows.h>
#include <wininet.h>
#include <fstream>
#include <filesystem>
#include <vector>
#include <wtypes.h>
#include <winreg.h>
#include <shlobj.h>
#pragma comment(lib, "wininet.lib")
#pragma comment(lib, "srclient.lib")
#pragma comment(lib, "wbemuuid.lib")
namespace fs = std::filesystem;
using namespace std;
void crsr() { HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE); CONSOLE_CURSOR_INFO cursorInfo; GetConsoleCursorInfo(hConsole, &cursorInfo); cursorInfo.bVisible = false; SetConsoleCursorInfo(hConsole, &cursorInfo); }
void indetion() {    cout << (E("\n\n\n\n")); }
int main(DWORD CEvent)
{
Authenticate:
    // you can mess with anything here, as it is only the menu.
    // also this is just a basic license system as you should not be selling this, but if for whatever reason
    // you wanna switch to keyauth or something like that you can.
    std::string user_license;
    crsr(), indetion();
    LI_FN(SetConsoleTitleA)(E(("Loader Base"))); // change to ur loader name
    std::cout << dye::aqua((char*)E("\n  [")), std::cout << E("DEBUG"), std::cout << dye::aqua((char*)E("] ")), std::cout << (E("Loader Base"));
    std::cout << dye::aqua((char*)E("\n  [")), std::cout << E("DEBUG"), std::cout << dye::aqua((char*)E("] ")), std::cout << (E("Loader Version (v1.1.3)")), LI_FN(Sleep)(1500);
    std::cout << dye::aqua((char*)E("\n  [")), std::cout << E("DEBUG"), std::cout << dye::aqua((char*)E("] ")), std::cout << (E("Build Version (dFgHjKlMnP)"));
    std::cout << dye::aqua((char*)E("\n  [")), std::cout << E("DEBUG"), std::cout << dye::aqua((char*)E("] ")), std::cout << (E("License: ")), std:: cin >> user_license;
    if (user_license == "d9oz02p6n5xm7u23") { // license
        std::cout << dye::aqua((char*)E("\n  [")), std::cout << E("DEBUG"), std::cout << dye::aqua((char*)E("] ")), std::cout << dye::green((char*)E("Valid License."));
        LI_FN(Sleep)(1600);
        goto Base;
    }
    else {
        std::cout << dye::aqua((char*)E("\n  [")), std::cout << E("DEBUG"), std::cout << dye::aqua((char*)E("] ")), std::cout << dye::red((char*)E("Invalid License."));
        LI_FN(Sleep)(1200), std::cout << dye::aqua((char*)E("\n  [")), std::cout << E("DEBUG"), std::cout << dye::aqua((char*)E("] ")), std::cout << (E("If You're Having Issues, Join the ")), std::cout << dye::light_blue((char*)E("Discord")), std::cout << (E("For Help."));
        LI_FN(Sleep)(2500), LI_FN(system)(E("cls"));
    }
Base:
    // be careful in this section, unless ur wanting to add ur own driver, spoofing & cleaning will take place below.
    // if you're trying to replace driver with bytes visit drv.hpp there is a more detailed explaination there aswell.
    LI_FN(system)(E("cls"));
    indetion();
    std::cout << dye::aqua((char*)E("\n  [")), std::cout << E("DEBUG"), std::cout << dye::aqua((char*)E("] ")), std::cout << (E("Loader Base"));
    std::cout << dye::aqua((char*)E("\n  [")), std::cout << E("DEBUG"), std::cout << dye::aqua((char*)E("] ")), std::cout << (E("Loader Version (v1.1.3)")), LI_FN(Sleep)(1500);
    std::cout << dye::aqua((char*)E("\n  [")), std::cout << E("DEBUG"), std::cout << dye::aqua((char*)E("] ")), std::cout << (E("Build Version (dFgHjKlMnP)\n"));

    std::cout << dye::aqua((char*)E("\n  [")), std::cout << E("F1"), std::cout << dye::aqua((char*)E("] ")), std::cout << (E("Temporary Spoofing"));
    std::cout << dye::aqua((char*)E("\n  [")), std::cout << E("F2"), std::cout << dye::aqua((char*)E("] ")), std::cout << (E("Permanent Spoofing"));
    std::cout << dye::aqua((char*)E("\n  [")), std::cout << E("F3"), std::cout << dye::aqua((char*)E("] ")), std::cout << (E("MAC Address Spoofing"));
    std::cout << dye::aqua((char*)E("\n  [")), std::cout << E("F4"), std::cout << dye::aqua((char*)E("] ")), std::cout << (E("Trace Cleaning"));
    std::cout << dye::aqua((char*)E("\n  [")), std::cout << E("F5"), std::cout << dye::aqua((char*)E("] ")), std::cout << dye::light_blue((char*)E("Discord"));
    while (true) {
        if (GetAsyncKeyState(VK_F1) & 0x8000) {
            MessageBox(NULL, "Spoofing Hardware", "Base", MB_OK);
            spoof(microsoft); // spoof
            MessageBox(NULL, "Successfully Spoofed PC", "Base", MB_OK);
            goto Base;
        }
        if (GetAsyncKeyState(VK_F2) & 0x8000) {
            MessageBox(NULL, "Spoofing Hardware", "Base", MB_OK);
            spoof(apple); // spoof
            MessageBox(NULL, "Successfully Spoofed PC", "Base", MB_OK);
            goto Base;
        }
        if (GetAsyncKeyState(VK_F3) & 0x8000) {
            MessageBox(NULL, "Spoofing MAC Address", "Base", MB_OK);
            ResetAdapters();
            LI_FN(system)(E("cls"));
            CMAC* MAC = new CMAC();
            MAC->RestartAdapters();
            MAC->GenerateMAC();
            MAC->SpoofMACRandom();
            MessageBox(NULL, "Successfully Spoofed MAC Address", "Base", MB_OK);
            goto Base;
        }
        if (GetAsyncKeyState(VK_F4) & 0x8000) {
            // add ur own cleaning method
        }
        if (GetAsyncKeyState(VK_F5) & 0x8000) {
            LI_FN(system)(E("open https://discord.gg/")); // add your discord here.
            goto Base;
        }

    }
}
