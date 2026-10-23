#pragma once
#ifdef PROFILING_ENABLED
  #include <tracy/Tracy.hpp>
  #define PROFILE_SCOPE ZoneScoped
  #define PROFILE_FRAME FrameMark
#else
  #define PROFILE_SCOPE
  #define PROFILE_FRAME
#endif