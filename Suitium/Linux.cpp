#if __linux__

#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>

#include "Addresses.hpp"
#include "Hooks.hpp"
#include "Hotfixes.hpp"
#include "Version.hpp"

void __attribute__((constructor)) entry()
{
  std::ifstream f("/proc/self/maps");
  std::string ln;
  std::getline(f, ln);

  std::string addrStr = ln.substr(0, ln.find("-"));

  std::uintptr_t baseAddress = std::stoul(addrStr, nullptr, 16);

  if (!addresses::Map(baseAddress) || !hotfixes::Make(baseAddress))
  {
    char text[256];
    std::snprintf(text, sizeof(char[256]), "Executable is not compatible with this Suitium version!\nAre you sure this is %d%c?", SUITIUM_GAME_VERSION_NUMBER, SUITIUM_GAME_VERSION_PATCH);
    std::cout << "Fatal Error: " << text << '\n';
    std::abort();
  }

  InstallHooks();
}

#endif