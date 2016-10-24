#ifndef _LINUX_FIEMAP_H
# define _LINUX_FIEMAP_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
//#include "fiemap.h" //This is from https://github.com/coreutils/coreutils/blob/df88fce71651afb2c3456967a142db0ae4bf9906/src/fiemap.h
# include <stdint.h>

struct fiemap_extent
{
  /* Logical offset in bytes for the start of the extent
 *      from the beginning of the file.  */
  uint64_t fe_logical;

  /* Physical offset in bytes for the start of the extent
 *      from the beginning of the disk.  */
  uint64_t fe_physical;

  /* Length in bytes for this extent.  */
  uint64_t fe_length;

  uint64_t fe_reserved64[2];

  /* FIEMAP_EXTENT_* flags for this extent.  */
  uint32_t fe_flags;

  uint32_t fe_reserved[3];
};

struct fiemap
{
  /* Logical offset(inclusive) at which to start mapping(in).  */
  uint64_t fm_start;

  /* Logical length of mapping which userspace wants(in).  */
  uint64_t fm_length;

  /* FIEMAP_FLAG_* flags for request(in/out).  */
  uint32_t fm_flags;

  /* Number of extents that were mapped(out).  */
  uint32_t fm_mapped_extents;

  /* Size of fm_extents array(in).  */
  uint32_t fm_extent_count;

  uint32_t fm_reserved;

  /* Array of mapped extents(out).
 *      This is protected by the ifdef because it uses non standard
 *           zero length arrays.  Note C99 has the equivalent flexible arrays,
 *                but we don't use those for maximum portability to older systems.  */
# ifdef __linux__
  struct fiemap_extent fm_extents[0];
# endif
};

/* The maximum offset can be mapped for a file.  */
# define FIEMAP_MAX_OFFSET       (~0ULL)

/* Sync file data before map.  */
# define FIEMAP_FLAG_SYNC        0x00000001

/* Map extented attribute tree.  */
# define FIEMAP_FLAG_XATTR       0x00000002

# define FIEMAP_FLAGS_COMPAT     (FIEMAP_FLAG_SYNC | FIEMAP_FLAG_XATTR)

/* Last extent in file.  */
# define FIEMAP_EXTENT_LAST              0x00000001

/* Data location unknown.  */
# define FIEMAP_EXTENT_UNKNOWN           0x00000002

/* Location still pending, Sets EXTENT_UNKNOWN.  */
# define FIEMAP_EXTENT_DELALLOC          0x00000004

/* Data cannot be read while fs is unmounted.  */
# define FIEMAP_EXTENT_ENCODED           0x00000008

/* Data is encrypted by fs.  Sets EXTENT_NO_BYPASS.  */
# define FIEMAP_EXTENT_DATA_ENCRYPTED    0x00000080

/* Extent offsets may not be block aligned.  */
# define FIEMAP_EXTENT_NOT_ALIGNED       0x00000100

/* Data mixed with metadata.  Sets EXTENT_NOT_ALIGNED.  */
# define FIEMAP_EXTENT_DATA_INLINE       0x00000200

/* Multiple files in block.  Set EXTENT_NOT_ALIGNED.  */
# define FIEMAP_EXTENT_DATA_TAIL         0x00000400

/* Space allocated, but not data (i.e., zero).  */
# define FIEMAP_EXTENT_UNWRITTEN         0x00000800

/* File does not natively support extents.  Result merged for efficiency.  */
# define FIEMAP_EXTENT_MERGED		0x00001000

/* Space shared with other files.  */
# define FIEMAP_EXTENT_SHARED            0x00002000

#endif

//#include <stdio.h>
//#include <string.h>
//#include <stdlib.h>
//#include <sys/fcntl.h>
//#include <errno.h>
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <unistd.h>
//#include <sys/ioctl.h>
//#include <linux/fs.h>
//#include "fiemap.h" //This is from https://github.com/coreutils/coreutils/blob/df88fce71651afb2c3456967a142db0ae4bf9906/src/fiemap.h

int main(int argc, char* argv[]) {

    int input_fd;

    if(argc != 2){
        printf ("Usage: ioctl file1");
        return 1;
    }

    /* Create input file descriptor */
    input_fd = open (argv [1], O_RDWR);
    if (input_fd < 0) {
            perror ("open");
            return 2;
    }

    union { struct fiemap f; char c[4096]; } fiemap_buf;
    struct fiemap *fiemap = &fiemap_buf.f;
    int s = ioctl(input_fd, FS_IOC_FIEMAP, fiemap);

    if (s == 0) {
        printf("ioctl success\n");
    } else {
        printf("ioctl failure\n");
        char * errmsg = strerror(errno);
        printf("error: %d %s\n", errno, errmsg);
    }

    /* Close file descriptors */
    close (input_fd);

    return s;
}
