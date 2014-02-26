#include <stdio.h>
#include <errno.h>
#include "rfs.h"
#include "rfs_rpc.h"

static CLIENT *rfs_client;

RFILE rfs_fopen(const char *path, const char *mode)
{
    RFILE stream;
    rfs_fopen_in_t input;
    rfs_fopen_out_t *result;

    input.path = malloc((strlen(path)+1)*sizeof(char));
    input.mode = malloc((strlen(mode)+1)*sizeof(char));
    strcpy(input.path, path);
    strcpy(input.mode, mode);
    result = rfs_fopen_1(&input, rfs_client);
    free(input.path);
    free(input.mode);

    if(result!=NULL) {
        errno = result->rerrno;
        stream = result->stream;
        xdr_free((xdrproc_t)xdr_rfs_fopen_out_t, (char*)result);
        return stream;
    } else return -1;
}

size_t rfs_fread(void *ptr, size_t size, size_t nmemb, RFILE stream)
{
    size_t ret_fread;
    rfs_fread_in_t input;
    rfs_fread_out_t *result;

    input.size = size;
    input.nmemb = nmemb;
    input.stream = stream;

    if((result=rfs_fread_1(&input, rfs_client))!=NULL) {
        errno = result->rerrno;
        ret_fread = result->ret_fread;
        memcpy(ptr, result->buff.buff_val, result->buff.buff_len);
        xdr_free((xdrproc_t)xdr_rfs_fread_out_t, (char*)result);
        return ret_fread;
    } else return 0;
}

size_t rfs_fwrite(const void *ptr, size_t size, size_t nmemb, RFILE stream)
{
    size_t ret_fwrite;
    rfs_fwrite_in_t input;
    rfs_fwrite_out_t *result;

    input.size = size;
    input.nmemb = nmemb;
    input.stream = stream;
    input.buff.buff_len = size*nmemb;
    input.buff.buff_val = malloc(input.buff.buff_len);
    memcpy(input.buff.buff_val, ptr, input.buff.buff_len);
    result = rfs_fwrite_1(&input, rfs_client);
    free(input.buff.buff_val);

    if(result!=NULL) {
        errno = result->rerrno;
        ret_fwrite = result->ret_fwrite;
        xdr_free((xdrproc_t)xdr_rfs_fwrite_out_t, (char*)result);
        return ret_fwrite;
    } else return 0;
}

int rfs_fclose(RFILE stream)
{
    int ret_fclose;
    rfs_fclose_in_t input;
    rfs_fclose_out_t *result;

    input.stream = stream;
    if((result=rfs_fclose_1(&input, rfs_client))!=NULL) {
        errno = result->rerrno;
        ret_fclose = result->ret_fclose;
        xdr_free((xdrproc_t)xdr_rfs_fclose_out_t, (char*)result);
        return ret_fclose;
    } else return EOF;
}

void rfs_init_rfs(const char *server)
{
    extern CLIENT *rfs_client;
    if(rfs_client==NULL) {
        rfs_client = clnt_create(server, RPC_RFS_PROGRAM, RPC_RFS_VER, "tcp");
        if(rfs_client == NULL) 
          clnt_pcreateerror (server);
    } else
      fprintf(stderr, "client already connected!\n"
              "your %s request ignored!\n", __func__);
}

void rfs_close_rfs()
{
    extern CLIENT *rfs_client;
    clnt_destroy(rfs_client);
    rfs_client = NULL;
}
