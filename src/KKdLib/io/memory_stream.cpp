/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "memory_stream.hpp"

memory_stream::memory_stream() {
    data.data = data.vec.begin();
}

memory_stream::~memory_stream() {
    close();
}

int memory_stream::flush() {
    return 0;
}

void memory_stream::close() {
    if (!this)
        return;

    data.vec.clear();
    data.vec.shrink_to_fit();
    data.data = data.vec.begin();
    stream::close();
}

bool memory_stream::check_null() {
    return !data.vec.size();
}

bool memory_stream::check_not_null() {
    return !!data.vec.size();
}

void memory_stream::align_read(size_t align) {
    size_t position = data.data - data.vec.begin();
    size_t temp_align = align - position % align;
    if (align != temp_align) {
        size_t pos = data.data - data.vec.begin();
        if (data.vec.size() < (size_t)(pos + temp_align)) {
            size_t size = data.vec.size();
            data.vec.resize(pos + temp_align);
            data.data = data.vec.begin() + pos;
            memset(data.data._Ptr, 0, temp_align);
        }
        data.data += temp_align;
    }
}

void memory_stream::align_write(size_t align) {
    size_t position = data.data - data.vec.begin();
    size_t temp_align = align - position % align;
    if (align != temp_align) {
        size_t pos = data.data - data.vec.begin();
        if (data.vec.size() < (size_t)(pos + temp_align)) {
            size_t size = data.vec.size();
            data.vec.resize(pos + temp_align);
            data.data = data.vec.begin() + pos;
            memset(data.data._Ptr, 0, temp_align);
        }
        data.data += temp_align;
    }
}

size_t memory_stream::read(size_t count) {
    return read((void*)0, count);
}

size_t memory_stream::read(void* buf, size_t count) {
    if (data.data >= data.vec.end())
        return EOF;

    size_t _count = data.vec.end() - data.data;
    if (_count >= count)
        _count = count;
    if (buf)
        memcpy(buf, data.data._Ptr, _count);
    data.data += _count;
    return _count;
}

size_t memory_stream::read(void* buf, size_t size, size_t count) {
    if (data.data >= data.vec.end())
        return EOF;

    size_t _count = data.vec.end() - data.data;
    if (_count >= size * count)
        _count = size * count;
    if (buf)
        memcpy(buf, data.data._Ptr, _count);
    data.data += _count;
    return _count;
}

size_t memory_stream::write(size_t count) {
    return write((void*)0, count);
}

size_t memory_stream::write(const void* buf, size_t count) {
    size_t pos = data.data - data.vec.begin();
    if (data.vec.size() < pos + count) {
        data.vec.resize(pos + count);
        data.data = data.vec.begin() + pos;
    }
    if (buf)
        memcpy(data.data._Ptr, buf, count);
    else
        memset(data.data._Ptr, 0, count);
    data.data += count;
    return count;
}

size_t memory_stream::write(const void* buf, size_t size, size_t count) {
    size_t pos = data.data - data.vec.begin();
    size_t _count = size * count;
    if (data.vec.size() < (size_t)(pos + _count)) {
        data.vec.resize(pos + _count);
        data.data = data.vec.begin() + pos;
    }
    if (buf)
        memcpy(data.data._Ptr, buf, _count);
    else
        memset(data.data._Ptr, 0, _count);
    data.data += _count;
    return _count;
}

int32_t memory_stream::read_char() {
    if (data.data >= data.vec.end())
        return EOF;
    return *(data.data++);
}

int32_t memory_stream::write_char(char c) {
    size_t pos = data.data - data.vec.begin();
    if (data.vec.size() < (size_t)(pos + 1)) {
        size_t size = data.vec.size();
        data.vec.resize(pos + 1);
        data.data = data.vec.begin() + pos;
    }
    *(data.data++) = c;
    return 0;
}

int64_t memory_stream::get_length() {
    length = data.vec.size();
    return length;
}

int64_t memory_stream::get_position() {
    return data.data - data.vec.begin();
}

int32_t memory_stream::set_position(int64_t pos, int32_t seek) {
    switch (seek) {
    case SEEK_SET: {
        if (pos < 0)
            return EOF;

        if (data.vec.size() < (size_t)pos) {
            size_t size = data.vec.size();
            data.vec.resize(pos);
            memset(data.vec.data() + size, 0, pos - size);
        }
        data.data = data.vec.begin() + pos;
    } return 0;
    case SEEK_CUR: {
        if (pos > 0) {
            size_t _pos = data.data - data.vec.begin();
            if (data.vec.size() < (size_t)(_pos + pos)) {
                size_t size = data.vec.size();
                data.vec.resize(_pos + pos);
                memset(data.vec.data() + size, 0, _pos + pos - size);
                data.data = data.vec.begin() + _pos;
            }
            data.data += pos;
        }
        else if (pos < 0) {
            if (data.data - data.vec.begin() < -pos)
                return EOF;
            else
                data.data += pos;
        }
    } return 0;
    case SEEK_END: {
        if (pos < 0)
            break;

        if (data.vec.size() < (size_t)pos)
            break;

        data.data = data.vec.end() - pos;
    } return 0;
    }
    return EOF;
}

void memory_stream::open() {
    close();

    length = 0;
}

void memory_stream::open(const void* data, size_t size) {
    close();

    if (!size) {
        length = 0;
        return;
    }

    this->data.vec.clear();
    this->data.vec.resize(size);
    if (this->data.vec.data())
        if (data)
            memcpy(this->data.vec.data(), data, size);
        else
            memset(this->data.vec.data(), 0, size);
    this->data.data = this->data.vec.begin();
    length = size;
}

void memory_stream::open(std::vector<uint8_t>& data) {
    close();

    if (!data.size()) {
        length = 0;
        return;
    }

    this->data.vec = data;
    this->data.data = this->data.vec.begin();
    length = data.size();
}

void memory_stream::copy(void** data, size_t* size) {
    if (!this || !data || !size)
        return;

    *size = this->data.vec.size();
    *data = force_malloc(*size);
    memcpy(*data, this->data.vec.data(), *size);
}

void memory_stream::copy(std::vector<uint8_t>& data) {
    if (!this)
        return;

    size_t length = this->data.vec.size();
    data.resize(length);
    memcpy(data.data(), this->data.vec.data(), length);
}
