/*  Copyright (C) 2012-2017 by László Nagy
    This file is part of Bear.

    Bear is a tool to generate compilation database for clang tooling.

    Bear is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Bear is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "config.h"

#if defined HAVE_SPAWN_HEADER
# include <spawn.h>
#endif

#include "Array.h"
#include "Environment.h"

namespace ear {

    constexpr char target_flag[] = "-t";
    constexpr char library_flag[] = "-l";
    constexpr char file_flag[] = "-f";
    constexpr char search_flag[] = "-s";

    template<typename Resolver>
    class Executor {
    public:
        Executor() noexcept = delete;

        explicit Executor(const ::ear::Environment *state) noexcept
                : state_(state) {}

        Executor(const Executor &) = delete;

        Executor(Executor &&) noexcept = delete;

        ~Executor() noexcept = default;

        Executor &operator=(const Executor &) = delete;

        Executor &operator=(Executor &&) noexcept = delete;

    public:
#ifdef HAVE_EXECVE
        int execve(const char *path, char *const argv[], char *const envp[]) const noexcept {
            auto fp = Resolver::execve();
            if (fp == nullptr)
                return -1;

            if (state_ == nullptr)
                return fp(path, argv, envp);

            const size_t argv_length = ::ear::array::length(argv);
            const char *dst[argv_length + 7];

            const char **it = dst;
            *it++ = state_->wrapper();
            *it++ = target_flag;
            *it++ = state_->target();
            *it++ = library_flag;
            *it++ = state_->library();
            *it++ = "--";

            ::ear::array::copy(argv, argv + argv_length, it, it + argv_length);

            return fp(state_->wrapper(), const_cast<char *const *>(dst), envp);
        }
#endif

#ifdef HAVE_EXECV
        int execv(const char *path, char *const argv[]) const noexcept {
            auto fp = Resolver::execv();
            if (fp == nullptr)
                return -1;

            if (state_ == nullptr)
                return fp(path, argv);

            const size_t argv_length = ::ear::array::length(argv);
            const char *dst[argv_length + 7];

            const char **it = dst;
            *it++ = state_->wrapper();
            *it++ = target_flag;
            *it++ = state_->target();
            *it++ = library_flag;
            *it++ = state_->library();
            *it++ = "--";

            ::ear::array::copy(argv, argv + argv_length, it, it + argv_length);

            return fp(state_->wrapper(), const_cast<char *const *>(dst));
        }
#endif

#ifdef HAVE_EXECVPE
        int execvpe(const char *file, char *const argv[], char *const envp[]) const noexcept {
            if (state_ == nullptr) {
                auto fp = Resolver::execvpe();
                return (fp == nullptr) ? -1 : fp(file, argv, envp);
            }

            const size_t argv_length = ::ear::array::length(argv);
            const char *dst[argv_length + 9];

            const char **it = dst;
            *it++ = state_->wrapper();
            *it++ = target_flag;
            *it++ = state_->target();
            *it++ = library_flag;
            *it++ = state_->library();
            *it++ = file_flag;
            *it++ = file;
            *it++ = "--";

            ::ear::array::copy(argv, argv + argv_length, it, it + argv_length);

            auto fp = Resolver::execve();
            return (fp == nullptr) ? -1 : fp(state_->wrapper(), const_cast<char *const *>(dst), envp);
        }
#endif

#ifdef HAVE_EXECVP
        int execvp(const char *file, char *const argv[]) const noexcept {
            if (state_ == nullptr) {
                auto fp = Resolver::execvp();
                return (fp == nullptr) ? -1 : fp(file, argv);
            }

            const size_t argv_length = ::ear::array::length(argv);
            const char *dst[argv_length + 9];

            const char **it = dst;
            *it++ = state_->wrapper();
            *it++ = target_flag;
            *it++ = state_->target();
            *it++ = library_flag;
            *it++ = state_->library();
            *it++ = file_flag;
            *it++ = file;
            *it++ = "--";

            ::ear::array::copy(argv, argv + argv_length, it, it + argv_length);

            auto fp = Resolver::execv();
            return (fp == nullptr) ? -1 : fp(state_->wrapper(), const_cast<char *const *>(dst));
        }
#endif

#ifdef HAVE_EXECVP2
        int execvP(const char *file, const char *search_path, char *const argv[]) const noexcept {
            if (state_ == nullptr) {
                auto fp = Resolver::execvP();
                return (fp == nullptr) ? -1 : fp(file, search_path, argv);
            }

            const size_t argv_length = ::ear::array::length(argv);
            const char *dst[argv_length + 11];

            const char **it = dst;
            *it++ = state_->wrapper();
            *it++ = target_flag;
            *it++ = state_->target();
            *it++ = library_flag;
            *it++ = state_->library();
            *it++ = file_flag;
            *it++ = file;
            *it++ = search_flag;
            *it++ = search_path;
            *it++ = "--";

            ::ear::array::copy(argv, argv + argv_length, it, it + argv_length);

            auto fp = Resolver::execv();
            return (fp == nullptr) ? -1 : fp(state_->wrapper(), const_cast<char *const *>(dst));
        }
#endif

#ifdef HAVE_EXECT
        int exect(const char *path, char *const argv[], char *const envp[]) const noexcept {
            if (state_ == nullptr) {
                auto fp = Resolver::exect();
                return (fp == nullptr) ? -1 : fp(path, argv, envp);
            }

            const size_t argv_length = ::ear::array::length(argv);
            const char *dst[argv_length + 7];

            const char **it = dst;
            *it++ = state_->wrapper();
            *it++ = target_flag;
            *it++ = state_->target();
            *it++ = library_flag;
            *it++ = state_->library();
            *it++ = "--";

            ::ear::array::copy(argv, argv + argv_length, it, it + argv_length);

            auto fp = Resolver::execve();
            return (fp == nullptr) ? -1 : fp(state_->wrapper(), const_cast<char *const *>(dst), envp);
        }
#endif


#ifdef HAVE_POSIX_SPAWN
        int posix_spawn(pid_t *pid, const char *path,
                        const posix_spawn_file_actions_t *file_actions,
                        const posix_spawnattr_t *attrp,
                        char *const argv[],
                        char *const envp[]) const noexcept {
            auto fp = Resolver::posix_spawn();
            if (fp == nullptr)
                return -1;

            if (state_ == nullptr)
                return fp(pid, path, file_actions, attrp, argv, envp);

            const size_t argv_length = ::ear::array::length(argv);
            const char *dst[argv_length + 7];

            const char **it = dst;
            *it++ = state_->wrapper();
            *it++ = target_flag;
            *it++ = state_->target();
            *it++ = library_flag;
            *it++ = state_->library();
            *it++ = "--";

            ::ear::array::copy(argv, argv + argv_length, it, it + argv_length);

            return fp(pid, state_->wrapper(), file_actions, attrp, const_cast<char *const *>(dst), envp);
        }
#endif

#ifdef HAVE_POSIX_SPAWNP
        int posix_spawnp(pid_t *pid, const char *file,
                         const posix_spawn_file_actions_t *file_actions,
                         const posix_spawnattr_t *attrp,
                         char *const argv[],
                         char *const envp[]) const noexcept {
            if (state_ == nullptr) {
                auto fp = Resolver::posix_spawnp();
                return (fp == nullptr) ? -1 : fp(pid, file, file_actions, attrp, argv, envp);
            }

            const size_t argv_length = ::ear::array::length(argv);
            const char *dst[argv_length + 7];

            const char **it = dst;
            *it++ = state_->wrapper();
            *it++ = target_flag;
            *it++ = state_->target();
            *it++ = library_flag;
            *it++ = state_->library();
            *it++ = "--";

            ::ear::array::copy(argv, argv + argv_length, it, it + argv_length);

            auto fp = Resolver::posix_spawn();
            return (fp == nullptr) ? -1 : fp(pid, state_->wrapper(), file_actions, attrp,
                                             const_cast<char *const *>(dst), envp);
        }
#endif

    private:
        const ::ear::Environment *const state_{};
    };

}