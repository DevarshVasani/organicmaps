#pragma once

#include "std/string.hpp"
#include "std/utility.hpp"

enum class MapOptions : uint8_t
{
  Nothing = 0x0,
  Map = 0x1,
  CarRouting = 0x2,
  MapWithCarRouting = 0x3,
  Diff = 0x4
};

using MwmCounter = uint32_t;
using MwmSize = uint64_t;
using TLocalAndRemoteSize = pair<MwmSize, MwmSize>;

bool HasOptions(MapOptions mask, MapOptions options);

MapOptions SetOptions(MapOptions mask, MapOptions options);

MapOptions UnsetOptions(MapOptions mask, MapOptions options);

MapOptions LeastSignificantOption(MapOptions mask);

string DebugPrint(MapOptions options);
