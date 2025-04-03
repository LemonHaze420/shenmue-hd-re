#ifndef _HLIB_H_
#define _HLIB_H_

#define TOKEN_TASK(a, b, c, d) \
    ((int32_t)(a) | ((int32_t)(b) << 8) | ((int32_t)(c) << 16) | ((int32_t)(d) << 24))

struct HLibTaskData
{
    int name;
    int field_4;
    int content_size;
    int unk;
    HLibTaskData* prev_ptr;
    HLibTaskData* next_ptr;
    int status;
    int alignment;
    unsigned __int8* data[3];
};

struct HLib_Task
{
    void(__fastcall* taskCallbackFnPtr)(HLibTaskData*);
    int32_t taskToken;
    int16_t taskIndex;
    int16_t r8b;
    HLib_Task* next;
    HLib_Task* nextTask;
    HLib_Task* FirstTask;
    HLib_Task* someTaskPtr2;
    HLib_Task* someTaskPtr3;
    HLib_Task* someTaskPtr4;
    HLib_Task* someTaskPtr5;
    void(__fastcall* taskFn)(__int64);
    DWORD bHasInitCallback;
    DWORD dReserved;
    void(__fastcall* initCallbackFnPtr)(HLibTaskData*);
    __int64* unk_taskCallbackFnPtr;
    __int64* field_60;
    HLibTaskData* task_data_ptr;
};

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
namespace HLib {
#endif


void __fastcall Init(); 
HLib_Task *__fastcall EnqueueTaskWithoutParameter(int64_t *callbackFunction, signed __int16 nextFunctionIndex, unsigned __int16 r8b, int32_t taskToken); 
extern void __fastcall SetTaskCallbackToInitCallback(HLibTaskData *); 
extern unsigned __int64 Malloc(); 

#ifdef __cplusplus
} // namespace
#endif


#ifdef __cplusplus
}
#endif


#endif