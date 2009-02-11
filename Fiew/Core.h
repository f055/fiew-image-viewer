#include <math.h>

#include "FwCHAR.h"
#include "List.h"
#include "XUn.h"

#include "Interface.h"
#include "Explorer.h"
#include "Cacher.h"
#include "Layer.h"
#include "Drawer.h"

class Core
{
private:
	HWND windowHandle;
	HINSTANCE instance;

	Interface *gui;
	Explorer *explorer;
	Cacher *cacher;
	Drawer *drawer;

	FwCHAR *commandLine;

	bool initialized;

	static HMODULE rarDll;

public:
	Core(WCHAR *cmdLine, HINSTANCE instance);
	~Core();

	LRESULT processMessages(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	HWND getWindowHandle();
	HINSTANCE getInstance();

	Interface *getGui();
	Explorer *getExplorer();
	Cacher *getCacher();
	Drawer *getDrawer();

	bool open(FwCHAR *path);
	bool open(WCHAR *path);
	void extract();
	void setwall();
	void close();

	void messageBox_Info(WCHAR *string);
	void messageBox_Error(WCHAR *string);

	static HMODULE getRarDll();
	static void getLastError();
	static int getEncoder(WCHAR* format, CLSID* pClsid);

private:
	void initialize(HWND windowHandle);
	bool extractResource(WORD id, WCHAR *filename);

	void loadDlls();
	void freeDlls();

	void destroy();
};