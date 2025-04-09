#ifndef NO_HEAP_HPP
#define NO_HEAP_HPP

// This header file is used to redefine dynamic memory allocation functions in C++.  It is used during development to
// ensure that the code does not use dynamic memory allocation functions.

#include <new>
#include <atomic>
#include <cstdio>
#include <cstdlib>
#include <dlfcn.h>

// This variable is used to control whether messages should be printed or not
// It uses std::atomic to ensure that it can be safely accessed from multiple threads
inline std::atomic<bool> printMessages{false};

// Call this function to enable message printing for heap management functions
inline void EnableNoHeapMessagePrinting()
{
    printMessages = true;
}

// Call this function to disable message printing for heap management functions
inline void DisableNoHeapMessagePrinting()
{
    printMessages = false;
}

// Function pointers to the original C-style heap management functions
inline void* (*originalMalloc)(std::size_t) = nullptr;
inline void (*originalFree)(void*) = nullptr;
inline void* (*originalCalloc)(std::size_t, std::size_t) = nullptr;
inline void* (*originalRealloc)(void*, std::size_t) = nullptr;

// Override the global new operator
inline void* operator new(std::size_t size)
{
    if (printMessages)
        std::fprintf(stderr, "NOHEAP: Heap allocation using operator \"new\"\n");

    // Save printMessages and set it to false to disable message printing during execution of std::malloc
    auto savePrintMessages{printMessages.exchange(false)};
    auto result{std::malloc(size)};

    // Restore the original value of printMessages
    printMessages.exchange(savePrintMessages);

    return result;
}

// Override the global delete operator
inline void operator delete(void* ptr) noexcept
{
    if (printMessages)
        std::fprintf(stderr, "NOHEAP: Heap deallocation using operator \"delete\"\n");

    // Save printMessages and set it to false to disable message printing during execution of std::free
    auto savePrintMessages{printMessages.exchange(false)};
    std::free(ptr);

    // Restore the original value of printMessages
    printMessages.exchange(savePrintMessages);
}

inline void operator delete(void* ptr, std::size_t size) noexcept
{
    if (printMessages)
        std::fprintf(
            stderr, "NOHEAP: Heap deallocation of size %zu using operator \"delete(void*, std::size_t)\"\n", size);

    // Save printMessages and set it to false to disable message printing during execution of std::free
    auto savePrintMessages{printMessages.exchange(false)};
    std::free(ptr);

    // Restore the original value of printMessages
    printMessages.exchange(savePrintMessages);
}

inline void* operator new[](std::size_t size)
{
    if (printMessages)
        std::fprintf(stderr, "NOHEAP: Heap allocation using operator \"new[]\"\n");

    // Save printMessages and set it to false to disable message printing during execution of std::malloc
    auto savePrintMessages{printMessages.exchange(false)};
    auto result{std::malloc(size)};

    // Restore the original value of printMessages
    printMessages.exchange(savePrintMessages);
    return result;
}

// Override the global delete[] operator
inline void operator delete[](void* ptr) noexcept
{
    if (printMessages)
        std::fprintf(stderr, "NOHEAP: Heap deallocation using operator \"delete[]\"\n");

    // Save printMessages and set it to false to disable message printing during execution of std::free
    auto savePrintMessages{printMessages.exchange(false)};
    std::free(ptr);

    // Restore the original value of printMessages
    printMessages.exchange(savePrintMessages);
}

inline void operator delete[](void* ptr, std::size_t size) noexcept
{
    if (printMessages)
        std::fprintf(
            stderr, "NOHEAP: Heap deallocation of size %zu using operator \"delete[](void*, std::size_t)\"\n", size);

    // Save printMessages and set it to false to disable message printing during execution of std::free
    auto savePrintMessages{printMessages.exchange(false)};
    std::free(ptr);

    // Restore the original value of printMessages
    printMessages.exchange(savePrintMessages);
}

// Override the global malloc function
inline void* malloc(std::size_t size)
{
    // Save the original malloc function pointer if it is not already saved
    if (not originalMalloc)
        originalMalloc = (void* (*)(std::size_t))dlsym(RTLD_NEXT, "malloc");
    if (printMessages)
        std::fprintf(stderr, "NOHEAP: Heap allocation using \"malloc\"\n");
    return originalMalloc(size);
}

// Override the global free function
inline void free(void* ptr) noexcept
{
    // Save the original free function pointer if it is not already saved
    if (not originalFree)
        originalFree = (void (*)(void*))dlsym(RTLD_NEXT, "free");
    if (printMessages)
        std::fprintf(stderr, "NOHEAP: Heap deallocation using \"free\"\n");
    originalFree(ptr);
}

// Override the global calloc function
inline void* calloc(std::size_t num, std::size_t size)
{
    if (not originalCalloc)
        originalCalloc = (void* (*)(std::size_t, std::size_t))dlsym(RTLD_NEXT, "calloc");
    if (printMessages)
        std::fprintf(stderr, "NOHEAP: Heap allocation using \"calloc\"\n");
    return originalCalloc(num, size);
}

// Override the global realloc function
inline void* realloc(void* ptr, std::size_t size)
{
    if (not originalRealloc)
        originalRealloc = (void* (*)(void*, std::size_t))dlsym(RTLD_NEXT, "realloc");
    if (printMessages)
        std::fprintf(stderr, "NOHEAP: Heap reallocation using \"realloc\"\n");
    return originalRealloc(ptr, size);
}

#endif // NO_HEAP_HPP
