
// Generics

{ "Shenmue::Initialization", 0x0, InitHook, (void**)&InitOrig },

#if STATIC_BUILD == 0
{ "Shenmue::Main", 0x0, MainHook, nullptr },
#endif

{ "HLib::EnqueueTaskWithoutParameter", 0x0, HLib::EnqueueTaskWithoutParameter, nullptr },

