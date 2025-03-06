#ifndef	MEM_MANANGER_HEADER
#define	MEM_MANANGER_HEADER

#define		DEFAULT_MEMORY_SIZE		4096

#define		STRAT_FIRST		0x01
#define		STRAT_BEST		0x02
#define		STRAT_WORST		0x03

#define		ACTION_ALLOCATE	0x10
#define		ACTION_RELEASE	0x20

typedef struct mmgr_action {
	int type;
	int id;
	int size;
	char paint;
} mmgr_action;

int runModel(FILE *outputfp, FILE *inputfp,
		long numberOfBytes, int fitStrategy,
		int verbosity);

int getAction(mmgr_action *action, FILE *inputfp,
		FILE *outputfp, int verbosity);
int printAction(FILE *outputfp, mmgr_action *action);

void writeAction(char *memoryBlock, long location, mmgr_action action);
void writeNext(char *memoryBlock, long location, long next);
int readId(char *memoryBlock, long location);
int readSize(char *memoryBlock, long location);
long readNext(char *memoryBlock, long location);
void paintMemory(char *memoryBlock, long location, int size, char paintCharacter);

#endif

