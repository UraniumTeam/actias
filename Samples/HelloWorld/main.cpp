#include <Actias/System/Assert.h>
// #include <Actias/System/Atomic.h>
#include <Actias/System/Streams.h>
#include <Actias/System/Threading.h>
#include <limits>

ActiasResult ACTIAS_ABI RunMain(void*)
{
    const char message[] = "Hello, World!\n";

    ActiasHandle handle;
    auto fileHandleResult = ActiasGetStdFileHandle(ACTIAS_STDOUT, &handle);
    ACTIAS_Assert(fileHandleResult == ACTIAS_SUCCESS);

    auto writeResult = ActiasWrite(handle, message, sizeof(message), nullptr);
    ACTIAS_Assert(writeResult == ACTIAS_SUCCESS);

    return ACTIAS_SUCCESS;
}

int main()
{
    ActiasThreadCreateInfo createInfo{};
    createInfo.StartRoutine = &RunMain;
    createInfo.pName        = u8"Thread Поток Thread";

    ActiasThreadInfo thread;
    auto threadResult = ActiasCreateThread(&createInfo, &thread);
    ACTIAS_Assert(threadResult == ACTIAS_SUCCESS);

    for (;;)
    {
        // seems weird we have to do this, idk

        auto waitResult = ActiasWaitForThread(thread.Handle, 100000);
        if (waitResult == ACTIAS_THREAD_WAIT_TIMEOUT)
        {
            continue;
        }

        ACTIAS_Assert(waitResult == ACTIAS_SUCCESS);
        break;
    }

    auto releaseResult = ActiasReleaseThread(thread.Handle);
    ACTIAS_Assert(releaseResult == ACTIAS_SUCCESS);
}
