/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "RPC_xFile.h"

bool_t
xdr_Input (XDR *xdrs, Input *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->descrizione, 256))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->numero_biglietti))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Evento (XDR *xdrs, Evento *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->descrizione, 256))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->tipo, 256))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->data, 256))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->luogo, 256))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->disponibilita, 256))
		 return FALSE;
	 if (!xdr_string (xdrs, &objp->prezzo, 256))
		 return FALSE;
	return TRUE;
}
