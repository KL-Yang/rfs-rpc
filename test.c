#include "rfs.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    rfs_init_rfs("localhost");

    RFILE fp;
    fp = rfs_fopen("test.dat", "w");
    printf("RFILE fp = %ld\n", fp);   fflush(0);

    rfs_close_rfs();
    return 0;
}
