#include "stdafx.h"
#include "Core.h"

HMODULE Core::rarDll = NULL;

Core::Core(WCHAR *cmdLine, HINSTANCE instance)
{
	this->instance = instance;
	this->windowHandle = NULL;
	this->commandLine = NULL;
	this->loadDlls();

	this->explorer = new Explorer(this);

	this->cacher = NULL;
	this->drawer = NULL;
	this->gui = NULL;

	if( cmdLine != NULL ){
		this->commandLine = new FwCHAR(cmdLine);
		if( this->commandLine->toLength() < 3 )
			this->commandLine = NULL;
	}

	this->initialized = false;
}

Core::~Core()
{
	this->destroy();
}

void Core::destroy()
{
	KillTimer(this->windowHandle,TIMER_MBB);
	KillTimer(this->windowHandle,TIMER_MCH);
	KillTimer(this->windowHandle,TIMER_MMM);
	KillTimer(this->windowHandle,TIMER_OVL);
	KillTimer(this->windowHandle,TIMER_THB);

	if( this->drawer != NULL )
		delete this->drawer;
	this->drawer = NULL;

	if( this->cacher != NULL )
		delete this->cacher;
	this->cacher = NULL;

	if( this->explorer != NULL )
		delete this->explorer;
	this->explorer = NULL;

	if( this->gui != NULL )
		delete this->gui;
	this->gui = NULL;

	if( this->commandLine != NULL )
		//delete this->commandLine;
	this->commandLine = NULL;

	this->freeDlls();
}

HMODULE Core::getRarDll()
{
	return Core::rarDll;
}

void Core::getLastError()
{
	TCHAR szBuf[80]; 
    LPVOID lpMsgBuf;
    DWORD dw = GetLastError(); 

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | 
        FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL );

    wsprintf(szBuf,L"Failed with error %d: %s",dw,lpMsgBuf);

	return;
}

void Core::initialize(HWND windowHandle)
{
	this->windowHandle = windowHandle;

	this->gui = new Interface(this);
	this->drawer = new Drawer(this);
	this->drawer->reset();

	this->open(this->commandLine);

	this->initialized = true;
}

bool Core::open(FwCHAR *path)
{
	bool result = false;

	if( path == NULL )
		return result;
	if( path->toWCHAR() == NULL )
		return result;
	path->stripBraces();
	
	this->gui->setMessage(MESSAGE_PROCESS);
	this->gui->setCursor(CURSOR_WAIT);
	if( this->explorer->browse(path) ){
		if( this->cacher != NULL )
			delete this->cacher;
		this->cacher = new Cacher(this);

		this->drawer->reset();

		result = true;
	}
	this->gui->setCursor(CURSOR_CLIENT);
	this->gui->update();

	return result;
}
bool Core::open(WCHAR *path)
{
	if( path == NULL )
		return false;

	FwCHAR *tmp = new FwCHAR(path);
	return this->open(tmp);
}

void Core::extract()
{
	bool result = false;

	this->gui->setMessage(MESSAGE_EXTRACT);
	this->gui->setCursor(CURSOR_WAIT);
	result = this->explorer->extract();
	this->gui->setCursor(CURSOR_CLIENT);
	this->gui->updateText();

	if( result == false )
		this->messageBox_Error(MESSAGE_EXTRACTERROR);
	else
		this->messageBox_Info(MESSAGE_EXTRACTEDIMAGE);
}

void Core::setwall()
{
	bool result = false;

	this->gui->setMessage(MESSAGE_PROCESS);
	this->gui->setCursor(CURSOR_WAIT);
	result = this->drawer->getScene()->setWall();
	this->gui->setCursor(CURSOR_CLIENT);
	this->gui->updateText();

	if( result == false )
		this->messageBox_Error(MESSAGE_SETWALLERROR);
	else
		this->messageBox_Info(MESSAGE_SETWALLEDIMAGE);
}

void Core::close()
{
	if( this->cacher != NULL )
		delete this->cacher;
	this->cacher = NULL;

	this->drawer->reset();
	this->explorer->reset();
	this->gui->update();
}

bool Core::extractResource(WORD id, WCHAR *filename)
{
	bool result = false;

	HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(id), RC_DLL);
	if( hResource != NULL ){
		HGLOBAL hLoader = LoadResource(NULL,hResource);
		if( hLoader != NULL ){
			byte *data = (byte *)LockResource(hLoader);
			DWORD len = SizeofResource(NULL,hResource);

			if( data != NULL && len > 0 ){
				HANDLE hFile = CreateFile(filename,
										GENERIC_READ | GENERIC_WRITE,
										0,
										NULL,
										CREATE_ALWAYS,
										FILE_ATTRIBUTE_NORMAL,
										NULL);
				if( hFile != INVALID_HANDLE_VALUE ){
					DWORD written = 0;

					int write = WriteFile(hFile,
							data,
							len,
							&written,
							NULL);
					if( len == written && write == TRUE )
						result = true;
				}
				CloseHandle(hFile);
			}
			UnlockResource(hLoader);
		}
		FreeResource(hLoader);
	}
	return result;
}

void Core::loadDlls()
{
	Core::rarDll = LoadLibrary(RARDLL);
	if( Core::rarDll == NULL )
		if( this->extractResource(IDR_DLLRAR,RARDLL) )
			Core::rarDll = LoadLibrary(RARDLL);
}

void Core::freeDlls()
{
	if( Core::rarDll != NULL )
		FreeLibrary( Core::rarDll );
	Core::rarDll = NULL;
}

int Core::getEncoder(WCHAR* format, CLSID* pClsid)
{
	UINT num = 0; 
	UINT size = 0;

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;

	GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;
		}    
	}
	free(pImageCodecInfo);
	return -1;
}

HWND Core::getWindowHandle()
{
	return this->windowHandle;
}
HINSTANCE Core::getInstance()
{
	return this->instance;
}

Interface *Core::getGui()
{
	return this->gui;
}

Explorer *Core::getExplorer()
{
	return this->explorer;
}

Cacher *Core::getCacher()
{
	return this->cacher;
}

Drawer *Core::getDrawer()
{
	return this->drawer;
}

void Core::messageBox_Info(WCHAR *string)
{
	MessageBox(this->windowHandle,string,MESSAGE_INFO,MB_OK | MB_ICONINFORMATION);
}
void Core::messageBox_Error(WCHAR *string)
{
	MessageBox(this->windowHandle,string,MESSAGE_ERROR,MB_OK | MB_ICONERROR);
}

LRESULT Core::processMessages(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	Graphics *gfx;

	if( this->initialized == false ){
		switch (message)
		{
		case WM_CREATE:
			SetTimer(hWnd,TIMER_INT,INIT_TOUT,NULL);
			break;
		case WM_TIMER:
			KillTimer(hWnd,wParam);
			if( wParam == TIMER_INT )
				this->initialize(hWnd);
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);

			gfx = Graphics::FromHDC(hdc);
			gfx->Clear(CLR_WHITE);
			delete gfx;

			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			this->destroy();
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		return 0;
	}
	switch (message)
	{
	case WM_CREATE:
		this->initialize(hWnd);
		break;
	case WM_DROPFILES:
		this->gui->openDropFile(wParam);
		break;
	case WM_TIMER:
		if( wParam != TIMER_MMM )
			KillTimer(hWnd,wParam);
		switch(wParam){
			case TIMER_MBB:
				this->gui->unblockMBB();
				break;
			case TIMER_MCH:
				this->gui->setCursor();
				break;
			case TIMER_OVL:
				this->drawer->hideOverlay();
				this->gui->updateMenu();
				break;
			case TIMER_THB:
				this->drawer->updateOverlay();
				break;
			case TIMER_MMM:
				this->gui->movemMM();
				break;
		}
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		
		switch (wmId)
		{
			default:
				this->gui->processMenu(wmId);
				return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_KEYDOWN:
	case WM_KEYUP:
		this->gui->processKeys(message,wParam,lParam);
		break;
	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MOUSEWHEEL:
	case WM_MOUSEMOVE:
		this->gui->processMouse(message,wParam,lParam);
		break;
	case WM_NCHITTEST:
		return this->gui->processMouseState(lParam,
											DefWindowProc(hWnd,message,wParam,lParam));
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		this->drawer->paint(hdc);
		EndPaint(hWnd, &ps);
		break;
	//case WM_ACTIVATEAPP:
	case WM_SIZE:
		if( this->drawer != NULL )
			this->drawer->invalidate();
	case WM_ERASEBKGND:
		return 1;
	case WM_DESTROY:
		this->destroy();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}