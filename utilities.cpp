#include "pch.h"
#include "utilities.h"
#include <cstdlib>

bool bDisableFileLog = false;
bool bForcedWindowed = false;

uintptr_t base_address;
GameType GGameType = Invalid;

uint64_t Query_perf_frequency() {
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    return freq.QuadPart;
}

uint64_t Query_perf_counter() {
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);
    return counter.QuadPart;
}

uint64_t GetTimeNs()
{
    LARGE_INTEGER freq, counter;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    return (counter.QuadPart * 1'000'000'000ULL) / freq.QuadPart;
}

void DebugLog(const char* format, ...) 
{
#   ifndef _DEBUG
       if constexpr (LOG_IN_RELEASE)
            return;
#   endif
#   if NO_LOGGING == 1
        return;
#   endif


    std::time_t t = std::time(nullptr);
    std::tm tm = *std::localtime(&t);

    char timeBuf[32];
    std::strftime(timeBuf, sizeof(timeBuf), "[%H:%M:%S %d/%m/%Y] ", &tm);
    std::printf("%s", timeBuf);
    va_list args;
    va_start(args, format);
    std::vprintf(format, args);
    va_end(args);

    if constexpr (LOG_TO_FILE) {
        if (bDisableFileLog)
            return;

        FILE* file = std::fopen(LOG_FILE_NAME, "a");
        if (file) {
            std::fprintf(file, "%s", timeBuf);
            va_start(args, format);
            std::vfprintf(file, format, args);
            va_end(args);
            std::fclose(file);
        }
    }
}


uintptr_t GetBaseAddress()
{
    if (!base_address)
        base_address = (uintptr_t)GetModuleHandle(NULL);
    return base_address;
}

bool IsShenmue1()
{
    auto timestamp = *(int*)(GetBaseAddress() + 0x09DDD64);
    if (timestamp == 0x5BE4DE30)
        return true;
    return false;
}
bool IsShenmue2()
{
    auto timestamp = *(int*)(GetBaseAddress() + 0x08E7A54);
    if (timestamp == 0x5BE4E77A)
        return true;
    return false;
}
GameType DetectVersion(GameType OfType) 
{
    if (OfType == Invalid) 
    {
        if (IsShenmue1())
            return Shenmue_1;
        else if (IsShenmue2())
            return Shenmue_2;
        else
            return Invalid;
    }
    else
    {
        if (OfType == Shenmue_1)
        {
            return IsShenmue1() ? Shenmue_1 : Invalid;
        }
        else if (OfType == Shenmue_2)
        {
            return IsShenmue2() ? Shenmue_2 : Invalid;
        }
    }
	return Invalid;
}

void DebugStop()
{
#   if HOOK_CHECK == 1
        system("PAUSE");
#   endif
#   if HOOK_CHECK_EXIT == 1
        exit(0);
#   endif
}