#define UNDEFINED -1

class FwCHAR;
class Core;
class Cell;

class File
{
protected:
	FwCHAR *filepath;
	FwCHAR *filename;
	int type;
	int mime;
	DWORD size;
	bool archived;

public:
	File(FwCHAR *path, int type = UNDEFINED, DWORD size = NULL, bool archived = false);
	virtual ~File();

	virtual void initSort(int sort = HEADSORT);

	FwCHAR *getFilePath();
	FwCHAR *getFileName();

	int getType();
	int getMime();
	DWORD getSize();

	bool isArchived();
};

class Catalog : public File
{
protected:
	List<File> *files;

	int sortSide;
	int count;
	int idx;

	HANDLE mut_list, mut_loop, mut_step, mut_terminator;
	HANDLE thrd_sorting;

public:
	Catalog(FwCHAR *path, int type = UNDEFINED, DWORD size = NULL);
	Catalog(FwCHAR *path, FwCHAR *filepath, int type = UNDEFINED);
	virtual ~Catalog();

	virtual void init(FwCHAR *thatpath = NULL);
	virtual void initSort(int sort = HEADSORT);

	virtual bool next(int id = NULL);
	virtual bool prev(int id = NULL);

	virtual Cell *load(int id = NULL);

	virtual bool isThatHead();
	virtual bool isThatTail();

	virtual int getCount();
	List<File> *getFiles();
	int getIdx();

	virtual void list(Core *core);

	static DWORD WINAPI trigSort(LPVOID param);
};

class Archive : public Catalog
{
private:
	XUn *pack;
	List<File> *unsortedFiles;

	HANDLE sem_sorted;
	bool sorted, terminate;
	
public:
	Archive(FwCHAR *path, int type = UNDEFINED, DWORD size = NULL);
	~Archive();

	void init(FwCHAR *thatpath = NULL);
	void initSort(int sort = HEADSORT);

	List<File> *getUnsortedFiles();
	int getCount();

	void setSorted();
	bool isSorted();

	bool extract();
	bool extract(File *file);
	void list(Core *core);

	bool next(int id = NULL);
	bool prev(int id = NULL);

	Cell *load(int id = NULL);

	bool isThatHead();
	bool isThatTail();

	static DWORD WINAPI sort(LPVOID param);
	static DWORD WINAPI sortStep(LPVOID param);
};

/* *** */

class Explorer
{
private:
	Core *core;

	Catalog *root;

public:
	Explorer(Core *core);
	~Explorer();

	bool browse(FwCHAR *path);

	FwCHAR *getArchivePath();
	Catalog *getRoot();

	static int getMime(FwCHAR *path);
	static int getMime(int type);

	static int getType(FwCHAR *path);
	static int getType(WCHAR *name);

	void reset();
	bool extract();

	bool isArchived();

private:
	Archive *getArchive();
};