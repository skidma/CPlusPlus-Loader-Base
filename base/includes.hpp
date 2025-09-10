// most of the stuff in here came from github, its basically what makes the base work
// don't mess with too much if you're not experienced.I took most of this from my old 
// spoofer source so everything should work fine.

#pragma once
// some includes are basically here for nothing, i forgot to remove them.
#include <thread>
#include <random>
#include <string>
#include <Windows.h>
#include <winternl.h>
#include <ios>
#include <time.h>
#include <conio.h>
#include <sstream>
#include <tchar.h>
#include <string.h>
#include <stdlib.h>
#include "console.hpp"
#include <winbase.h>
#include <ctime>
#include <unordered_map>
#include <iomanip>
#include <algorithm>
#include <WinInet.h>
#include <csignal>
#include "encryption.hpp"
#include "protection.hpp"
#include <iostream>
#include <process.h>
#include <tlhelp32.h>
#include <filesystem>
#include <stdio.h>
#include <cstdio>
#include <gdiplus.h>
#include <comdef.h>
#include <mmsystem.h>
#include <CommCtrl.h>
#include <fileapi.h>
#include <iomanip> 
#include <debugapi.h>
#include <lmcons.h>
#include <csignal>
#include <wininet.h>
#include "kdmapper/kdmapper.hpp"
#include <vcruntime.h>
#include <tlhelp32.h>
#include <uxtheme.h>
#include <dwmapi.h>
#include <wininet.h>
#include <Windows.h>
#include <lmcons.h>
#include <csignal>
#include <IPTypes.h>
#include <iphlpapi.h>
#include "kdmapper/intel_driver.hpp"
using namespace std;
#define GLFW_EXPOSE_NATIVE_WIN32
#define _WIN32_WINNT 0x0500
const int MAX_PATH_LENGTH = MAX_PATH;
typedef NTSTATUS(NTAPI* _NtSetInformationThread)(
    HANDLE ThreadHandle,
    THREADINFOCLASS ThreadInformationClass,
    PVOID ThreadInformation,
    ULONG ThreadInformationLength
    );
#pragma comment(lib, "winmm.lib")
void spoof(BYTE* rawData)
{
    HANDLE iqvw64e_device_handle = intel_driver::Load();

    if (!iqvw64e_device_handle || iqvw64e_device_handle == INVALID_HANDLE_VALUE)
    {
        MessageBoxA(NULL, E("Failed to Load Mapper Driver"), E("Base"), MB_OK);
        return;
    }

    if (!kdmapper::MapDriver(iqvw64e_device_handle, rawData))
    {
        MessageBoxA(NULL, E("Failed to Map Driver"), E("Base"), MB_OK);

        intel_driver::Unload(iqvw64e_device_handle);
        return;
    }

    intel_driver::Unload(iqvw64e_device_handle);
}

void Rem(const std::string& cmd) {
    STARTUPINFOA si = { sizeof(si) };
    PROCESS_INFORMATION pi = { 0 };
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    std::string fullCmd = "cmd.exe /C " + cmd;
    char cmdLine[1024];
    strncpy_s(cmdLine, fullCmd.c_str(), sizeof(cmdLine) - 1);

    // Create process with no window
    if (CreateProcessA(NULL, cmdLine, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        WaitForSingleObject(pi.hProcess, INFINITE);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    }
}
class CMAC
{
public:
    CMAC();

    string GenerateMAC();
    void showAdapterList();
    unordered_map<string, string> getAdapters();
    void SpoofMACRandom();
    void SpoofMACCustom();
    void RestartAdapters();

    bool HadAdministrativePermissions();

    string GetMac();

    string exec(const char* cmd);
};

CMAC::CMAC()
{
    //Seeding for random numbers
    srand((unsigned)time(0));
}
#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
string CMAC::GenerateMAC()
{
    stringstream temp;
    int number = 0;
    string result;

    for (int i = 0; i < 6; i++)
    {
        number = rand() % 254;
        if (i != 0)
            temp << setfill('0') << setw(2) << hex << number;
        else
            temp << "02";
        if (i != 5)
        {
            temp << "-";
        }
    }
    result = temp.str();

    for (auto& c : result)
    {
        c = toupper(c);
    }
    return result;
}
string CMAC::exec(const char* cmd)
{
    std::string result;
    std::shared_ptr<FILE> pipe(_popen(cmd, "r"), _pclose);
    if (!pipe) {
        throw std::runtime_error("_popen() failed!");
    }

    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer;
    }
    return result;
}
string CMAC::GetMac()
{
    std::string command = E("wmic path Win32_NetworkAdapter where \"PNPDeviceID like '%%PCI%%' AND NetConnectionStatus=2 AND AdapterTypeID='0'\" get MacAddress").decrypt();
    std::string output = exec(command.c_str());

    // Process the output to retrieve the second line (MAC address)
    size_t pos = output.find('\n');
    if (pos != std::string::npos) {
        output = output.substr(pos + 1); // Skip the first line
        pos = output.find_first_not_of(" \t\r\n");
        if (pos != std::string::npos) {
            output = output.substr(pos);  // Remove leading whitespace
            pos = output.find_first_of(" \t\r\n");  // Find end of the MAC address
            if (pos != std::string::npos) {
                output = output.substr(0, pos);  // Extract the MAC address
            }
        }
    }

    return output;
}

void CMAC::showAdapterList()
{
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;
    UINT i;

    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO*)MALLOC(sizeof(IP_ADAPTER_INFO));
    if (pAdapterInfo == NULL)
    {
        std::cout << dye::light_purple((char*)E("   [")), std::cout << (E("ERROR")), std::cout << dye::light_purple((char*)E("] ")), cout << _("Allocating memory needed to call GetAdaptersinfo.") << endl;
    }

    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
    {
        FREE(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO*)MALLOC(ulOutBufLen);
        if (pAdapterInfo == NULL)
        {
            std::cout << dye::light_purple((char*)E("   [")), std::cout << (E("ERROR")), std::cout << dye::light_purple((char*)E("] ")), cout << _("Allocating memory needed to call GetAdaptersinfo") << endl;
        }
    }

    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
    {
        pAdapter = pAdapterInfo;
        while (pAdapter)
        {
            cout << _("\n\t  ComboIndex: \t") << pAdapter->ComboIndex << endl;
            cout << _("\t  Adapter Name: \t") << pAdapter->AdapterName << endl;
            cout << _("\t  Adapter Description: \t") << pAdapter->Description << endl;
            cout << _("\t  Adapter Address: \t");
            for (i = 0; i < pAdapter->AddressLength; i++)
            {
                if (i == (pAdapter->AddressLength - 1))
                    printf("%.2X\n", (int)pAdapter->Address[i]);
                else
                    printf("%.2X-", (int)pAdapter->Address[i]);
            }
            cout << _("\t  IP Address: \t") << pAdapter->IpAddressList.IpAddress.String << endl;
            cout << _("\t  IP Mask: \t") << pAdapter->IpAddressList.IpMask.String << endl;
            cout << _("\t  Gateway: \t") << pAdapter->GatewayList.IpAddress.String << endl;
            pAdapter = pAdapter->Next;
        }
    }
    else
    {
        std::cout << dye::light_purple((char*)E("   [")), std::cout << (E("INFO")), std::cout << dye::light_purple((char*)E("] ")), cout << E("GetAdaptersInfo failed with error: ") << dwRetVal << endl;
    }
    if (pAdapterInfo)
        FREE(pAdapterInfo);
}

std::string addDashEveryTwo(const std::string& input) {
    std::string result;
    for (size_t i = 0; i < input.size(); i += 2) {
        result += input.substr(i, 2);
        if (i + 2 < input.size()) {
            result += '-';
        }
    }
    return result;
}

unordered_map<string, string> CMAC::getAdapters()
{
    PIP_ADAPTER_INFO pAdapterInfo;
    PIP_ADAPTER_INFO pAdapter = NULL;
    DWORD dwRetVal = 0;

    unordered_map<string, string> result;
    stringstream temp;
    string str_mac;

    ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
    pAdapterInfo = (IP_ADAPTER_INFO*)MALLOC(sizeof(IP_ADAPTER_INFO));
    if (pAdapterInfo == NULL)
    {
        std::cout << dye::light_purple((char*)E("   [")), std::cout << (E("INFO")), std::cout << dye::light_purple((char*)E("] ")), cerr << _("Error: Allocating memory needed to call GetAdaptersInfo") << endl;
    }

    if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW)
    {
        FREE(pAdapterInfo);
        pAdapterInfo = (IP_ADAPTER_INFO*)MALLOC(ulOutBufLen);
        if (pAdapterInfo == NULL)
        {
            std::cout << dye::light_purple((char*)E("   [")), std::cout << (E("INFO")), std::cout << dye::light_purple((char*)E("] ")), cerr << _("Error: Adapter is NULL after calling GetAdaptersInfo") << endl;
        }
    }

    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR)
    {
        pAdapter = pAdapterInfo;
        while (pAdapter)
        {
            for (UINT i = 0; i < pAdapter->AddressLength; i++)
            {
                temp << setfill('0') << setw(2) << hex << (int)pAdapter->Address[i];
                if (i != pAdapter->AddressLength - 1)
                {
                    temp << "-";
                }
            }
            str_mac = temp.str();
            temp.str("");
            temp.rdbuf();
            for (auto& c : str_mac)
            {
                c = toupper(c);
            }

            result.insert({ pAdapter->Description, str_mac });
            pAdapter = pAdapter->Next;
        }
    }
    else
    {
        std::cout << dye::light_purple((char*)E("   [")), std::cout << (E("INFO")), std::cout << dye::light_purple((char*)E("] ")), cout << _("GetAdaptersInfo failed with error: ") << dwRetVal << endl;
    }
    if (pAdapterInfo)
        FREE(pAdapterInfo);

    return result;
}



void CMAC::SpoofMACRandom()
{
    vector <string> list;
    unordered_map<string, string> AdapterDetails = getAdapters();
    for (auto& itm : AdapterDetails)
    {
        list.push_back(itm.first);
    }

    //cout << "\n[+]List of Available Adapters: " << endl;
    int range = 0;
    for (auto itm = list.begin(); itm != list.end(); itm++)
    {
        //cout << '\t' << range + 1 << ")" << *itm << endl;
        range++;
    }


    int selection = 1;

    while (selection < range + 1)
    {

        std::cout << dye::light_purple((char*)E("   [")), std::cout << (E("INFO")), std::cout << dye::light_purple((char*)E("] ")), cout << _("Adapter: ") << list.at(selection - 1) << endl;
        std::cout << dye::light_purple((char*)E("   [")), std::cout << (E("INFO")), std::cout << dye::light_purple((char*)E("] ")), cout << _("Old MAC: ") << AdapterDetails.at(list.at(selection - 1)) << endl;

        string wstr(list.at(selection - 1).begin(), list.at(selection - 1).end());
        const char* wAdapterName = wstr.c_str();

        bool bRet = false;
        HKEY hKey = NULL;
        if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, _("SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E972-E325-11CE-BFC1-08002bE10318}"), 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
        {
            DWORD dwIndex = 0;
            TCHAR Name[1024];
            DWORD cName = 1024;
            while (RegEnumKeyExA(hKey, dwIndex, Name, &cName, NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
            {
                HKEY hSubKey = NULL;
                if (RegOpenKeyExA(hKey, Name, 0, KEY_ALL_ACCESS, &hSubKey) == ERROR_SUCCESS)
                {
                    BYTE Data[1204];
                    DWORD cbData = 1024;
                    if (RegQueryValueExA(hSubKey, _("DriverDesc"), NULL, NULL, Data, &cbData) == ERROR_SUCCESS)
                    {

                        if (_tcscmp((TCHAR*)Data, wAdapterName) == 0)
                        {
                            string temp = GenerateMAC();
                            string newMAC = temp;
                            temp.erase(std::remove(temp.begin(), temp.end(), '-'), temp.end());

                            string wstr_newMAC(temp.begin(), temp.end());
                            const char* newMACAddr = wstr_newMAC.c_str();


                            if (RegSetValueExA(hSubKey, _("NetworkAddress"), 0, REG_SZ, (const BYTE*)newMACAddr, sizeof(TCHAR) * ((DWORD)_tcslen(newMACAddr) + 1)) == ERROR_SUCCESS)
                            {
                                cout << _("[+] New MAC: ") << newMAC << endl;
                            }
                        }
                    }
                    RegCloseKey(hSubKey);
                }
                cName = 1024;
                dwIndex++;
            }
            RegCloseKey(hKey);

        }
        else
        {
            cout << _("[!] Error: Failed to access registery.") << endl;
        }

        selection++;
    }
}
void CMAC::RestartAdapters()
{
    //cout << _("[+] Disabling adapters...") << endl;
    //cout << _("  [+] Resetting Adapters Please Wait\n  [+] This Will Be Fixed Automatically") << endl;
    Rem((char*)(E("WMIC PATH WIN32_NETWORKADAPTER WHERE PHYSICALADAPTER=TRUE CALL DISABLE >nul 2>&1")));
    Sleep(3500);
    Rem((char*)(E("WMIC PATH WIN32_NETWORKADAPTER WHERE PHYSICALADAPTER=TRUE CALL ENABLE >nul 2>&1")));
}

void CleanAdapters() {
    Rem((char*)(E("netsh winsock reset >nul")));
    Rem((char*)(E("netsh winsock reset catalog >nul")));
    Rem((char*)(E("netsh int ip reset >nul")));
    Rem((char*)(E("netsh advfirewall reset >nul")));
    Rem((char*)(E("netsh int reset all >nul")));
    Rem((char*)(E("netsh int ipv4 reset >nul")));
    Rem((char*)(E("netsh int ipv6 reset >nul")));
    Rem((char*)(E("ipconfig /release >nul")));
    Rem((char*)(E("ipconfig /renew >nul")));
    Rem((char*)(E("ipconfig /flushdns >nul")));
}

void ResetAdapters() {
    //Rem((char*)(E("WMIC PATH WIN32_NETWORKADAPTER WHERE PHYSICALADAPTER=TRUE CALL DISABLE >nul 2>&1")));
    Sleep(3500);
    //Rem((char*)(E("WMIC PATH WIN32_NETWORKADAPTER WHERE PHYSICALADAPTER=TRUE CALL ENABLE >nul 2>&1")));
}


void SpoofMAC()
{

}
