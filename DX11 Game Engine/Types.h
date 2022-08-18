#pragma once

#include <bitset>
#include <cstdint>
#include <assert.h>

using EntityID = std::uint32_t;
const EntityID MAX_ENTITIES = 4096;
using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;
