#include "pvfs2.h"
#include "pvfs2-hint.h"

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

typedef struct file_object_s pfile;
/*
{
    struct options  user_opts;
    file_object src ;`
};*/

int pvfsInit();
int pvfsDestory();
int pvfsopen(pfile * ,char *, char *);
int pvfsclose(pfile *);
int pvfsread(pfile *, char * ,int ,int);
int pvfswrite(pfile *, char * ,int ,int);
