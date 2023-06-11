#pragma once

#include <subhook.h>

#include "../Addresses.hpp"
#include "../structs/VehicleType.hpp"
#include "fmt/format.h"
#include <iostream>

static subhook::Hook *mainMenuHook;

int MainMenuFunc(int);

int MainMenuFunc(int p1) {
  subhook::ScopedHookRemove scopedRemove(mainMenuHook);
  addresses::CSDrawTextFunc(fmt::format("Sub Rosa 0.{}{}", *addresses::GameVersionNumber, (char)(*addresses::GameVersionPatchNumber + 97)).c_str(), 5, 0, 0, 50, 50, 32.f, 1.f, 1.f, 1.f, 1.f, (void*)0);

  return addresses::MainMenu(p1);
};