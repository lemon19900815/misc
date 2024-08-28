// Copyright (c), buerjia.
// All rights reserved.

#pragma once

#include <iostream>
#include <cstdint>
#include <fstream>

namespace file_util
{

static int64_t get_file_size(std::ifstream& in)
{
    // seek end for get the file size.
    in.seekg(0, std::ios::end);
    std::streampos file_size = in.tellg();
    in.seekg(0, std::ios::beg);

    return static_cast<int64_t>(file_size);
}

static int64_t get_file_size(const std::string& filename, int32_t opt)
{
    std::ifstream in(filename.c_str(), std::ifstream::in | opt);
    if (!in.is_open())
    {
        return -1;
    }

    auto file_size = get_file_size(in);
    in.close();

    return file_size;
}

static bool write(const std::string& filename, const std::string& content, int opt)
{
    std::ofstream stream(filename.c_str(), std::ofstream::out | opt);
    if (stream.is_open())
    {
        stream << content;
        stream.close();
        return true;
    }

    return false;
}

static bool write_bin(const std::string& filename, const std::string& content)
{
    return write(filename, content, std::ifstream::binary);
}

static bool read(const std::string& filename, std::string& content, int opt)
{
    std::ifstream in(filename.c_str(), std::ifstream::in | opt);
    if (!in.is_open())
    {
        return false;
    }

    auto file_size = get_file_size(in);
    content.resize(file_size, ' ');

    in.read(content.data(), file_size);
    in.close();

    return true;
}

static bool read_bin(const std::string& filename, std::string& content)
{
    return read(filename, content, std::ifstream::binary);
}

} // namespace file_util
