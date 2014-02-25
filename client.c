#include <stdio.h>
#include <errno.h>
#include "rfs.h"

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

    errno = result->rerrno;
    stream = result->stream;
    xdr_free((xdrproc_t)xdr_rfs_fopen_out_t, (char*)result);
    return stream;
}

size_t rfs_fread(void *ptr, size_t size, size_t nmemb, RFILE stream)
{
    size_t ret_fread;
    rfs_fread_in_t input;
    rfs_fread_out_t *result;

    input.size = size;
    input.nmemb = nmemb;
    input.stream = stream;
    result = rfs_fread_1(&input, rfs_client);
    errno = result->rerrno;
    ret_fread = result->ret_fread;
    memcpy(ptr, result->buff.buff_val, result->buff.buff_len);

    xdr_free((xdrproc_t)xdr_rfs_fread_out_t, (char*)result);
    return ret_fread;
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
    errno = result->rerrno;
    ret_fwrite = result->ret_fwrite;

    xdr_free((xdrproc_t)xdr_rfs_fwrite_out_t, (char*)result);
    return ret_fwrite;
}

int rfs_fclose(RFILE stream)
{
    int ret_fclose;
    rfs_fclose_in_t input;
    rfs_fclose_out_t *result;

    input.stream = stream;
    result = rfs_fclose_1(&input, rfs_client);

    errno = result->rerrno;
    ret_fclose = result->ret_fclose;
    xdr_free((xdrproc_t)xdr_rfs_fclose_out_t, (char*)result);
    return ret_fclose;
}

CLIENT * rfs_init_rfs(const char *server)
{
    CLIENT  *client;
    client = clnt_create(server, RPC_RFS_PROGRAM, RPC_RFS_VER, "tcp");
    if(client == NULL) {
        clnt_pcreateerror (server);
    }
    return client;
}

int rfs_close_rfs(CLIENT *client)
{
    clnt_destroy(client);
    return 0;
}

int main()
{
    extern CLIENT *rfs_client;
    rfs_client = rfs_init_rfs("localhost");

    RFILE fp;
    fp = rfs_fopen("test.dat", "w");
    printf("RFILE fp = %ld\n", fp);   fflush(0);

    rfs_close_rfs(rfs_client);
    return 0;
}
