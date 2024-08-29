/*
 * This file is part of the CXX distribution (https://github.com/seanhobeck/cxx).
 * Copyright (c) 2015 Liviu Ionescu.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 *
 *
 *      @author  Sean Hobeck
 *       @date 2024-08-29
 *
 *
 **/
#ifndef CXX_PRINT_H
#define CXX_PRINT_H

/// @uses: fwprintf, fprintf, FILE (_IO_FILE), std::ostream, std::cout, std::wcout
#include <iostream>

/// @uses: std::string, std::wstring
#include <string>

/// @uses: std::vformat, std::wformat
#include "format.h"

namespace std {
    /// @fn: prints out to stdout, with formatted args.
    /// @tparam: ...pargs_t packed args (same as virtual arguments).
    /// @param: _format the string to be formatted to.
    /// @param: _args format parameters to format the string and print to stdout.
    template<typename ... pargs_t>
    static void 
    print(const std::string& _format, pargs_t... _args) noexcept 
    { 
        std::cout << vformat(_format, _args...); 
    }
    /// @fn: prints out to a file stream, with formatted args.
    template<typename ... pargs_t>
    static void 
    print(const std::ostream& _fs, const std::string& _format, pargs_t... _args) noexcept 
    { 
        _fs << vformat(_format, _args...); 
    }


    /// @fn: prints a line out to stdout, with formatted args.
    /// @tparam: ...pargs_t packed args (same as virtual arguments).
    /// @param: _format the string to be formatted to.
    /// @param: _args format parameters to format the string and print a line to stdout.
    template<typename ... pargs_t>
    static void 
    println(const std::string& _format, pargs_t... _args) noexcept 
    { 
        std::cout << vformat(_format, _args...) << std::endl; 
    }
    /// @fn: prints a line out to a file stream, with formatted args.
    template<typename ... pargs_t>
    static void 
    println(const std::ostream& _fs, const std::string& _format, pargs_t... _args) noexcept 
    { 
        _fs << vformat(_format, _args...) << std::endl;
    }


    /// @fn: writes out to a file pointer, with formatted args (unicode).
    /// @tparam: ...pargs_t packed args (same as virtual arguments).
    /// @param: _fp 
    /// @param: _format the string to be formatted to.
    /// @param: _args format parameters to format the string and print a line to the file pointer.
    template<typename ... pargs_t>
    static void 
    vprint(FILE* _fp, const std::string& _format, pargs_t... _args) noexcept 
    { 
        if (auto _f = vformat(_format, _args...); _f.length() > 0)
            fwrite(_f.c_str(), _f.length(), 1ul, _fp);
    }
    /// @fn: writes out to stdout with formatted args (unicode).
    template<typename ... pargs_t>
    static void 
    vprint(const std::string& _format, pargs_t... _args) noexcept 
    { 
        vprint(stdout, _format, _args...);
    }
    
    
    /// @fn: writes out to a file pointer, ending the line, with formatted args (unicode).
    /// @tparam: ...pargs_t packed args (same as virtual arguments).
    /// @param: _fp the file pointer.
    /// @param: _format the string to be formatted to.
    /// @param: _args format parameters to format the string and print a line to the file pointer.
    template<typename ... pargs_t>
    static void 
    vprintln_unicode(FILE* _fp, const std::string& _format, pargs_t... _args) noexcept 
    { 
        if (auto _f = vformat(_format, _args...) + '\n'; _f.length() > 0)
            fprintf(_fp, _f.c_str());
    }
    /// @fn: writes out to stdout with formatted args (unicode).
    template<typename ... pargs_t>
    static void 
    vprintln_unicode(const std::string& _format, pargs_t... _args) noexcept 
    { 
        vprintln_unicode(stdout, _format, _args...);
    }
    /// @fn: writes out to std::wostream with formatted args (unicode).
    template<typename ... pargs_t>
    static void 
    vprintln_unicode(std::ostream& _stream, std::wstring& _format, pargs_t... _args) noexcept 
    { 
        if (auto _f = wformat(_format, _args...) + std::endl; _f.length() > 0)
            _stream << _f;
    }
        
    
    /// @fn: writes out to a file pointer, ending the line, with formatted args (non-unicode).
    /// @tparam: ...pargs_t packed args (same as virtual arguments).
    /// @param: _fp 
    /// @param: _format the wide string to be formatted to.
    /// @param: _args format parameters to format the wstring and print a line to the file pointer.
    template<typename ... pargs_t>
    static void 
    vprintln_nonunicode(FILE* _fp, const std::wstring& _format, pargs_t... _args) noexcept 
    { 
        if (auto _f = wformat(_format, _args...) + L'\n'; _f.length() > 0)
            fwprintf(_fp, _f.c_str());
    }
    /// @fn: writes out to stdout with formatted args (non-unicode).
    template<typename ... pargs_t>
    static void 
    vprintln_nonunicode(const std::wstring& _format, pargs_t... _args) noexcept 
    { 
        if (auto _f = wformat(_format, _args...) + L'\n'; _f.length() > 0)
            std::wcout << _f;
    }
    /// @fn: writes out to std::wostream with formatted args (non-unicode).
    template<typename ... pargs_t>
    static void 
    vprintln_nonunicode(std::wostream& _stream, std::wstring& _format, pargs_t... _args) noexcept 
    { 
        if (auto _f = wformat(_format, _args...) + L'\n'; _f.length() > 0)
            _stream << _f;
    }
}
#endif