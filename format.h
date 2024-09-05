/*
 *		@brief: This header is an extension of the libstdc++ project, specifically adding c++ compatible format functions.
 *
 *		This file is part of CXX (https://github.com/seanhobeck/cxx).
 *		Copyright (c) 2024 Sean Hobeck.
 *
 *		This program is free software: you can redistribute it and/or modify
 *		it under the terms of the GNU General Public License as published by
 *		the Free Software Foundation, either version 3 of the License, or
 *		(at your option) any later version.
 *
 *		This program is distributed in the hope that it will be useful, but
 *		WITHOUT ANY WARRANTY; without even the implied warranty of
 *		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *		General Public License for more details.
 *
 *		You should have received a copy of the GNU General Public License
 *		along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *		@date    05 / 09 / 24
 *
 */

#ifndef CXX_FORMAT_H
#define CXX_FORMAT_H

/// @uses: std::snprintf(), std::string
#include <string>

/// @uses: std::shared_ptr, std::unique_ptr
#include <memory>

/// @uses: std::optional, std::nullopt
#include <optional>

namespace std
_GLIBCXX_VISIBILITY(default) {
    /// @fn: formats a string (with a specified length).
    template<typename... pargs_t>
    _GLIBCXX_NODISCARD
    inline std::string
    vnformat(const std::string &_format, std::size_t _n, pargs_t... _args) {
        if (auto sz = (std::size_t) std::snprintf(nullptr, 0, _format.c_str(), _args...) + 1; sz > 0 && _n != 0) {
            std::unique_ptr<char[]> buf(new char[_n]);
            std::snprintf(buf.get(), _n, _format.c_str(), _args...);
            return std::string(buf.get(), buf.get() + _n - 1);
        }
        return {};
    }

    /// @fn: formats a string.
    template<typename... pargs_t>
    _GLIBCXX_NODISCARD
    inline std::string
    vformat(const std::string _format, pargs_t... _args) {
        if (auto sz = (std::size_t) std::snprintf(nullptr, 0, _format.c_str(), _args...) + 1; sz > 0) {
            std::unique_ptr<char[]> buf(new char[sz]);
            std::snprintf(buf.get(), sz, _format.c_str(), _args...);
            return std::string(buf.get(), buf.get() + sz - 1);
        }
        return {};
    }

    /// @fn: formats a string (with std::optional).
    template<typename... pargs_t>
    _GLIBCXX_NODISCARD
    inline std::optional<std::string>
    voformat(const std::string &_format, pargs_t... _args) {
        auto s = vformat(_format, _args...);
        return s.length() == 0 ? std::make_optional(s) : std::nullopt;
    }

    /// @fn: formats a string (with a specified length).
    template<typename... pargs_t>
    _GLIBCXX_NODISCARD
    inline std::wstring
    wnformat(const std::wstring &_format, std::size_t _n, pargs_t... _args) {
        if (auto sz = (std::size_t) std::swprintf(nullptr, 0, _format.c_str(), _args...) + 1; sz > 0 && _n != 0) {
            std::unique_ptr<wchar_t[]> buf(new wchar_t[_n]);
            std::swprintf(buf.get(), _n, _format.c_str(), _args...);
            return std::wstring(buf.get(), buf.get() + _n - 1);
        }
        return {};
    }

    /// @fn: formats a non-unicode string.
    template<typename... pargs_t>
    _GLIBCXX_NODISCARD
    inline std::wstring
    wformat(const std::wstring _format, pargs_t... _args) {
        if (auto sz = (std::size_t) std::swprintf(nullptr, 0, _format.c_str(), _args...) + 1; sz > 0) {
            std::unique_ptr<wchar_t[]> buf(new wchar_t[sz]);
            std::swprintf(buf.get(), sz, _format.c_str(), _args...);
            return std::wstring(buf.get(), buf.get() + sz - 1);
        }
        return {};
    }

    /// @fn: formats a wstring (with std::optional).
    template<typename... pargs_t>
    _GLIBCXX_NODISCARD
    inline std::optional<std::wstring>
    woformat(const std::wstring &_format, pargs_t... _args) {
        auto s = wformat(_format, _args...);
        return s.length() == 0 ? std::make_optional(s) : std::nullopt;
    }
}
#endif
