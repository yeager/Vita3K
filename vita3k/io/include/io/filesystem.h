// Vita3K emulator project
// Copyright (C) 2025 Vita3K team
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#pragma once

#include <util/fs.h>

#ifdef _WIN32
#include <util/string_utils.h>
#endif

#include <dirent.h>

#include <memory>

typedef std::shared_ptr<FILE> FilePtr;

// For opening Boost.Filesystem files, Boost returns wide strings for Windows, normal strings for other OS
// Dirent and FILE only accept and return wide char strings for Windows, and normal for other OS
#ifdef _WIN32
const wchar_t *translate_open_mode(const int flags);

inline FilePtr create_shared_file(const fs::path &path, const int open_mode) {
    const auto file = _wfopen(path.generic_path().wstring().c_str(), translate_open_mode(open_mode));
    return file ? FilePtr(file, std::fclose) : FilePtr();
}

typedef std::shared_ptr<_WDIR> DirPtr;

inline DirPtr create_shared_dir(const fs::path &path) {
    return DirPtr(_wopendir(path.generic_path().wstring().c_str()), _wclosedir);
}

inline std::string get_file_in_dir(const _wdirent *file) {
    return string_utils::wide_to_utf(file->d_name);
}

inline _wdirent *get_system_dir_ptr(const DirPtr &dir) {
    return _wreaddir(dir.get());
}
#else
const char *translate_open_mode(const int flags);

inline FilePtr create_shared_file(const fs::path &path, const int open_mode) {
    const auto file = fopen(path.generic_path().string().c_str(), translate_open_mode(open_mode));
    return file ? FilePtr(file, std::fclose) : FilePtr();
}

typedef std::shared_ptr<DIR> DirPtr;

inline DirPtr create_shared_dir(const fs::path &path) {
    return DirPtr(opendir(path.generic_path().string().c_str()), closedir);
}

inline std::string get_file_in_dir(dirent *file) {
    return std::string(file->d_name);
}

inline dirent *get_system_dir_ptr(DirPtr dir) {
    return readdir(dir.get());
}
#endif
