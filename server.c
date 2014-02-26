#include <stdio.h>
#include <errno.h>
#include "rfs_rpc.h"

rfs_fopen_out_t *
rfs_fopen_1_svc(rfs_fopen_in_t *input, struct svc_req *req)
{
    static rfs_fopen_out_t result;
    xdr_free((xdrproc_t)xdr_rfs_fopen_out_t, (char*)&result);
    FILE *stream;
    char *path, *mode;
    path = input->path; 
    mode = input->mode;
    stream = fopen(path, mode);
    result.rerrno = errno;
    result.stream = (int64_t)stream;
    return (&result);
}

rfs_fread_out_t *
rfs_fread_1_svc(rfs_fread_in_t *input, struct svc_req *req)
{
    static rfs_fread_out_t result;
    xdr_free((xdrproc_t)xdr_rfs_fread_out_t, (char*)&result);
    FILE *stream;
    size_t size, nmemb, ret_fread;
    stream = (FILE*)input->stream;
    size = input->size;
    nmemb = input->nmemb;
    result.buff.buff_len = size*nmemb;
    result.buff.buff_val = malloc(result.buff.buff_len);
    ret_fread = fread(result.buff.buff_val, size, nmemb, stream);
    result.rerrno = errno;
    result.ret_fread = ret_fread;
    return (&result);
}

rfs_fwrite_out_t *
rfs_fwrite_1_svc(rfs_fwrite_in_t *input, struct svc_req *req)
{
    static rfs_fwrite_out_t result;
    xdr_free((xdrproc_t)xdr_rfs_fwrite_out_t, (char*)&result);
    FILE *stream;
    size_t size, nmemb, ret_fwrite;
    stream = (FILE*)input->stream;
    size = input->size;
    nmemb = input->nmemb;
    ret_fwrite = fwrite(input->buff.buff_val, size, nmemb, stream);
    result.rerrno = errno;
    result.ret_fwrite = ret_fwrite;
    return (&result);
}

rfs_fclose_out_t *
rfs_fclose_1_svc(rfs_fclose_in_t *input, struct svc_req *req)
{
    static rfs_fclose_out_t result;
    xdr_free((xdrproc_t)xdr_rfs_fclose_out_t, (char*)&result);
    FILE *stream;
    int ret_fclose;
    stream = (FILE*)input->stream;
    ret_fclose = fclose(stream);
    result.rerrno = errno;
    result.ret_fclose = ret_fclose;
    return (&result);
}
