typedef int64_t RFILE;

/* fopen */
struct rfs_fopen_in_t
{
	string path<>;
	string mode<>;
};
struct rfs_fopen_out_t
{
    int     rerrno;
    RFILE   stream;
};

/* fread */
struct rfs_fread_in_t
{
    int64_t size;
    int64_t nmemb;
    RFILE   stream;
};
struct rfs_fread_out_t
{
    int     rerrno;
    int64_t ret_fread;
    opaque  buff<>;
};

/* fwrite */
struct rfs_fwrite_in_t
{
    int64_t size;
    int64_t nmemb;
    int64_t stream;
    opaque  buff<>;
};
struct rfs_fwrite_out_t
{
    int     rerrno;
    int64_t ret_fwrite;
};

/* fclose */
struct rfs_fclose_in_t
{
    int64_t stream;
};
struct rfs_fclose_out_t
{
    int     rerrno;
    int     ret_fclose;
};

/* ftell */
/* fseek */
/* fflush */

program RPC_RFS_PROGRAM {
    version RPC_RFS_VER {
        rfs_fopen_out_t  RFS_FOPEN(rfs_fopen_in_t)    = 1;
        rfs_fread_out_t  RFS_FREAD(rfs_fread_in_t)    = 2;
        rfs_fwrite_out_t RFS_FWRITE(rfs_fwrite_in_t)  = 3;
        rfs_fclose_out_t RFS_FCLOSE(rfs_fclose_in_t)  = 4;
    } = 1;
} = 0x20000003;
