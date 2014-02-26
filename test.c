#include "rfs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    rfs_init_rfs("localhost");

    RFILE fp;
    fp = rfs_fopen("test.dat", "w");
    printf("RFILE fp = %ld\n", fp);   fflush(0);

    char *msg = "Hello\nWorld\nRPC!\n";
    rfs_fwrite(msg, sizeof(char), strlen(msg), fp);

    rfs_fclose(fp);

    rfs_close_rfs();
    return 0;
}
