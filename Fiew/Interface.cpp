#include "stdafx.h"
#include "Core.h"

#include "shlobj.h"

Interface::Interface(Core *core)
{
	this->core = core;

	this->mState = NULL;
	this->mButton = NULL;
	this->mX = this->mY = 0;
	this->dX = this->dY = 0;
	this->mBB = false;
	this->mWH = false;
	this->mMM = false;

	this->fullscreen = false;
	this->fullpath = false;
	this->menuvisible = true;
	this->menuheight = NULL;

	this->textmessage = NULL;

	this->windowLong = NULL;
	this->windowMenu = NULL;
	this->menuFile = NULL;
	this->menuView = NULL;
	this->menuHelp = NULL;

	this->cursor = LoadCursor(NULL,CURSOR_CLIENT);

	this->initLocale();
	this->update();
}

Interface::~Interface()
{

}

void Interface::initLocale()
{
	MENUITEMINFO mif;
	HMENU main, submain;
	HWND window = this->core->getWindowHandle();

	mif.cbSize = sizeof(MENUITEMINFO);
	this->windowMenu = LoadMenu(this->core->getInstance(),MAKEINTRESOURCE(IDR_MENU));

	// File
	mif.fMask = MIIM_SUBMENU;
	GetMenuItemInfo(this->windowMenu,0,true,&mif);
	main = mif.hSubMenu;
	this->menuFile = main;
	mif.fMask = MIIM_STRING;
	mif.fType = MFT_STRING;
	mif.dwTypeData = MENU_FILE;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(this->windowMenu,0,true,&mif);
	
	mif.dwTypeData = MENU_FILE_OPEN;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(main,0,true,&mif);
	mif.dwTypeData = MENU_FILE_OPENFOLDER;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(main,1,true,&mif);
	mif.dwTypeData = MENU_FILE_CLOSE;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(main,2,true,&mif);
	mif.dwTypeData = MENU_FILE_EXTRACT;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(main,4,true,&mif);
	mif.dwTypeData = MENU_FILE_SETWALL;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(main,5,true,&mif);
	mif.dwTypeData = MENU_FILE_EXIT;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(main,7,true,&mif);

	// View
	mif.fMask = MIIM_SUBMENU;
	GetMenuItemInfo(this->windowMenu,1,true,&mif);
	main = mif.hSubMenu;
	this->menuView = main;
	mif.fMask = MIIM_STRING;
	mif.fType = MFT_STRING;
	mif.dwTypeData = MENU_VIEW;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(this->windowMenu,1,true,&mif);

	mif.dwTypeData = MENU_VIEW_FULLSCREEN;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(main,0,true,&mif);
	mif.dwTypeData = MENU_VIEW_THUMBNAILS;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(main,2,true,&mif);
	mif.dwTypeData = MENU_VIEW_LIST;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(main,3,true,&mif);
	mif.dwTypeData = MENU_VIEW_FLOWSCROLL;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(main,9,true,&mif);
	// View_Fitto
	mif.fMask = MIIM_SUBMENU;
	GetMenuItemInfo(main,5,true,&mif);
	submain = mif.hSubMenu;
	mif.fMask = MIIM_STRING;
	mif.fType = MFT_STRING;
	mif.dwTypeData = MENU_VIEW_FITTO;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(main,5,true,&mif);

	mif.dwTypeData = MENU_VIEW_FITTO_SCREENOV;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(submain,0,true,&mif);
	mif.dwTypeData = MENU_VIEW_FITTO_SCREEN;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(submain,1,true,&mif);
	mif.dwTypeData = MENU_VIEW_FITTO_HEIGHT;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(submain,3,true,&mif);
	mif.dwTypeData = MENU_VIEW_FITTO_WIDTH;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(submain,4,true,&mif);
	mif.dwTypeData = MENU_VIEW_FITTO_NUMPAD;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(submain,5,true,&mif);
	mif.dwTypeData = MENU_VIEW_FITTO_LEFT;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(submain,7,true,&mif);
	mif.dwTypeData = MENU_VIEW_FITTO_RIGHT;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(submain,8,true,&mif);
	// View_Zoom
	mif.fMask = MIIM_SUBMENU;
	GetMenuItemInfo(main,6,true,&mif);
	submain = mif.hSubMenu;
	mif.fMask = MIIM_STRING;
	mif.fType = MFT_STRING;
	mif.dwTypeData = MENU_VIEW_ZOOM;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(main,6,true,&mif);

	mif.dwTypeData = MENU_VIEW_ZOOM_ZOOMIN;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(submain,9,true,&mif);
	mif.dwTypeData = MENU_VIEW_ZOOM_ZOOMOUT;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(submain,10,true,&mif);
	// View_Rotate
	mif.fMask = MIIM_SUBMENU;
	GetMenuItemInfo(main,7,true,&mif);
	submain = mif.hSubMenu;
	mif.fMask = MIIM_STRING;
	mif.fType = MFT_STRING;
	mif.dwTypeData = MENU_VIEW_ROTATE;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(main,7,true,&mif);

	mif.dwTypeData = MENU_VIEW_ROTATE_RESET;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(submain,4,true,&mif);

	// Help
	mif.fMask = MIIM_SUBMENU;
	GetMenuItemInfo(this->windowMenu,2,true,&mif);
	main = mif.hSubMenu;
	this->menuHelp = main;
	mif.fMask = MIIM_STRING;
	mif.fType = MFT_STRING;
	mif.dwTypeData = MENU_HELP;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(this->windowMenu,2,true,&mif);
	
	mif.dwTypeData = MENU_HELP_ABOUT;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(main,0,true,&mif);
	mif.dwTypeData = MENU_HELP_MANUAL;
	mif.cch = wcslen(mif.dwTypeData);
	SetMenuItemInfo(main,1,true,&mif);

	SetMenu(this->core->getWindowHandle(),this->windowMenu);
}

void Interface::update()
{
	this->updateMenu();
	this->updateText();
}
void Interface::updateMenu()
{
	Drawer *drawer = this->core->getDrawer();
	Explorer *explorer = this->core->getExplorer();

	Layer *scene = NULL;
	Layer *overlay = NULL;
	if( drawer != NULL ){
		scene = drawer->getScene();
		overlay = drawer->getOverlay();
	}

	UINT state = MF_GRAYED;
	if( scene != NULL )
		if( scene->isContent() == true )
			state = MF_ENABLED;

	EnableMenuItem(this->windowMenu,ID_FILE_CLOSE,state);
	EnableMenuItem(this->windowMenu,ID_FILE_SETASWALLPAPER,state);
	EnableMenuItem(this->windowMenu,ID_VIEW_THUMBNAILS,state);
	EnableMenuItem(this->windowMenu,ID_VIEW_LIST,state);
	EnableMenuItem(this->windowMenu,ID_VIEW_FLOWSCROLL,state);
	EnableMenuItem(this->menuView,5,state | MF_BYPOSITION);
	EnableMenuItem(this->menuView,6,state | MF_BYPOSITION);
	EnableMenuItem(this->menuView,7,state | MF_BYPOSITION);

	state = MF_GRAYED;
	if( explorer != NULL && scene != NULL )
		if( explorer->isArchived() == true && scene->isContent() == true )
			state = MF_ENABLED;
	EnableMenuItem(this->windowMenu,ID_FILE_EXTRACT,state);

	CheckMenuItem(this->windowMenu,ID_FITTO_SCREENIFOVERSIZED,MF_UNCHECKED);
	CheckMenuItem(this->windowMenu,ID_FITTO_SCREEN,MF_UNCHECKED);
	CheckMenuItem(this->windowMenu,ID_FITTO_WIDTH,MF_UNCHECKED);
	CheckMenuItem(this->windowMenu,ID_FITTO_HEIGHT,MF_UNCHECKED);
	CheckMenuItem(this->windowMenu,ID_FITTO_NUMPAD,MF_UNCHECKED);
	CheckMenuItem(this->windowMenu,ID_FITTO_LEFT,MF_UNCHECKED);
	CheckMenuItem(this->windowMenu,ID_FITTO_RIGHT,MF_UNCHECKED);

	int fitmode = NULL;
	if( scene != NULL )
		fitmode = scene->getFitmode();
	switch(fitmode){
		case FITSCREENOV:
			CheckMenuItem(this->windowMenu,ID_FITTO_SCREENIFOVERSIZED,MF_CHECKED);
			break;
		case FITSCREEN:
			CheckMenuItem(this->windowMenu,ID_FITTO_SCREEN,MF_CHECKED);
			break;
		case FITWIDTH:
			CheckMenuItem(this->windowMenu,ID_FITTO_WIDTH,MF_CHECKED);
			break;
		case FITHEIGHT:
			CheckMenuItem(this->windowMenu,ID_FITTO_HEIGHT,MF_CHECKED);
			break;
		case FITNUMPAD:
			CheckMenuItem(this->windowMenu,ID_FITTO_NUMPAD,MF_CHECKED);
			break;
	}
	int sidemode = NULL;
	if( scene != NULL )
		sidemode = scene->getSidemode();
	if( sidemode == LEFT )
		CheckMenuItem(this->windowMenu,ID_FITTO_LEFT,MF_CHECKED);
	else if( sidemode == RIGHT )
		CheckMenuItem(this->windowMenu,ID_FITTO_RIGHT,MF_CHECKED);

	bool sidedraw = false;
	if( scene != NULL )
		sidedraw = scene->getSidedraw();

	bool about = false;
	bool manual = false;
	bool thumbs = false;
	bool list = false;
	if( drawer != NULL ){
		about = drawer->isAbout();
		manual = drawer->isManual();
		thumbs = drawer->isThumbs();
		list = drawer->isList();
	}
	CheckMenuItem(this->windowMenu,ID_VIEW_FULLSCREEN,MF_UNCHECKED);

	CheckMenuItem(this->windowMenu,ID_VIEW_THUMBNAILS,MF_UNCHECKED);
	CheckMenuItem(this->windowMenu,ID_VIEW_LIST,MF_UNCHECKED);

	CheckMenuItem(this->windowMenu,ID_VIEW_FLOWSCROLL,MF_UNCHECKED);

	CheckMenuItem(this->windowMenu,ID_HELP_ABOUTFIEW,MF_UNCHECKED);
	CheckMenuItem(this->windowMenu,ID_HELP_MANUAL,MF_UNCHECKED);

	if( this->fullscreen == true )
		CheckMenuItem(this->windowMenu,ID_VIEW_FULLSCREEN,MF_CHECKED);
	if( thumbs == true )
		CheckMenuItem(this->windowMenu,ID_VIEW_THUMBNAILS,MF_CHECKED);
	if( list == true )
		CheckMenuItem(this->windowMenu,ID_VIEW_LIST,MF_CHECKED);
	if( sidedraw == true )
		CheckMenuItem(this->windowMenu,ID_VIEW_FLOWSCROLL,MF_CHECKED);
	if( about == true )
		CheckMenuItem(this->windowMenu,ID_HELP_ABOUTFIEW,MF_CHECKED);
	if( manual == true )
		CheckMenuItem(this->windowMenu,ID_HELP_MANUAL,MF_CHECKED);
}

void Interface::updateText()
{
	FwCHAR *text = new FwCHAR(APP_TITLE);

	Cacher *cacher = this->core->getCacher();
	Drawer *drawer = this->core->getDrawer();
	Explorer *explorer = this->core->getExplorer();

	Catalog *root = NULL;
	bool archived = false;
	if( explorer != NULL ){
		archived = explorer->isArchived();
		root = explorer->getRoot();
	}
	if( drawer != NULL )
		if( drawer->getScene() != NULL )
			if( drawer->getScene()->isContent() == true ){
				Layer *scene = drawer->getScene();
				File *file = scene->getFile();
				if( drawer->isList() == true || 
					drawer->isThumbs() == true )
					if( cacher != NULL )
						if( cacher->getThat() != NULL )
							if( cacher->getThat()->getFile() != NULL )
								file = cacher->getThat()->getFile();

				text->mergeWith(L" - \"");
				if( this->fullpath == true ){
					if( archived == true ){
						text->mergeWith(explorer->getArchivePath());
						text->mergeWith(L"\\~");
						text->mergeWith(file->getFilePath());
					}
					else
						text->mergeWith(file->getFilePath());
				}
				else {
					if( archived == true )
						text->mergeWith(L"~");
					text->mergeWith(file->getFileName());
				}
				if( drawer->isList() == false && 
					drawer->isThumbs() == false ){

					text->mergeWith(L"\" [");
					FwCHAR *width = new FwCHAR(scene->getImageWidth());
					FwCHAR *height = new FwCHAR(scene->getImageHeight());
					FwCHAR *zoom = new FwCHAR((int)(scene->getZoom() * 100));
					text->mergeWith(width);
					text->mergeWith(L"x");
					text->mergeWith(height);
					text->mergeWith(L" @ ");
					text->mergeWith(zoom);
					text->mergeWith(L"%] ");

					delete width;
					delete height;
					delete zoom;
				}
				else {
					text->mergeWith(L"\"");
				}
				if( root != NULL ){
					text->mergeWith(L" - #");
					FwCHAR *count = new FwCHAR(root->getCount());
					FwCHAR *idx = new FwCHAR(root->getIdx() + 1);
					text->mergeWith(idx);
					text->mergeWith(GUI_OF);
					text->mergeWith(count);

					delete count;
					delete idx;
				}
			}
	if( this->textmessage != NULL ){
		text->mergeWith(L" - ");
		text->mergeWith(this->textmessage);
	}
	SetWindowText(this->core->getWindowHandle(),text->toWCHAR());

	delete text;
}

void Interface::openFile()
{
	OPENFILENAME ofn;
	bool result = false;

	int szSize = 2 * MAX_PATH;
	WCHAR *szFile = new WCHAR[szSize];
	szFile[0] = '\0';

	FwCHAR *folder = new FwCHAR();
	if( this->core->getExplorer() != NULL )
		if( this->core->getExplorer()->getRoot() != NULL )
			folder->getFolderFrom(
				this->core->getExplorer()->getRoot()->getFilePath() );

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = this->core->getWindowHandle();
	ofn.lpstrFile = szFile;
	ofn.lpstrInitialDir = folder->toWCHAR();

	ofn.nMaxFile = szSize;
	ofn.lpstrFilter = OPENFILEFILTER;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	
	ofn.Flags =
		OFN_PATHMUSTEXIST | 
		OFN_FILEMUSTEXIST |
		OFN_NONETWORKBUTTON;

	this->blockMBB();
	if( GetOpenFileName(&ofn) ){
		result = this->core->open(szFile);
	}
	this->timerMBB();

	delete [] szFile;
	delete folder;
}
void Interface::openFolder()
{
	BROWSEINFO bnfo;
	bool result = false;

	int szSize = MAX_PATH;
	WCHAR *szFile = new WCHAR[szSize];
	szFile[0] = '\0';

	ZeroMemory(&bnfo, sizeof(bnfo));
	bnfo.hwndOwner = this->core->getWindowHandle();
	bnfo.pszDisplayName = NULL;
	bnfo.pidlRoot = NULL;
	
	bnfo.lpfn = NULL;
	bnfo.iImage = NULL;

	bnfo.ulFlags = 
		BIF_RETURNONLYFSDIRS | 
		BIF_DONTGOBELOWDOMAIN | 
		BIF_NONEWFOLDERBUTTON;

	LPITEMIDLIST output = NULL;
	if( (output = SHBrowseForFolder(&bnfo)) != NULL ){
		if( SHGetPathFromIDList(output,szFile) == YES ){
			FwCHAR *folder = new FwCHAR(szFile);
			result = this->core->open(folder);
		}
		CoTaskMemFree(output);
	}
	delete [] szFile;
}
void Interface::openDropFile(WPARAM wParam)
{
	bool result = false;
	int szSize = 2 * MAX_PATH;
	WCHAR *szFile = new WCHAR[szSize];
	szFile[0] = '\0';

	if( DragQueryFile((HDROP)wParam,0,szFile,szSize) ){
		result = this->core->open(szFile);
		DragFinish((HDROP)wParam);
	}
	delete [] szFile;
}
void Interface::close()
{
	this->core->close();
}

void Interface::showMenu()
{
	if( this->fullscreen == true && this->menuvisible == false ){
		SetMenu(this->core->getWindowHandle(),this->windowMenu);
		this->menuvisible = true;
		this->showCursor();

		if( this->core->getDrawer() != NULL )
			this->core->getDrawer()->setMenuheight(this->menuheight);
	}
}
void Interface::hideMenu()
{
	if( this->fullscreen == true && this->menuvisible == true ){
		SetMenu(this->core->getWindowHandle(),NULL);
		this->menuvisible = false;
		if( this->core->getDrawer() != NULL )
			this->core->getDrawer()->setMenuheight();
	}
}

bool Interface::isFullscreen()
{
	return this->fullscreen;
}

void Interface::setMessage(WCHAR *message)
{
	this->textmessage = message;
	this->updateText();
	this->textmessage = NULL;
}

void Interface::processMenu(int id)
{
	Drawer *drawer = this->core->getDrawer();
	Layer *layer = NULL;
	if( drawer != NULL )
		layer = drawer->getTopmost();

	switch(id){
		case ID_FILE_OPEN:
			this->openFile();
			break;
		case ID_FILE_OPENFOLDER:
			this->openFolder();
			break;
		case ID_FILE_CLOSE:
			this->close();
			break;
		case ID_FILE_EXTRACT:
			this->core->extract();
			break;
		case ID_FILE_SETASWALLPAPER:
			this->core->setwall();
			break;
		case ID_FILE_EXIT:
			DestroyWindow(this->core->getWindowHandle());
			break;
		case ID_VIEW_FULLSCREEN:
			this->setFullscreen(AUTO);
			this->update();
			break;
		case ID_VIEW_THUMBNAILS:
			if( drawer != NULL ){
				drawer->showThumbs();
				this->updateMenu();
			}
			break;
		case ID_VIEW_LIST:
			if( drawer != NULL ){
				drawer->showList();
				this->updateMenu();
			}
			break;
		case ID_FITTO_SCREENIFOVERSIZED:
			if( layer != NULL ){
				layer->setFitmode(FITSCREENOV);
				this->update();
			}
			break;
		case ID_FITTO_SCREEN:
			if( layer != NULL ){
				layer->setFitmode(FITSCREEN);
				this->update();
			}
			break;
		case ID_FITTO_WIDTH:
			if( layer != NULL ){
				layer->setFitmode(FITWIDTH);
				this->update();
			}
			break;
		case ID_FITTO_HEIGHT:
			if( layer != NULL ){
				layer->setFitmode(FITHEIGHT);
				this->update();
			}
			break;
		case ID_FITTO_NUMPAD:
			if( layer != NULL ){
				layer->setFitmode(FITNUMPAD);
				this->update();
			}
			break;
		case ID_FITTO_LEFT:
			if( layer != NULL ){
				layer->setSidemode(LEFT);
				this->updateMenu();
			}
			break;
		case ID_FITTO_RIGHT:
			if( layer != NULL ){
				layer->setSidemode(RIGHT);
				this->updateMenu();
			}
			break;
		case ID_ZOOM_100:
			if( layer != NULL ){
				layer->zoomat();
				this->updateText();
			}
			break;
		case ID_ZOOM_75:
			if( layer != NULL ){
				layer->zoomat(0.75);
				this->updateText();
			}
			break;
		case ID_ZOOM_50:
			if( layer != NULL ){
				layer->zoomat(0.5);
				this->updateText();
			}
			break;
		case ID_ZOOM_25:
			if( layer != NULL ){
				layer->zoomat(0.25);
				this->updateText();
			}
			break;
		case ID_ZOOM_200:
			if( layer != NULL ){
				layer->zoomat(2.0);
				this->updateText();
			}
			break;
		case ID_ZOOM_300:
			if( layer != NULL ){
				layer->zoomat(3.0);
				this->updateText();
			}
			break;
		case ID_ZOOM_500:
			if( layer != NULL ){
				layer->zoomat(5.0);
				this->updateText();
			}
			break;
		case ID_ZOOM_ZOOMIN:
			if( layer != NULL ){
				layer->zoomer(ZOOMSTEP);
				this->updateText();
			}
			break;
		case ID_ZOOM_ZOOMOUT:
			if( layer != NULL ){
				layer->zoomer(-ZOOMSTEP);
				this->updateText();
			}
			break;
		case ID_ROTATE_RESET:
			if( layer != NULL ){
				layer->rotateReset();
				this->updateText();
			}
			break;
		case ID_ROTATE_90:
			if( layer != NULL ){
				layer->rotate(1);
				this->updateText();
			}
			break;
		case ID_ROTATE_180:
			if( layer != NULL ){
				layer->rotate(2);
				this->updateText();
			}
			break;
		case ID_ROTATE_270:
			if( layer != NULL ){
				layer->rotate(3);
				this->updateText();
			}
			break;
		case ID_VIEW_FLOWSCROLL:
			if( layer != NULL ){
				layer->setSidedraw();
				this->updateMenu();
			}
			break;
		case ID_HELP_ABOUTFIEW:
			if( drawer != NULL ){
				drawer->showAbout();
				this->updateMenu();
			}
			break;
		case ID_HELP_MANUAL:
			if( drawer != NULL ){
				drawer->showManual();
				this->updateMenu();
			}
			break;
	}
}
void Interface::processKeys(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message){
		case WM_KEYDOWN:
			this->capKeyDown(wParam);
			break;
		case WM_KEYUP:
			this->capKeyUp(wParam);
			break;
	}
}
void Interface::processMouse(UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message){
		case WM_LBUTTONDOWN:
			this->capMouseDown(lParam,wParam,LEFT);
			break;
		case WM_LBUTTONUP:
			this->capMouseUp(lParam,wParam,LEFT);
			break;
		case WM_MBUTTONDOWN:
			this->capMouseDown(lParam,wParam,MID);
			break;
		case WM_MBUTTONUP:
			this->capMouseUp(lParam,wParam,MID);
			break;
		case WM_RBUTTONDOWN:
			this->capMouseDown(lParam,wParam,RIGHT);
			break;
		case WM_RBUTTONUP:
			this->capMouseUp(lParam,wParam,RIGHT);
			break;
		case WM_MOUSEWHEEL:
			this->capMouseWheel(lParam,wParam);
			break;
		case WM_MOUSEMOVE:
			this->capMouseMove(lParam,wParam);
			break;
	}
}

void Interface::capKeyDown(WPARAM wParam)
{
	Drawer *drawer = this->core->getDrawer();
	Layer *layer = NULL;
	if( drawer != NULL )
		layer = drawer->getTopmost();

	switch(wParam){
		case VK_UP:
			if( layer != NULL )
				layer->scrollVer(SCROLLSTEP);
			break;
		case VK_DOWN:
			if( layer != NULL )
				layer->scrollVer(-SCROLLSTEP);
			break;
		case VK_LEFT:
			if( layer != NULL )
				layer->scrollHor(SCROLLSTEP);
			break;
		case VK_RIGHT:
			if( layer != NULL )
				layer->scrollHor(-SCROLLSTEP);
			break;
		case VK_ADD:
			this->processMenu(ID_ZOOM_ZOOMIN);
			break;
		case VK_SUBTRACT:
			this->processMenu(ID_ZOOM_ZOOMOUT);
			break;
		case VK_SPACE:
		case VK_RETURN:
			if( layer != NULL ){
				layer->nextImage();
				layer->setCancel(false);
			}
			break;
		case VK_INSERT:
		case VK_NUMPAD0:
			if( layer != NULL )
				layer->prevImage();
			break;
		case VK_PRIOR:
		case VK_NUMPAD9:
			if( layer != NULL )
				layer->scrollSet(-layer->getMaxrollHor(),layer->getMaxrollVer());
			break;
		case VK_NUMPAD8:
			if( layer != NULL )
				layer->scrollSet(0,layer->getMaxrollVer());
			break;
		case VK_HOME:
		case VK_NUMPAD7:
			if( layer != NULL )
				layer->scrollSet(layer->getMaxrollHor(),layer->getMaxrollVer());
			break;
		case VK_NUMPAD6:
			if( layer != NULL )
				layer->scrollSet(-layer->getMaxrollHor(),0);
			break;
		case VK_NUMPAD5:
			if( layer != NULL )
				layer->scrollSet(0,0);
			break;
		case VK_NUMPAD4:
			if( layer != NULL )
				layer->scrollSet(layer->getMaxrollHor(),0);
			break;
		case VK_NEXT:
		case VK_NUMPAD3:
			if( layer != NULL )
				layer->scrollSet(-layer->getMaxrollHor(),-layer->getMaxrollVer());
			break;
		case VK_NUMPAD2:
			if( layer != NULL )
				layer->scrollSet(0,-layer->getMaxrollVer());
			break;
		case VK_END:
		case VK_NUMPAD1:
			if( layer != NULL )
				layer->scrollSet(layer->getMaxrollHor(),-layer->getMaxrollVer());
			break;
		case VK_ESCAPE:
			if( this->fullscreen == true ){
				this->setFullscreen(NO);
				this->updateMenu();
			}
			else if( drawer != NULL ){
				if( layer != NULL )
					layer->setCancel();

				drawer->hideOverlay();
				this->updateMenu();
			}
			break;
		case VK_MULTIPLY:
			this->processMenu(ID_VIEW_FULLSCREEN);
			break;
		case VK_DELETE:
		case VK_DECIMAL:
		case VK_OEM_COMMA:
		case VK_OEM_PERIOD:
			this->processMenu(ID_FITTO_SCREENIFOVERSIZED);
			break;
		case VK_SHIFT:
				this->setFullpath();
			break;
		case VK_DIVIDE:
			if( layer != NULL ){
				layer->rotate(1);
				this->updateText();
			}
			break;
		case VK_BACK:
			if( layer != NULL ){
				layer->repos();
				this->update();
			}
			break;
		case VK_O:
			if( layer != NULL )
				layer->setGifDir();
			break;
		case VK_P:
			if( layer != NULL )
				layer->setGifDir(NULL);
			break;
		case VK_OEM_4:
			this->processMenu(ID_FITTO_LEFT);
			break;
		case VK_OEM_6:
			this->processMenu(ID_FITTO_RIGHT);
			break;
	}
}
void Interface::capKeyUp(WPARAM wParam)
{
	Drawer *drawer = this->core->getDrawer();
	Layer *layer = NULL;
	if( drawer != NULL )
		layer = drawer->getTopmost();

	switch(wParam){
		case VK_ADD:
		case VK_SUBTRACT:
			if( layer != NULL )
				layer->zoomend();
			break;
	}
}

void Interface::capMouseDown(LPARAM lParam, WPARAM wParam, int button)
{
	if( this->mWH == true )
		return;

	if( this->mState == HTCLIENT && this->mBB == false ){
		this->mButton = button;
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if( button == MID ){
			this->setCursor(CURSOR_SCROLL);
			this->mX = x;
			this->mY = y;

			if( this->mMM == false ){
				//this->setCapture(true);
				this->mMM = true;
			}
			else {
				this->mMM = false;
				this->dX = this->dY = 0;
				this->setCursor(CURSOR_CLIENT);
				KillTimer(this->core->getWindowHandle(),TIMER_MMM);
			}
		}
	}
}
void Interface::capMouseMove(LPARAM lParam, WPARAM wParam)
{
	Drawer *drawer = this->core->getDrawer();
	Layer *layer = NULL;
	Overlay *overlay = NULL;
	if( drawer != NULL ){
		layer = drawer->getTopmost();
		overlay = drawer->getOverlay();
	}

	int x = LOWORD(lParam);
	int y = HIWORD(lParam);

	this->showCursor();

	if( overlay != NULL ){
		RECT lay = overlay->getOverlayRect();
		if( x > lay.left && x < lay.right &&
			y > lay.top && y < lay.bottom )
			this->setCursor(CURSOR_HAND);
		else
			this->setCursor(CURSOR_CLIENT);
	}
	if( this->fullscreen == true ){
		if( y < this->menuheight )
			this->showMenu();
		else
			this->hideMenu();
	}
	if( this->mButton == MID ){
		if( layer != NULL )
			layer->scroll(x - this->mX,y - this->mY);

		this->mX = x;
		this->mY = y;

		this->mMM = false;
	}
	if( this->mMM == true ){
		this->dX = (int)((this->mX - x)/SCROLL_DIV);
		this->dY = (int)((this->mY - y)/SCROLL_DIV);
	}
	if( this->fullscreen == true && this->menuvisible == false )
		SetTimer(this->core->getWindowHandle(),TIMER_MCH,CURSOR_HIDE,NULL);
}
void Interface::capMouseUp(LPARAM lParam, WPARAM wParam, int button)
{
	Drawer *drawer = this->core->getDrawer();
	Layer *layer = NULL;
	if( drawer != NULL )
		layer = drawer->getTopmost();

	if( this->mState == HTCLIENT && this->mBB == false && this->mWH == false ){
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);

		if( button == LEFT )
			if( layer != NULL )
				layer->prevImage(x,y);
		if( button == RIGHT )
			if( layer != NULL )
				layer->nextImage(x,y);
	}
	if( button == MID ){
		this->mBB = false;

		if( this->mMM == true ){
			//this->setCapture(false);
			SetTimer(this->core->getWindowHandle(),TIMER_MMM,SCROLL_TOUT,NULL);
		}
		else {
			this->setCursor(CURSOR_CLIENT);
		}
	}
	if( this->mButton == button )
		this->mButton = 0;
	if( this->mWH == true ){
		if( button == RIGHT )
			if( layer != NULL )
				layer->zoomend();
		this->mWH = false;
	}
}
void Interface::capMouseWheel(LPARAM lParam, WPARAM wParam)
{
	Drawer *drawer = this->core->getDrawer();
	Layer *layer = NULL;
	if( drawer != NULL )
		layer = drawer->getTopmost();

	int dir = GET_WHEEL_DELTA_WPARAM(wParam);
	int key = GET_KEYSTATE_WPARAM(wParam);

	if( dir > 0 ){
		if( key == MK_RBUTTON ){
			this->mWH = true;
			this->processMenu(ID_ZOOM_ZOOMIN);
		}
		else if( key == MK_LBUTTON ){
			this->mWH = true;
			if( layer != NULL )
				layer->scrollHor(SCROLLSTEP);
		}
		else
			if( layer != NULL )
				layer->scrollVer(SCROLLSTEP);
	}
	else {
		if( key == MK_RBUTTON ){
			this->mWH = true;
			this->processMenu(ID_ZOOM_ZOOMOUT);
		}
		else if( key == MK_LBUTTON ){
			this->mWH = true;
			if( layer != NULL )
				layer->scrollHor(-SCROLLSTEP);
		}
		else
			if( layer != NULL )
				layer->scrollVer(-SCROLLSTEP);
	}
}
LRESULT Interface::processMouseState(LPARAM lParam, LRESULT lResult)
{
	this->mState = lResult;
	return lResult;
}

void Interface::setCapture(bool state)
{
	if( state == true )
		SetCapture(this->core->getWindowHandle());
	else
		ReleaseCapture();
}

void Interface::showCursor()
{
	KillTimer(this->core->getWindowHandle(),TIMER_MCH);
	this->setCursor(this->cursor);
}
HCURSOR Interface::setCursor(LPCWSTR name)
{
	HCURSOR lastCursor = NULL;

	if( name == NULL )
		lastCursor = this->setCursor(lastCursor);
	else
		lastCursor = this->setCursor(LoadCursor(NULL,name));

	return lastCursor;
}
HCURSOR Interface::setCursor(HCURSOR cursor)
{
	if( cursor != NULL )
		this->cursor = cursor;

	return SetCursor(cursor);
}

void Interface::setFullpath()
{
	if( this->fullpath == true )
		this->fullpath = false;
	else
		this->fullpath = true;
	this->updateText();
}
void Interface::setFullscreen(int mode){
	HWND window = this->core->getWindowHandle();

	if( this->fullscreen == false && (mode == AUTO || mode == YES) ){

		this->windowInfo.cbSize = sizeof(WINDOWINFO);
		GetWindowInfo(window,&this->windowInfo);
		this->windowLong = GetWindowLong(window,GWL_STYLE);

		if( SetWindowLong(window,GWL_STYLE,WS_POPUP | WS_VISIBLE) == false ){
			this->core->messageBox_Error(MESSAGE_CANNOTFULLSCR);
			return;
		}
		if( SetWindowPos(window,HWND_NOTOPMOST,0,0,
							GetDeviceCaps(GetDC(NULL),HORZRES),
							GetDeviceCaps(GetDC(NULL),VERTRES),
							SWP_FRAMECHANGED) == false ){
			this->core->messageBox_Error(MESSAGE_CANNOTFULLSCR);
			return;
		}
		RECT client;
		GetClientRect(this->core->getWindowHandle(),&client);

		this->menuheight = GetDeviceCaps(GetDC(NULL),VERTRES) - client.bottom;
		this->fullscreen = true;
		this->hideMenu();
	}
	else if( this->fullscreen == true && (mode == AUTO || mode == NO) ){
		int x, y, w, h;
		x = this->windowInfo.rcWindow.left;
		y = this->windowInfo.rcWindow.top;
		w = this->windowInfo.rcWindow.right - this->windowInfo.rcWindow.left;
		h = this->windowInfo.rcWindow.bottom - this->windowInfo.rcWindow.top;
		
		this->showMenu();
		this->menuheight = 0;
		this->fullscreen = false;		

		SetWindowLong(window,GWL_STYLE,this->windowLong);
		SetWindowPos(window,HWND_NOTOPMOST,x,y,w,h,SWP_FRAMECHANGED);

		this->core->getDrawer()->setMenuheight();
	}
	this->update();
}

void Interface::blockMBB()
{
	this->mBB = true;
}
void Interface::timerMBB()
{
	SetTimer(this->core->getWindowHandle(),TIMER_MBB,MOUSE_BLOCK,NULL);
}
void Interface::unblockMBB()
{
	this->mBB = false;
}

void Interface::movemMM()
{
	Drawer *drawer = this->core->getDrawer();
	Layer *layer = NULL;
	if( drawer != NULL )
		layer = this->core->getDrawer()->getTopmost();
	if( layer != NULL )
		layer->scroll(this->dX,this->dY);
}
