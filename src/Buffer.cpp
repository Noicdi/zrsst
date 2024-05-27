#include "Buffer.h"

const std::size_t Buffer::DEFAULT_BUFFER_SIZE = 1024;

Buffer::Buffer(const std::size_t size)
{
    buffer_.reserve(size);
}

Buffer::Buffer(const Buffer& rhs)
    : buffer_(rhs.buffer_)
{
    buffer_.reserve(rhs.buffer_.capacity());
}

Buffer::Buffer(Buffer&& rhs) noexcept
{
    buffer_.swap(rhs.buffer_);
}

Buffer& Buffer::operator=(const Buffer& rhs)
{
    if (&rhs != this)
    {
        buffer_ = rhs.buffer_;
        buffer_.reserve(rhs.buffer_.capacity());
    }

    return *this;
}

Buffer& Buffer::operator=(Buffer&& rhs) noexcept
{
    if (&rhs != this)
    {
        buffer_.swap(rhs.buffer_);
    }

    return *this;
}

bool Buffer::append(const char* str, const std::size_t str_length)
{
    if (!canWrite(str_length))
    {
        return false;
    }

    buffer_.insert(buffer_.end(), str, str + str_length);

    return true;
}

void Buffer::append(const char* str)
{
    append(str, std::strlen(str));
}

void Buffer::append(const std::string& str)
{
    append(str.c_str(), str.size());
}

Buffer::t_buffer_size Buffer::size() const
{
    return buffer_.size();
}

Buffer::t_buffer_size Buffer::capacity() const
{
    return buffer_.capacity();
}

bool Buffer::canWrite(const std::size_t length) const
{
    return size() + length <= capacity();
}

const char* Buffer::read() const
{
    return buffer_.data();
}

void Buffer::clear()
{
    buffer_.clear();
}

bool Buffer::empty() const
{
    return buffer_.empty();
}
