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


struct Input {
	char *descrizione;
	int numero_biglietti;
};
typedef struct Input Input;

struct Evento {
	char *descrizione;
	char *tipo;
	char *data;
	char *luogo;
	char *disponibilita;
	char *prezzo;
};
typedef struct Evento Evento;

#define TICKETTWO 0x20000013
#define TICKETTWOVERSION 1

#if defined(__STDC__) || defined(__cplusplus)
#define INSERIMENTO_EVENTO 1
extern  int * inserimento_evento_1(Evento *, CLIENT *);
extern  int * inserimento_evento_1_svc(Evento *, struct svc_req *);
#define ACQUISTA_BIGLIETTI 2
extern  int * acquista_biglietti_1(Input *, CLIENT *);
extern  int * acquista_biglietti_1_svc(Input *, struct svc_req *);
extern int tickettwo_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define INSERIMENTO_EVENTO 1
extern  int * inserimento_evento_1();
extern  int * inserimento_evento_1_svc();
#define ACQUISTA_BIGLIETTI 2
extern  int * acquista_biglietti_1();
extern  int * acquista_biglietti_1_svc();
extern int tickettwo_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_Input (XDR *, Input*);
extern  bool_t xdr_Evento (XDR *, Evento*);

#else /* K&R C */
extern bool_t xdr_Input ();
extern bool_t xdr_Evento ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_RPC_XFILE_H_RPCGEN */