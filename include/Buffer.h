#ifndef ZRSST_SRC_BUFFER_H_
#define ZRSST_SRC_BUFFER_H_

#include <cstring>
#include <string>
#include <vector>

class Buffer
{
public:
    typedef std::vector<char>   t_buffer;
    typedef t_buffer::size_type t_buffer_size;

public:
    explicit Buffer(std::size_t size = DEFAULT_BUFFER_SIZE);

    Buffer(const Buffer& rhs);

    Buffer(Buffer&& rhs) noexcept;

    Buffer& operator=(const Buffer& rhs);

    Buffer& operator=(Buffer&& rhs) noexcept;

    ~Buffer() = default;

    bool append(const char* str, std::size_t str_length);

    void append(const char* str);

    void append(const std::string& str);

    void clear();

    [[nodiscard]] const char* read() const;

    [[nodiscard]] bool empty() const;

    [[nodiscard]] t_buffer_size size() const;

    [[nodiscard]] t_buffer_size capacity() const;

    [[nodiscard]] bool canWrite(std::size_t length) const;

private:
    static const std::size_t DEFAULT_BUFFER_SIZE;

    t_buffer buffer_;
};

#endif // ZRSST_SRC_BUFFER_H_
