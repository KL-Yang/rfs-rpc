#!/bin/bash
rpcgen rfs_rpc.x    
#I will test rpcgen -M rfs.x later for thread safe mode
gcc -ggdb -c rfs_rpc_clnt.c rfs_rpc_xdr.c rfs_rpc_svc.c server.c
gcc -ggdb -c -W -Wall client.c test.c
ar cr librfs_simple.a client.o rfs_rpc_clnt.o rfs_rpc_xdr.o
gcc test.o -o client librfs_simple.a -lnsl
gcc server.o rfs_rpc_svc.o rfs_rpc_xdr.o -o server -lnsl
