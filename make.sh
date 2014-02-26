#!/bin/bash
rpcgen rfs_rpc.x    
#I will test rpcgen -M rfs.x later for thread safe mode
gcc -ggdb -c rfs_rpc_clnt.c rfs_rpc_xdr.c rfs_rpc_svc.c server.c
gcc -ggdb -c -W -Wall client.c test.c
gcc client.o test.o rfs_rpc_clnt.o rfs_rpc_xdr.o -o client -lnsl
gcc server.o rfs_rpc_svc.o rfs_rpc_xdr.o -o server -lnsl
