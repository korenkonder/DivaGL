/*
    by korenkonder
    GitHub/GitLab: korenkonder
*/

#include "file_stream.hpp"
#include <share.h>

file_stream::file_stream() : stream() {

}

file_stream::~file_stream() {
    close();
}

int file_stream::flush() {
    return fflush(stream);
}

void file_stream::close() {
    if (!this)
        return;

    if (stream) {
        fflush(stream);
        fclose(stream);
        stream = 0;
    }
    stream::close();
}

bool file_stream::check_null() {
    return !stream;
}

bool file_stream::check_not_null() {
    return !!stream;
}

void file_stream::align_read(size_t align) {
    int64_t position = _ftelli64(stream);
    size_t temp_align = align - position % align;
    if (align != temp_align)
        _fseeki64(stream, position + temp_align, 0);
}

void file_stream::align_write(size_t align) {
    int64_t position = _ftelli64(stream);
    size_t temp_align = align - position % align;
    if (align != temp_align) {
            memset(buf, 0, min_def(sizeof(buf), temp_align));
            size_t i = temp_align;
            while (i >= sizeof(buf)) {
                fwrite(buf, 1, sizeof(buf), stream);
                i -= sizeof(buf);
            }

            if (i > 0)
                fwrite(buf, 1, i, stream);
        }
}

size_t file_stream::read(size_t count) {
    return read((void*)0, count);
}

size_t file_stream::read(void* buf, size_t count) {
    if (!buf) {
        int64_t act_count = 0;
        while (count > 0) {
            act_count += fread(this->buf, 1, min_def(count, sizeof(this->buf)), stream);
            count -= sizeof(this->buf);
        }
        return act_count;
    }
    else
        return fread(buf, 1, count, stream);
}

size_t file_stream::read(void* buf, size_t size, size_t count) {
    if (!buf) {
        int64_t act_count = 0;
        while (count > 0) {
            act_count += fread(this->buf, size, min_def(count, sizeof(this->buf) / size), stream);
            count -= sizeof(this->buf);
        }
        return act_count;
    }
    else
        return fread(buf, size, count, stream);
}

size_t file_stream::write(size_t count) {
    return write((void*)0, count);
}

size_t file_stream::write(const void* buf, size_t count) {
    if (!buf) {
        memset(this->buf, 0, sizeof(this->buf));
        int64_t act_count = 0;
        while (count > 0) {
            act_count += fwrite(this->buf, 1, min_def(count, sizeof(this->buf)), stream);
            count -= sizeof(this->buf);
        }
        return act_count;
    }
    else
        return fwrite(buf, 1, count, stream);
}

size_t file_stream::write(const void* buf, size_t size, size_t count) {
    if (!buf) {
        memset(this->buf, 0, sizeof(this->buf));
        int64_t act_count = 0;
        while (count > 0) {
            act_count += fwrite(this->buf, 1, min_def(count, sizeof(this->buf) / size), stream);
            count -= sizeof(this->buf) / size;
        }
        return act_count;
    }
    else
        return fwrite(buf, size, count, stream);
}

int32_t file_stream::read_char() {
     return fgetc(stream);
}

int32_t file_stream::write_char(char c) {
    return fputc(c, stream);
}

int64_t file_stream::get_length() {
    if (stream) {
        size_t temp = _ftelli64(stream);
        _fseeki64(stream, 0, SEEK_END);
        length = _ftelli64(stream);
        _fseeki64(stream, temp, SEEK_SET);
    }
    else
        length = 0;
    return length;
}

int64_t file_stream::get_position() {
    return _ftelli64(stream);
}

int32_t file_stream::set_position(int64_t pos, int32_t seek) {
    return _fseeki64(stream, pos, seek);
}

void file_stream::open(const char* path, const char* mode) {
    close();

    if (!path || !mode)
        return;

    wchar_t* temp_path = utf8_to_utf16(path);
    wchar_t* temp_mode = utf8_to_utf16(mode);
    stream = _wfsopen(temp_path, temp_mode, _SH_DENYNO);
    get_length();
    free_def(temp_path);
    free_def(temp_mode);
}

void file_stream::open(const wchar_t* path, const wchar_t* mode) {
    close();

    if (!path || !mode)
        return;

    stream = _wfsopen(path, mode, _SH_DENYNO);
    get_length();
}
