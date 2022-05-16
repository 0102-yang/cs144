#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

using namespace std;

ByteStream::ByteStream(const size_t capacity) : _buffer(), _capacity(capacity) {}

size_t ByteStream::write(const string &data) {
    if (_is_write_end) {
        return 0;
    }

    size_t write_count = 0;
    for (char c : data) {
        if (_buffer_size >= _capacity) {
            break;
        }
        _buffer += c;
        write_count++;
        _buffer_size++;
    }
    _write_count += write_count;
    return write_count;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const { return move(_buffer.substr(0, len)); }

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    _buffer = move(_buffer.substr(len));
    _read_count += len;
    _buffer_size -= len;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    string result = move(_buffer.substr(0, len));
    pop_output(len);
    return result;
}

void ByteStream::end_input() { _is_write_end = true; }

bool ByteStream::input_ended() const { return _is_write_end; }

size_t ByteStream::buffer_size() const { return _buffer_size; }

bool ByteStream::buffer_empty() const { return _buffer_size == 0; }

bool ByteStream::eof() const { return _is_write_end && (_write_count <= _read_count); }

size_t ByteStream::bytes_written() const { return _write_count; }

size_t ByteStream::bytes_read() const { return _read_count; }

size_t ByteStream::remaining_capacity() const { return _capacity - _buffer_size; }
