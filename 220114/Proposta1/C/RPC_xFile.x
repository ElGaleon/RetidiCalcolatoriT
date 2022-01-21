#define MAXS 256
#define MAXFILE 6

struct File{
	string fileName<MAXS;
};

struct Output {
	File files [MAXFILE];
};

  
program RPC_XFILES {
	version RPC_XFILESVERSION {         
		int ELIMINA_OCCORRENZE(File) = 1;        
        Output LISTA_FILETESTO(File) = 2;
	} = 1;
} = 0x20000013;

