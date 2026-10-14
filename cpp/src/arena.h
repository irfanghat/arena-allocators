#include <cstddef>
#include <cstdint>
#include <new>
#include <stdexcept>
#include <vector>

class Arena
{
  public:
    explicit Arena(size_t size) : m_size(size), m_offset(0)
    {
        m_buffer = new char[m_size];
    }

    ~Arena()
    {
        delete[] m_buffer;
    }

    Arena(Arena const&) = delete;
    Arena& operator=(Arena const&) = delete;

    template <typename T>
    T*
    allocate(size_t count = 1)
    {
        size_t required_size = sizeof(T) * count;
        size_t alignment = alignof(T);
        size_t aligned_offset = (m_offset + alignment - 1) & ~(alignment - 1);

        if (aligned_offset + required_size > m_size)
        {
            throw std::bad_alloc();
        }

        void* ptr = m_buffer + aligned_offset;
        m_offset = aligned_offset + required_size;

        return new (ptr) T();
    }

    void
    reset()
    {
        m_offset = 0;
    }

  private:
    char* m_buffer;
    size_t m_size;
    size_t m_offset;
};