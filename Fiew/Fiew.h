#pragma once

#include "resource.h"

/* essentials */
#define APP_TITLE		L"Fiew"
#define MAX_LOADSTRING	100
#define FERROR			-1
#define DEFAULT			666
#define ENGLISH_LOCALE	"English_US.1252"

#define HEADSORT		-1
#define TAILSORT		1

#define LEFT			1
#define MID				2
#define RIGHT			3

#define TOP				-1
#define BOT				1

#define NO				0
#define YES				1
#define AUTO			2
#define NOTHUMBS		3

#define FITSCREEN		10
#define FITSCREENOV		15
#define FITNUMPAD		20
#define FITWIDTH		30
#define FITHEIGHT		40
#define FITLEFT			50
#define FITRIGHT		60

#define ROT_0			0
#define ROT_90			1
#define ROT_180			2
#define ROT_270			3

#define VK_O			0x4F
#define VK_P			0x50

#define TIMER_MBB		333
#define TIMER_MCH		325
#define TIMER_MMM		111
#define TIMER_OVL		847
#define TIMER_THB		783

#define TIMER_INT		777

#define TICKER_OFF		-1
#define TICKER_STEPS	4
#define TICKER_SIZE		5
#define TICKER_INDENT	6

/* confs */
#define DEBUG
//#define DISKCACHING

#define FONT				L"Arial"
#define FONTSIZE			11
#define DIRMERGE			L"\\*"

#define INIT_TOUT			10

#define RC_DLL				L"DLL"
#define RC_PNG				L"PNG"
#define RARDLL				L"unrar.dll"

#define CACHE_SIZE			20
#define CACHE_LIMIT			10

#define MARGIN				20
#define FRAME				2

#define SCROLLSTEP			50
#define ZOOMSTEP			0.05
#define ZOOMINIT			1.0

#define MINDELAY			50

#define CURSOR_CLIENT		IDC_ARROW
#define CURSOR_SCROLL		IDC_SIZEALL
#define CURSOR_WAIT			IDC_WAIT
#define CURSOR_ARRWAIT		IDC_APPSTARTING
#define CURSOR_HAND			IDC_HAND

#define CURSOR_HIDE			2000
#define MOUSE_BLOCK			200
#define SCROLL_TOUT			50
#define OVL_HIDEOUT			100

#define SCROLL_DIV			3.0

#define OVL_SIZE			650
#define OVL_MARGIN			30

#define THB_SIZE			165
#define THB_SMSIZE			110
#define THB_SPACE			10
#define THB_ROW				5
#define THB_COUNT			12
#define THB_TOUT			500

#define LST_X				5
#define LST_Y				30
#define LST_MAXLEN			96
#define LST_MOD				5

#define CLR_WHITE			Color(255,255,255,255)
#define CLR_WHITE_ALPHA		Color(175,255,255,255)
#define CLR_FRAME_LIGHT		Color(255,212,208,200)
#define CLR_FRAME_DARK		Color(255,112,108,100)
#define CLR_LITE			Color(200,212,208,200)
#define CLR_DIRK			Color(200,112,108,100)

/* file recognition */

/* mime */
#define MIME_NONE		0
#define MIME_IMAGE		1
#define MIME_ARCHIVE	2
#define MIME_DIR		3

/* type */
#define TYPE_NONE			0
#define TYPE_TIFFINTEL		1
#define TYPE_TIFFMOTOROLA	2
#define TYPE_GIF87a			3
#define TYPE_GIF89a			4
#define TYPE_PNG			5
#define TYPE_JPEGJFIF		6
#define TYPE_JPEGEXIF		7
#define TYPE_JPEGAPP2		8
#define TYPE_JPEGAPP3		9
#define TYPE_JPEGAPP4		10
#define TYPE_JPEGAPP5		11
#define TYPE_JPEGAPP6		12
#define TYPE_JPEGAPP7		13
#define TYPE_JPEGAPP8		14
#define TYPE_JPEGAPP9		15
#define TYPE_JPEGAPPA		16
#define TYPE_JPEGAPPB		17
#define TYPE_JPEGAPPC		18
#define TYPE_JPEGAPPD		19
#define TYPE_JPEGAPPE		20
#define TYPE_JPEGAPPF		21
#define TYPE_BITMAP			22

#define TYPE_ZIP			23
#define TYPE_RAR			24

#define TYPE_DIR			25

/* locales */
#define LANG_EN 44
#ifndef LANG_EN
#define LANG_PL 48
#endif

// messages
#define GUI_ARCHSPLIT		L"~"
#define GUI_OF L"/"
#define GUI_SEP L"\n\n"

#ifdef LANG_EN
#define MENU_FILE L"&File"
#define MENU_FILE_OPEN L"&Open...\tCtrl+O"
#define MENU_FILE_OPENFOLDER L"Open &Folder...\tCtrl+Shift+O"
#define MENU_FILE_EXTRACT L"&Extract\tCtrl+E"
#define MENU_FILE_SETWALL L"Set as &Wallpaper\tCtrl+W"
#define MENU_FILE_CLOSE L"&Close\tCtrl+C"
#define MENU_FILE_EXIT L"E&xit\tCtrl+Q"

#define MENU_VIEW L"&View"
#define MENU_VIEW_FULLSCREEN L"Full&screen\t*"
#define MENU_VIEW_THUMBNAILS L"Thum&bnails\tCtrl+M"
#define MENU_VIEW_LIST L"&List\tCtrl+L"
#define MENU_VIEW_FITTO L"Fi&t to"
#define MENU_VIEW_ZOOM L"&Zoom"
#define MENU_VIEW_ROTATE L"&Rotate"

#define MENU_VIEW_FITTO_SCREENOV L"Sc&reen if Oversized\t."
#define MENU_VIEW_FITTO_SCREEN L"Sc&reen"
#define MENU_VIEW_FITTO_HEIGHT L"&Height\tCtrl+F"
#define MENU_VIEW_FITTO_WIDTH L"&Width\tCtrl+Shift+F"
#define MENU_VIEW_FITTO_NUMPAD L"&Numpad\tAlt+F"
#define MENU_VIEW_FITTO_LEFT L"&Left\t["
#define MENU_VIEW_FITTO_RIGHT L"&Right\t]"

#define MENU_VIEW_ZOOM_ZOOMIN L"Zoom &in\t+"
#define MENU_VIEW_ZOOM_ZOOMOUT L"Zoom &out\t-"

#define MENU_VIEW_ROTATE_RESET L"&Reset"

#define MENU_VIEW_FLOWSCROLL L"&Flow Scroll\tCtrl+S"

#define MENU_HELP L"&Help"
#define MENU_HELP_ABOUT L"&About...\tCtrl+?"
#define MENU_HELP_MANUAL L"&Manual...\tCtrl+I"

#define MESSAGE_ERROR L"Error"
#define MESSAGE_WARNING L"Warning"
#define MESSAGE_INFO L"Notice"
#define MESSAGE_CLOSE L"Close"

#define MESSAGE_PROCESS L"Processing... Please wait."
#define MESSAGE_EXTRACT L"Extracting... Please wait."

#define MESSAGE_FAKEIMG L"This file is not a proper image and cannot be read."
#define MESSAGE_NORAR L"Unable to load unrar.dll! Reading RAR Archives disabled."
#define MESSAGE_NORARFUNC L"Unable to load unrar.dll functions! Reading RAR Archives disabled."
#define MESSAGE_CANNOTOPEN L"The file cannot be opened."
#define MESSAGE_CANNOTFULLSCR L"Could not change to fullscreen display."
#define MESSAGE_CLOSETEXT L"Do you want to close the image?"

#define MESSAGE_EXTRACTEDIMAGE L"Image successfully extracted."
#define MESSAGE_EXTRACTERROR L"The extraction was unsuccessful."

#define MESSAGE_SETWALLEDIMAGE L"Image successfully set as wallpaper."
#define MESSAGE_SETWALLERROR L"Placing the wallpaper was unsuccessful."

#define MESSAGE_RAR_NO_MEMORY L"Not enough memory to initialize data structures."
#define MESSAGE_RAR_BAD_DATA L"Archive header broken."
#define MESSAGE_RAR_BAD_ARCHIVE L"File is not valid RAR archive."
#define MESSAGE_RAR_UNKNOWN_FORMAT L"Unknown encryption used for archive headers."
#define MESSAGE_RAR_EOPEN L"File open error."
#define MESSAGE_RAR_BAD_PASS L"Incorrect archive password."

#define MESSAGE_EMPTY L"No image files in this collection."

#define MESSAGE_ZIP_CONST L"Unknown zip result code."
#define MESSAGE_ZIP_ZR_OK L"Success."
#define MESSAGE_ZIP_ZR_FAIL L"File is not valid ZIP archive."
#define MESSAGE_ZIP_ZR_NODUPH L"Couldn't duplicate handle."
#define MESSAGE_ZIP_ZR_NOFILE L"Couldn't create/open file."
#define MESSAGE_ZIP_ZR_NOALLOC L"Failed to allocate memory."
#define MESSAGE_ZIP_ZR_WRITE L"Error writing to file."
#define MESSAGE_ZIP_ZR_NOTFOUND L"File not found in the zipfile."
#define MESSAGE_ZIP_ZR_MORE L"Still more data to unzip."
#define MESSAGE_ZIP_ZR_CORRUPT L"Zipfile is corrupt or not a zipfile."
#define MESSAGE_ZIP_ZR_READ L"Error reading file."
#define MESSAGE_ZIP_ZR_ARGS L"Caller: faulty arguments."
#define MESSAGE_ZIP_ZR_PARTIALUNZ L"Caller: the file had already been partially unzipped."
#define MESSAGE_ZIP_ZR_NOTMMAP L"Caller: can only get memory of a memory zipfile."
#define MESSAGE_ZIP_ZR_MEMSIZE L"Caller: not enough space allocated for memory zipfile."
#define MESSAGE_ZIP_ZR_FAILED L"Caller: there was a previous error."
#define MESSAGE_ZIP_ZR_ENDED L"Caller: additions to the zip have already been ended."
#define MESSAGE_ZIP_ZR_ZMODE L"Caller: mixing creation and opening of zip."
#define MESSAGE_ZIP_ZR_NOTINITED L"Zip-bug: internal initialisation not completed."
#define MESSAGE_ZIP_ZR_SEEK L"Zip-bug: trying to seek the unseekable."
#define MESSAGE_ZIP_ZR_MISSIZE L"Zip-bug: the anticipated size turned out wrong."
#define MESSAGE_ZIP_ZR_NOCHANGE L"Zip-bug: tried to change mind, but not allowed."
#define MESSAGE_ZIP_ZR_FLATE L"Zip-bug: an internal error during flation."

#define DIALOG_PASSWORD L"Archive password"

#define OPENFILEFILTER L"All Readable\0*.JPG;*.JPEG;*.PNG;*.GIF;*.TIF;*.TIFF;*.BMP;*.RAR;*.CBR;*.ZIP;*.CBZ\0Jpeg\0*.JPG;*.JPEG\0Png\0*.PNG\0Gif\0*.GIF\0Tif\0*.TIF;*.TIFF\0Bmp\0*.BMP\0Rar\0*.RAR\0Cbr\0*.CBR\0Zip\0*.ZIP\0Cbz\0*.CBZ\0\0";

#endif
#ifdef LANG_PL
#define MENU_FILE L"&Plik"
#define MENU_FILE_OPEN L"&Otwórz...\tCtrl+O"
#define MENU_FILE_OPENFOLDER L"Otwórz &Folder...\tCtrl+Shift+O"
#define MENU_FILE_EXTRACT L"&Rozpakuj\tCtrl+E"
#define MENU_FILE_SETWALL L"Ustaw jako &Tapetê\tCtrl+W"
#define MENU_FILE_CLOSE L"&Zamknij\tCtrl+C"
#define MENU_FILE_EXIT L"&WyjdŸ\tCtrl+Q"

#define MENU_VIEW L"&Widok"
#define MENU_VIEW_FULLSCREEN L"Pe³ny &Ekran\t*"
#define MENU_VIEW_THUMBNAILS L"&Miniaturki\tCtrl+M"
#define MENU_VIEW_LIST L"&Lista\tCtrl+L"
#define MENU_VIEW_FITTO L"&Dopasuj do"
#define MENU_VIEW_ZOOM L"&Skaluj"
#define MENU_VIEW_ROTATE L"&Obróæ"

#define MENU_VIEW_FITTO_SCREENOV L"&Ekranu gdy Wiêkszy\t."
#define MENU_VIEW_FITTO_SCREEN L"&Ekranu"
#define MENU_VIEW_FITTO_HEIGHT L"&Wysokoœci\tCtrl+F"
#define MENU_VIEW_FITTO_WIDTH L"&Szerokoœci\tCtrl+Shift+F"
#define MENU_VIEW_FITTO_NUMPAD L"&Numpada\tAlt+F"
#define MENU_VIEW_FITTO_LEFT L"&Lewej\t["
#define MENU_VIEW_FITTO_RIGHT L"&Prawej\t]"

#define MENU_VIEW_ZOOM_ZOOMIN L"Po&wiêksz\t+"
#define MENU_VIEW_ZOOM_ZOOMOUT L"Po&mniejsz\t-"

#define MENU_VIEW_ROTATE_RESET L"&Resetuj"

#define MENU_VIEW_FLOWSCROLL L"&P³ynne Przewijanie\tCtrl+S"

#define MENU_HELP L"&Pomoc"
#define MENU_HELP_ABOUT L"&O Programie...\tCtrl+?"
#define MENU_HELP_MANUAL L"&Interfejs...\tCtrl+I"

#define MESSAGE_ERROR L"B³¹d"
#define MESSAGE_WARNING L"Ostrze¿enie"
#define MESSAGE_INFO L"Informacja"
#define MESSAGE_CLOSE L"Zamknij"

#define MESSAGE_PROCESS L"Przetwarzanie... Proszê czekaæ."
#define MESSAGE_EXTRACT L"Rozpakowywanie... Proszê czekaæ."

#define MESSAGE_FAKEIMG L"Plik nie jest w³aœciwym obrazem i nie mo¿e byæ odczytany."
#define MESSAGE_NORAR L"Za³adowanie biblioteki unrar.dll nie powiod³o siê! Odczyt archiwów RAR wy³¹czony."
#define MESSAGE_NORARFUNC L"Za³adowanie funkcji biblioteki unrar.dll nie powiod³o siê! Odczyt archiwów RAR wy³¹czony."
#define MESSAGE_CANNOTOPEN L"Plik nie mo¿e zostaæ otwarty."
#define MESSAGE_CANNOTFULLSCR L"Zmiana na tryb pe³noekranowy nie powiod³a siê."
#define MESSAGE_CLOSETEXT L"Czy chcesz zamkn¹c obraz?"

#define MESSAGE_EXTRACTEDIMAGE L"Obraz zosta³ rozpakowany."
#define MESSAGE_EXTRACTERROR L"Rozpakowanie nie powiod³o siê."

#define MESSAGE_SETWALLEDIMAGE L"Obraz umieszczony na pulpicie."
#define MESSAGE_SETWALLERROR L"Umieszczenie na pulpicie nie powiod³o siê."

#define MESSAGE_RAR_NO_MEMORY L"Za ma³o pamiêci by zainicjalizowaæ struktury."
#define MESSAGE_RAR_BAD_DATA L"Uszkodzony nag³ówek archiwum."
#define MESSAGE_RAR_BAD_ARCHIVE L"Plik nie jest prawid³owym archiwem RAR."
#define MESSAGE_RAR_UNKNOWN_FORMAT L"Nieznane kodowanie nag³ówka archiwum."
#define MESSAGE_RAR_EOPEN L"B³¹d odczytu pliku."
#define MESSAGE_RAR_BAD_PASS L"B³êdne has³o do archiwum."

#define MESSAGE_EMPTY L"Brak obrazów w tym zbiorze."

#define MESSAGE_ZIP_CONST L"Nieznany kod rezultatu."
#define MESSAGE_ZIP_ZR_OK L"Sukces."
#define MESSAGE_ZIP_ZR_FAIL L"Plik nie jest prawid³owym archiwem ZIP."
#define MESSAGE_ZIP_ZR_NODUPH L"B³ad duplikacji handlera."
#define MESSAGE_ZIP_ZR_NOFILE L"B³ad otwarcia/utworzenia pliku."
#define MESSAGE_ZIP_ZR_NOALLOC L"B³ad alokacji pamiêci."
#define MESSAGE_ZIP_ZR_WRITE L"B³ad zapisu pliku."
#define MESSAGE_ZIP_ZR_NOTFOUND L"Plik nie zosta³ znaleziony w archiwum."
#define MESSAGE_ZIP_ZR_MORE L"Wiêcej danych do rozpakowania."
#define MESSAGE_ZIP_ZR_CORRUPT L"Archiwum jest uszkodzone lub nie jest archiwem ZIP."
#define MESSAGE_ZIP_ZR_READ L"B³¹d odczytu pliku."
#define MESSAGE_ZIP_ZR_ARGS L"Caller: b³êdne argumenty."
#define MESSAGE_ZIP_ZR_PARTIALUNZ L"Caller: plik ju¿ zosta³ czêœciowo rozpakowany."
#define MESSAGE_ZIP_ZR_NOTMMAP L"Caller: mo¿na pobraæ pamiêæ tylko archiwum w pamiêci."
#define MESSAGE_ZIP_ZR_MEMSIZE L"Caller: za ma³o pamiêci zaalokowanej dla archiwum w pamiêci."
#define MESSAGE_ZIP_ZR_FAILED L"Caller: wczeœniej wyst¹pi³ b³¹d."
#define MESSAGE_ZIP_ZR_ENDED L"Caller: dodawanie do archiwum ju¿ zosta³o wstrzymane."
#define MESSAGE_ZIP_ZR_ZMODE L"Caller: mieszanie tworzenia i dodawania do archiwum."
#define MESSAGE_ZIP_ZR_NOTINITED L"Zip-bug: wewnêtrzna inicjalizacja przerwana."
#define MESSAGE_ZIP_ZR_SEEK L"Zip-bug: próba poszukiwania nieistniej¹cego."
#define MESSAGE_ZIP_ZR_MISSIZE L"Zip-bug: zak³adana wielkoœæ okaza³a siê b³êdna."
#define MESSAGE_ZIP_ZR_NOCHANGE L"Zip-bug: chcia³em zmieniæ w³asne zdanie, ale mi nie pozwolili."
#define MESSAGE_ZIP_ZR_FLATE L"Zip-bug: wewnêtrzny b³¹d podczas operacji 'flation'."

#define DIALOG_PASSWORD L"Podaj has³o do archiwum"

#define OPENFILEFILTER L"Wszystkie\0*.JPG;*.JPEG;*.PNG;*.GIF;*.TIF;*.TIFF;*.BMP;*.RAR;*.CBR;*.ZIP;*.CBZ\0Jpeg\0*.JPG;*.JPEG\0Png\0*.PNG\0Gif\0*.GIF\0Tif\0*.TIF;*.TIFF\0Bmp\0*.BMP\0Rar\0*.RAR\0Cbr\0*.CBR\0Zip\0*.ZIP\0Cbz\0*.CBZ\0\0";
#endif
