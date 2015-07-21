#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <libgen.h>
#include <getopt.h>

#include "pvfs2.h"
#include "pvfs2-hint.h"

/* optional parameters, filled in by parse_args() */

struct options
{
    PVFS_size strip_size;
    int num_datafiles;
    int buf_size;
    char* srcfile;
    char* destfile;
    int show_timings;
};

enum object_type { 
    UNIX_FILE = 1,
    PVFS2_FILE 
};

enum open_type {
    OPEN_SRC,
    OPEN_DEST
};

typedef struct pvfs2_file_object_s {
    PVFS_fs_id fs_id;
    PVFS_object_ref ref;
    char pvfs2_path[PVFS_NAME_MAX];	
    char user_path[PVFS_NAME_MAX];
    PVFS_sys_attr attr;
    PVFS_permissions perms;
} pvfs2_file_object;

typedef struct unix_file_object_s {
    int fd;
    int mode;
    char path[NAME_MAX];
} unix_file_object;

typedef struct file_object_s {
    int fs_type;
    union {
	unix_file_object ufs;
	pvfs2_file_object pvfs2;
    } u;
} file_object;

struct options* parse_args(int argc, char* argv);
void usage(int argc, char** argv);
double Wtime(void);
void print_timings( double time, int64_t total);
int resolve_filename(file_object *obj, char *filename);
int generic_open(file_object *obj, PVFS_credentials *credentials, 
	int nr_datafiles, PVFS_size strip_size, char *srcname, int open_type);
size_t generic_read(file_object *src, char *buffer, 
	int64_t offset, size_t count, PVFS_credentials *credentials);
size_t generic_write(file_object *dest, char *buffer, 
	int64_t offset, size_t count, PVFS_credentials *credentials);
int generic_cleanup(file_object *src, file_object *dest,
                           PVFS_credentials *credentials);
void make_attribs(PVFS_sys_attr *attr,
                         PVFS_credentials *credentials, 
                         int nr_datafiles, int mode);

int convert_pvfs2_perms_to_mode(PVFS_permissions perms);
int pvfsread (int argc, char * argv);
