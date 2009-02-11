class Core;

using namespace Gdiplus;

class Drawer
{
private:
	Core *core;
	List<Layer> *layers;
	Layer *scene, *overlay;
	Thumblay *thumblay;
	Listlay *listlay;

	bool fulldraw, visAbout, visManual, visThumbs, visList;

public:
	Drawer(Core *core);
	~Drawer();

	void paint(HDC hdc);
	void reset();

	Layer *getTopmost();
	Layer *getScene();
	Overlay *getOverlay();
	Thumblay *getThumblay();
	Listlay *getListlay();
	RECT getClientSize();

	void updateOverlay();

	void hideOverlay(bool invalid = true);
	void showAbout();
	void showManual();
	void showThumbs();
	void showList();

	bool isAbout();
	bool isManual();
	bool isThumbs();
	bool isList();

	Image *loadImageResource(WORD id, LPCWSTR type);

	void setMenuheight(int val = 0);
	void invalidate(bool full = true);
};