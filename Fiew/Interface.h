class Core;

class Interface
{
private:
	Core *core;

	LRESULT mState;
	UINT mButton;
	int mX, mY, dX, dY;
	bool mBB, mWH, mMM;

	bool fullscreen, menuvisible, fullpath;
	WCHAR *textmessage;
	int menuheight;

	WINDOWINFO windowInfo;
	HMENU windowMenu, menuFile, menuView, menuHelp;
	LONG windowLong;
	HCURSOR cursor;

public:
	Interface(Core *core);
	~Interface();

	void update();
	void updateMenu();
	void updateText();

	void openFile();
	void openFolder();
	void openDropFile(WPARAM wParam);
	void close();

	bool isFullscreen();

	void setMessage(WCHAR *message);

	void showCursor();
	HCURSOR setCursor(LPCWSTR name = NULL);
	HCURSOR setCursor(HCURSOR cursor);

	void blockMBB();
	void timerMBB();
	void unblockMBB();

	void movemMM();

	void processMenu(int id);
	void processKeys(UINT message, WPARAM wParam, LPARAM lParam);
	void processMouse(UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT processMouseState(LPARAM lParam, LRESULT lResult);

private:
	void initLocale();

	void showMenu();
	void hideMenu();

	void capKeyDown(WPARAM wParam);
	void capKeyUp(WPARAM wParam);

	void capMouseDown(LPARAM lParam, WPARAM wParam, int button);
	void capMouseUp(LPARAM lParam, WPARAM wParam, int button);
	void capMouseWheel(LPARAM lParam, WPARAM wParam);
	void capMouseMove(LPARAM lParam, WPARAM wParam);

	void setCapture(bool state);
	void setFullscreen(int mode);
	void setFullpath();
};