#ifndef NO_DYNAMIC_MEMORY_HPP
#define NO_DYNAMIC_MEMORY_HPP

// This header file is used to disable dynamic memory allocation functions in C++.  It is used during development
// to ensure that the code does not use dynamic memory allocation functions.

#include <cstddef> // Defines std::size_t

// Disable use of dynamic memory allocation functions.
#define malloc forbidden_malloc
#define free forbidden_free
#define realloc forbidden_realloc
#define calloc forbidden_calloc

// Prevents use of C++ dynamic memory management.
void* operator new(std::size_t) = delete;
void* operator new[](std::size_t) = delete;
void operator delete(void*) noexcept = delete;
void operator delete[](void*) noexcept = delete;
void operator delete(void*, std::size_t) noexcept = delete;   // Added for sized deallocation
void operator delete[](void*, std::size_t) noexcept = delete; // Added for sized deallocation

#endif // NO_DYNAMIC_MEMORY_HPP