#!/bin/bash
rpcgen rfs.x    
#I will test rpcgen -M rfs.x later for thread safe mode
gcc -ggdb -c rfs_clnt.c rfs_xdr.c rfs_svc.c 
gcc -ggdb -c -W -Wall client.c server.c 
gcc client.o rfs_clnt.o rfs_xdr.o -o client -lnsl
gcc server.o rfs_svc.o rfs_xdr.o -o server -lnsl
