/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "RPC_xFile.h"

bool_t
xdr_File (XDR *xdrs, File *objp)
{
	register int32_t *buf;

	 if (!xdr_string (xdrs, &objp->fileName, 256))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_Output (XDR *xdrs, Output *objp)
{
	register int32_t *buf;

	int i;
	 if (!xdr_vector (xdrs, (char *)objp->files, 6,
		sizeof (File), (xdrproc_t) xdr_File))
		 return FALSE;
	return TRUE;
}