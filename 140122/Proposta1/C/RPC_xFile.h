/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _RPC_XFILE_H_RPCGEN
#define _RPC_XFILE_H_RPCGEN

#include <rpc/rpc.h>


#ifdef __cplusplus
extern "C" {
#endif


struct File {
	char *fileName;
};
typedef struct File File;

struct Output {
	File files[6];
};
typedef struct Output Output;

#define RPC_XFILES 0x20000013
#define RPC_XFILESVERSION 1

#if defined(__STDC__) || defined(__cplusplus)
#define ELIMINA_OCCORRENZE 1
extern  int * elimina_occorrenze_1(File *, CLIENT *);
extern  int * elimina_occorrenze_1_svc(File *, struct svc_req *);
#define LISTA_FILETESTO 2
extern  Output * lista_filetesto_1(File *, CLIENT *);
extern  Output * lista_filetesto_1_svc(File *, struct svc_req *);
extern int rpc_xfiles_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define ELIMINA_OCCORRENZE 1
extern  int * elimina_occorrenze_1();
extern  int * elimina_occorrenze_1_svc();
#define LISTA_FILETESTO 2
extern  Output * lista_filetesto_1();
extern  Output * lista_filetesto_1_svc();
extern int rpc_xfiles_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_File (XDR *, File*);
extern  bool_t xdr_Output (XDR *, Output*);

#else /* K&R C */
extern bool_t xdr_File ();
extern bool_t xdr_Output ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_RPC_XFILE_H_RPCGEN */
