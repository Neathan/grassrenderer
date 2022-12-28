#pragma once

#ifndef NO_PROFILING

#include <tracy/Tracy.hpp>
#include <tracy/TracyOpenGL.hpp>


#define ProfileFrame FrameMark

#define ProfileScope ZoneScoped
#define ProfileScopeNamed(name) ZoneScopedN(name)


#define ProfileRegisterGPUContext TracyGpuContext
#define ProfileCollectGPU TracyGpuCollect

#define ProfileGPUScope(name) TracyGpuZone(name)

#endif