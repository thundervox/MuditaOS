// Copyright (c) 2017-2021, Mudita Sp. z.o.o. All rights reserved.
// For licensing, see https://github.com/mudita/MuditaOS/LICENSE.md

#include <iosyscalls.hpp>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <fcntl.h>
#include <errno.h>
#include <stdarg.h>     // for va_*
#include <limits.h>     // for PATH_MAX

#include "syscalls_real.hpp"

#include "debug.hpp"

namespace
{
    namespace real {
        __REAL_DECL(link);
        __REAL_DECL(unlink);
        __REAL_DECL(symlink);

        __REAL_DECL(fcntl);
        __REAL_DECL(fcntl64);

        __REAL_DECL(chdir);
        __REAL_DECL(fchdir);

        __REAL_DECL(getcwd);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
        __REAL_DECL(getwd);
#pragma GCC diagnostic pop

        __REAL_DECL(get_current_dir_name);
        __REAL_DECL(mkdir);

        __REAL_DECL(chmod);
        __REAL_DECL(fchmod);
        __REAL_DECL(fsync);
        __REAL_DECL(fdatasync);

        __REAL_DECL(__xstat);
        __REAL_DECL(__lxstat);
        __REAL_DECL(__fxstat);

        __REAL_DECL(__xstat64);
        __REAL_DECL(__lxstat64);
        __REAL_DECL(__fxstat64);
    } // namespace real

    void __attribute__((constructor)) _lib_posix_initialize()
    {
        __REAL_DLSYM(link);
        __REAL_DLSYM(unlink);
        __REAL_DLSYM(symlink);

        __REAL_DLSYM(fcntl);
        __REAL_DLSYM(fcntl64);

        __REAL_DLSYM(chdir);
        __REAL_DLSYM(fchdir);

        __REAL_DLSYM(getcwd);
        __REAL_DLSYM(getwd);
        __REAL_DLSYM(get_current_dir_name);
        __REAL_DLSYM(mkdir);

        __REAL_DLSYM(chmod);
        __REAL_DLSYM(fchmod);
        __REAL_DLSYM(fsync);
        __REAL_DLSYM(fdatasync);

        __REAL_DLSYM(__xstat);
        __REAL_DLSYM(__lxstat);
        __REAL_DLSYM(__fxstat);

        __REAL_DLSYM(__xstat64);
        __REAL_DLSYM(__lxstat64);
        __REAL_DLSYM(__fxstat64);


        if (!(real::link && real::unlink && real::symlink && real::fcntl && real::fcntl64
            && real::chdir && real::fchdir && real::getcwd && real::getwd
            && real::get_current_dir_name && real::mkdir && real::chmod && real::fchmod
            && real::fsync && real::fdatasync
            && real::__xstat && real::__lxstat && real::__fxstat
            && real::__xstat64 && real::__lxstat64 && real::__fxstat64))
        {
            abort();
        }
    }
} //namespace

extern "C" {
    namespace vfs = vfsn::linux::internal;
    using fs = purefs::fs::filesystem;

    int _iosys_link(const char *oldpath, const char *newpath)
    {
        if(vfs::redirect_to_image(oldpath))
        {
            TRACE_SYSCALLN("(%s,%s) -> VFS", oldpath, newpath);
            errno = ENOSYS;
            std::cerr << "Unsupported syscall " <<  __PRETTY_FUNCTION__ << std::endl;
            return -1;
        }
        else
        {
            TRACE_SYSCALLN("(%s,%s) -> linux fs", oldpath, newpath);
            char tmp[PATH_MAX], tmp2[PATH_MAX];
            const auto oldp = vfs::npath_translate(oldpath,tmp);
            const auto newp = vfs::npath_translate(newpath,tmp2);
            return real::link(oldp,newp);
        }
    }
    __asm__(".symver _iosys_link,link@GLIBC_2.2.5");

     int _iosys_unlink(const char *name)
     {
        if(vfs::redirect_to_image(name))
        {
            TRACE_SYSCALLN("(%s) -> VFS", name);
            return vfs::invoke_fs(&fs::unlink, name);
        }
        else
        {
            TRACE_SYSCALLN("(%s) -> linux fs", name);
            char tmp[PATH_MAX];
            const auto path = vfs::npath_translate(name,tmp);
            return real::unlink(path);
        }
     }
    __asm__(".symver _iosys_unlink,unlink@GLIBC_2.2.5");

    int _iosys_stat(const char *file, struct stat *pstat)
    {
        if(vfs::redirect_to_image(file))
        {
            TRACE_SYSCALLN("(%s) -> VFS", file);
            return vfs::invoke_fs(&fs::stat, file, *pstat);
        }
        else
        {
            TRACE_SYSCALLN("(%s) -> linux fs", file);
            char tmp[PATH_MAX];
            const auto newfile = vfs::npath_translate(file,tmp);
            return real::__xstat(1,newfile,pstat);
        }
    }
    __asm__(".symver _iosys_stat,stat@GLIBC_2.2.5");

    int _iosys_lstat(const char *pathname, struct stat *statbuf)
    {
        if(vfs::redirect_to_image(pathname))
        {
            TRACE_SYSCALLN("(%s) -> VFS", pathname);
            return vfs::invoke_fs(&fs::stat, pathname, *statbuf);
        }
        else
        {
            TRACE_SYSCALLN("(%s) -> linux fs", pathname);
            char tmp[PATH_MAX];
            const auto newpath = vfs::npath_translate(pathname,tmp);
            return real::__lxstat(1,newpath,statbuf);
        }
    }
    __asm__(".symver _iosys_lstat,lstat@GLIBC_2.2.5");

    int _iosys_fstat(int fd, struct stat *pstat)
    {
        if(vfs::is_image_fd(fd))
        {
            TRACE_SYSCALLN("(%d) -> VFS", fd);
            return vfs::invoke_fs(&fs::fstat,fd,*pstat);
        }
        else
        {
            TRACE_SYSCALLN("(%d) -> linux fs", fd);
            return real::__fxstat(1,fd,pstat);
        }
    }
    __asm__(".symver _iosys_fstat,fstat@GLIBC_2.2.5");

    int _iosys_fcntl(int fd, int cmd, ... /* arg */ )
    {
        if(vfs::is_image_fd(fd))
        {
            TRACE_SYSCALLN("(%d) -> VFS", fd);
            errno = ENOSYS;
            std::cerr << "Unsupported syscall " << __PRETTY_FUNCTION__ << std::endl;
            return -1;
        }
        else
        {
            TRACE_SYSCALLN("(%d) -> linux fs", fd);
            uintptr_t param;
            va_list args;
            va_start(args,cmd);
            param = va_arg(args,uintptr_t);
            auto ret =  real::fcntl(fd, cmd, param );
            va_end(args);
            return ret;
        }
    }
    __asm__(".symver _iosys_fcntl,fcntl@GLIBC_2.2.5");

    int _iosys_fcntl64(int fd, int cmd, ... /* arg */ )
    {
        if(vfs::is_image_fd(fd))
        {
            TRACE_SYSCALLN("(%d) -> VFS", fd);
            errno = ENOSYS;
            std::cerr << "Unsupported syscall " << __PRETTY_FUNCTION__ << std::endl;
            return -1;
        } else {
            TRACE_SYSCALLN("(%d) -> linux fs", fd);
            uintptr_t param;
            va_list args;
            va_start(args,cmd);
            param = va_arg(args,uintptr_t);
            auto ret =  real::fcntl64(fd, cmd, param);
            va_end(args);
            return ret;
        }
    }
    __asm__(".symver _iosys_fcntl64,fcntl64@GLIBC_2.2.5");

    int _iosys_chdir(const char *path)
    {
        if(vfs::redirect_to_image(path))
        {
            TRACE_SYSCALLN("(%s) -> VFS", path);
            return vfs::invoke_fs(&fs::chdir, path);
        }
        else
        {
            TRACE_SYSCALLN("(%s) -> linux fs", path);
            char tmp[PATH_MAX];
            const auto newpath = vfs::npath_translate(path,tmp);
            return real::chdir(newpath);
        }
    }
    __asm__(".symver _iosys_chdir,chdir@GLIBC_2.2.5");

    int _iosys_fchdir(int fd)
    {
        if(vfs::is_image_fd(fd))
        {
            TRACE_SYSCALLN("(%d) -> VFS", fd);
            errno = ENOSYS;
            std::cerr << "Unsupported syscall " << __PRETTY_FUNCTION__ << std::endl;
            return -1;
        }
        else
        {
            TRACE_SYSCALLN("(%d) -> linux fs", fd);
            return real::fchdir(fd);
        }
    }
    __asm__(".symver _iosys_fchdir,fchdir@GLIBC_2.2.5");

    char *_iosys_getcwd(char *buf, size_t size)
    {
        if(vfs::redirect_to_image())
        {
            TRACE_SYSCALLN("%s", "() -> VFS");
            auto vfs = purefs::subsystem::vfs_core();
            if(!vfs) {
                errno = EIO;
                return nullptr;
            }
            auto xwd = vfs->getcwd();
            xwd.copy(buf,size);
            return buf;
        }
        else
        {
            TRACE_SYSCALLN("%s", "() -> linux fs");
            return real::getcwd(buf,size);
        }
    }
    __asm__(".symver _iosys_getcwd,getcwd@GLIBC_2.2.5");

    char *_iosys_getwd(char *buf)
    {
        if(vfs::redirect_to_image())
        {
            TRACE_SYSCALLN("%s", "() -> VFS");
            auto vfs = purefs::subsystem::vfs_core();
            if(!vfs) {
                errno = EIO;
                return nullptr;
            }
            auto xwd = vfs->getcwd();
            xwd.copy(buf,xwd.size());
            return buf;
        }
        else
        {
            TRACE_SYSCALLN("%s", "() -> linux fs");
            return real::getwd(buf);
        }
    }
    __asm__(".symver _iosys_getwd,getwd@GLIBC_2.2.5");


    char *_iosys_get_current_dir_name(void)
    {
        if(vfs::redirect_to_image())
        {
            TRACE_SYSCALLN("%s", "() -> VFS");
            auto vfs = purefs::subsystem::vfs_core();
            if(!vfs) {
                errno = EIO;
                return nullptr;
            }
            auto xwd = vfs->getcwd();
            auto ret = new char[xwd.size()+1];
            xwd.copy(ret,xwd.size());
            ret[xwd.size()] = '\0';
            return ret;
        }
        else
        {
            TRACE_SYSCALLN("%s", "() -> linux fs");
            return real::get_current_dir_name();
        }
    }
    __asm__(".symver _iosys_get_current_dir_name,get_current_dir_name@GLIBC_2.2.5");

    int _iosys_mkdir(const char *pathname, mode_t mode)
    {
        if(vfs::redirect_to_image(pathname))
        {
            TRACE_SYSCALLN("(%s) -> VFS", pathname);
            return vfs::invoke_fs(&fs::mkdir, pathname, mode);
        }
        else
        {
            TRACE_SYSCALLN("(%s) -> linux fs", pathname);
            char tmp[PATH_MAX];
            const auto path = vfs::npath_translate(pathname,tmp);
            return real::mkdir(path,mode);
        }
    }
    __asm__(".symver _iosys_mkdir,mkdir@GLIBC_2.2.5");

    int _iosys_chmod(const char *pathname, mode_t mode)
    {
        if(vfs::redirect_to_image(pathname))
        {
            TRACE_SYSCALLN("(%s) -> VFS", pathname);
            return vfs::invoke_fs(&fs::chmod,pathname,mode);
        }
        else
        {
            TRACE_SYSCALLN("(%s) -> linux fs", pathname);
            char tmp[PATH_MAX];
            const auto path = vfs::npath_translate(pathname,tmp);
            return real::chmod(path,mode);
        }
    }
    __asm__(".symver _iosys_chmod,chmod@GLIBC_2.2.5");

    int _iosys_fchmod(int fd, mode_t mode)
    {
        if(vfs::is_image_fd(fd))
        {
            TRACE_SYSCALLN("(%d) -> VFS", fd);
            return vfs::invoke_fs(&fs::fchmod,fd,mode);
        }
        else
        {
            TRACE_SYSCALLN("(%d) -> linux fs", fd);
            return real::fchmod(fd,mode);
        }
    }
    __asm__(".symver _iosys_fchmod,fchmod@GLIBC_2.2.5");

    int _iosys_fsync(int fd)
    {
        if(vfs::is_image_fd(fd))
        {
            TRACE_SYSCALLN("(%d) -> VFS", fd);
            return vfs::invoke_fs(&fs::fsync, fd);
        }
        else
        {
            TRACE_SYSCALLN("(%d) -> linux fs", fd);
            return real::fsync(fd);
        }
    }
    __asm__(".symver _iosys_fsync,fsync@GLIBC_2.2.5");

    int _iosys_fdatasync(int fd)
    {
        if(vfs::is_image_fd(fd))
        {
            TRACE_SYSCALLN("(%d) -> VFS", fd);
            return vfs::invoke_fs(&fs::fsync, fd);
        }
        else
        {
            TRACE_SYSCALLN("(%d) -> linux fs", fd);
            return real::fdatasync(fd);
        }
    }
    __asm__(".symver _iosys_fdatasync,fdatasync@GLIBC_2.2.5");

    int _iosys_symlink(const char *target, const char *linkpath)
    {
        if(vfs::redirect_to_image(target))
        {
            TRACE_SYSCALLN("(%s,%s) -> VFS", target, linkpath);
            return vfs::invoke_fs(&fs::symlink,target,linkpath);
        }
        else
        {
            TRACE_SYSCALLN("(%s,%s) -> linux fs", target, linkpath);
            char tmp[PATH_MAX], tmp2[PATH_MAX];
            const auto tgtp = vfs::npath_translate(target,tmp);
            const auto linp = vfs::npath_translate(linkpath,tmp2);
            return real::symlink(tgtp,linp);
        }
    }
    __asm__(".symver _iosys_symlink,symlink@GLIBC_2.2.5");

    int _iosys_xstat(int ver, const char * path, struct stat * stat_buf)
    {
        if(vfs::redirect_to_image(path))
        {
            TRACE_SYSCALLN("(%s) -> VFS", path);
            return vfs::invoke_fs(&fs::stat, path, *stat_buf);
        }
        else
        {
            TRACE_SYSCALLN("(%s) -> linux fs", path);
            char tmp[PATH_MAX];
            const auto newp = vfs::npath_translate(path,tmp);
            return real::__xstat(ver,newp,stat_buf);
        }
    }
    __asm__(".symver _iosys_xstat,__xstat@GLIBC_2.2.5");

    int _iosys_lxstat(int ver, const char * path, struct stat * stat_buf)
    {
        if(vfs::redirect_to_image(path))
        {
            TRACE_SYSCALLN("(%s) -> VFS", path);
            return vfs::invoke_fs(&fs::stat, path, *stat_buf);
        }
        else
        {
            TRACE_SYSCALLN("(%s) -> linux fs", path);
            char tmp[PATH_MAX];
            const auto newp = vfs::npath_translate(path,tmp);
            return real::__lxstat(ver,newp,stat_buf);
        }
    }
    __asm__(".symver _iosys_lxstat,__lxstat@GLIBC_2.2.5");

    int _iosys_fxstat(int ver, int fildes, struct stat * stat_buf)
    {
        if(vfs::is_image_fd(fildes))
        {
            TRACE_SYSCALLN("(%d) -> VFS", fildes);
            return vfs::invoke_fs(&fs::fstat, fildes, *stat_buf);
        }
        else
        {
            TRACE_SYSCALLN("(%d) -> linux fs", fildes);
            return real::__fxstat(ver,fildes,stat_buf);
        }
    }
    __asm__(".symver _iosys_fxstat,__fxstat@GLIBC_2.2.5");

    int _iosys_fxstatat(int vers, int fd, const char *filename, struct stat *buf, int flag)
    {
        errno = ENOSYS;
        std::cerr << "Unsupported syscall " << __PRETTY_FUNCTION__ << std::endl;
        return -1;
    }
    __asm__(".symver _iosys_fxstatat,__fxstatat@GLIBC_2.4");

    int _iosys_xstat64(int ver, const char * path, struct stat64 * stat_buf)
    {
        if(vfs::redirect_to_image(path))
        {
            TRACE_SYSCALLN("(%s) -> VFS", path);
            return vfs::invoke_fs(&fs::stat, path, *(struct stat*)stat_buf);
        }
        else
        {
            TRACE_SYSCALLN("(%s) -> linux fs", path);
            char tmp[PATH_MAX];
            const auto newp = vfs::npath_translate(path,tmp);
            return real::__xstat64(ver,newp,stat_buf);
        }
    }
    __asm__(".symver _iosys_xstat64,__xstat64@GLIBC_2.2.5");

    int _iosys_lxstat64(int ver, const char * path, struct stat64 * stat_buf)
    {
        if(vfs::redirect_to_image(path))
        {
            TRACE_SYSCALLN("(%s) -> VFS", path);
            return vfs::invoke_fs(&fs::stat, path, *(struct stat*)stat_buf);
        }
        else
        {
            TRACE_SYSCALLN("(%s) -> linux fs", path);
            char tmp[PATH_MAX];
            const auto newp = vfs::npath_translate(path,tmp);
            return real::__lxstat64(ver,newp,stat_buf);
        }
    }
    __asm__(".symver _iosys_lxstat64,__lxstat64@GLIBC_2.2.5");

    int _iosys_fxstat64(int ver, int fildes, struct stat64 * stat_buf)
    {
        if(vfs::is_image_fd(fildes))
        {
            TRACE_SYSCALLN("(%d) -> VFS", fildes);
            return vfs::invoke_fs(&fs::fstat, fildes, *(struct stat*)stat_buf);
        }
        else
        {
            TRACE_SYSCALLN("(%d) -> linux fs", fildes);
            return real::__fxstat64(ver,fildes,stat_buf);
        }
    }
    __asm__(".symver _iosys_fxstat64,__fxstat64@GLIBC_2.2.5");

    int _iosys_fxstatat64(int vers, int fd, const char *filename, struct stat64 *buf, int flag)
    {
        errno = ENOSYS;
        std::cerr << "Unsupported syscall " << __PRETTY_FUNCTION__ << std::endl;
        return -1;
    }
    __asm__(".symver _iosys_fxstatat64,__fxstatat64@GLIBC_2.4");

    int _iosys_mount (const char *special_file, const char *dir,
		  const char *fstype, unsigned long int rwflag,
		  const void *data)
    {
        if(vfs::redirect_to_image(dir))
        {
            return vfs::invoke_fs(&fs::mount, special_file, dir, fstype, rwflag );
        }
        else
        {
            //NOTE: Always return zero because we will use fileystem which is always RW
            TRACE_SYSCALLN("(%s, %s, %s, %08lx,%p) -> linux fs", special_file, dir,fstype, rwflag, data);
            return 0;
        }
    }
    __asm__(".symver _iosys_mount,mount@GLIBC_2.2.5");

    int _iosys_umount (const char *mount_point)
    {
        if(vfs::redirect_to_image(mount_point))
        {
            return vfs::invoke_fs(&fs::umount, mount_point );
        }
        else
        {
            //NOTE: Always return zero because we will use fileystem which is always RW
            TRACE_SYSCALLN("(%s) -> linux fs", mount_point);
            return 0;
        }
    }
    __asm__(".symver _iosys_umount,umount@GLIBC_2.2.5");

}
