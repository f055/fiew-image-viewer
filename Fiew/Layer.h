using namespace Gdiplus;

class Core;
class Cell;

class Layer {
protected:
	Core *core;
	Cell *cell;
	Image *image, *next, *prev, *scali, *scaln, *scalp;
	int type, fitmode, sidemode;

	int x, y, width, height, cwidth, cheight;
	int rollHor, rollVer, maxrollHor, maxrollVer, minrollVer, offrollVer;
	int offset, menuheight, rot, gifdir;
	double zoom;
	bool fulldraw, sidedraw, zoomdraw, passmess, fullscreen, cancel;

	GUID *dimension;
	UINT frameCount, frameThat;

	SolidBrush *Brush_Back;
	SolidBrush *Brush_DarkBack;
	SolidBrush *Brush_LiteBack;

	Pen *Pen_Border;
	Pen *Pen_DarkBorder;

	int FontSize;

	FontFamily *FontFamily_Arial;
	Font *Font_Default;

	HANDLE mut_animloop, mut_image, mut_terminator;
	HANDLE thrd_anim;

public:
	Layer(Core *core, Image *image = NULL);
	virtual ~Layer();

	void loadContent(int init = TOP);

	virtual void nextImage(int x = FERROR, int y = FERROR);
	virtual void prevImage(int x = FERROR, int y = FERROR);

	virtual Bitmap *render();
	void reset();
	virtual void repos();

	virtual void scroll(int hor, int ver);
	virtual void scrollSet(int x, int y);
	virtual void scrollHor(int val);
	virtual void scrollVer(int val);

	virtual void zoomer(double val = NULL);
	virtual void zoomat(double val = ZOOMINIT);
	virtual void zoomend();
	
	virtual void rotate(int val = NULL);
	virtual void rotateReset(bool novalid = false);

	int getX();
	int getY();

	int getWidth();
	int getHeight();
	File *getFile();
	int getImageWidth();
	int getImageHeight();
	RECT getClientSize();

	int getMaxrollHor();
	int getMaxrollVer();

	double getZoom();
	bool getSidedraw();
	int getSidemode();
	int getFitmode();

	virtual void setFitmode(int mode = DEFAULT);
	virtual void unsetFitmode();

	virtual void setSidemode(int mode = NULL);

	virtual void setSidedraw();

	void setMenuheight(int val);
	void setGifDir(int dir = DEFAULT);
	void setCancel(bool set = true);

	bool setWall();

	bool isCancel();
	bool isContent();
	bool isPassMess();

	void invalidate(bool full = true);

	static DWORD WINAPI anim(LPVOID param);

protected:
	void init();
	void setPassmess();

	void afterLoadContent(int init);

	void nextContent(int init = TOP);
	void prevContent(int init = BOT);

	bool nextFrame(bool back = false);
	bool prevFrame(bool back = false);

	void rotate(Image *image);

	void zoombegin();
	void rotateSet();

	void locate(int init = NULL);
	void boundRoll();
	void boundZoom();

	double getZoom(int width, int height);
	double getZoom(int width, int height, int cwidth, int cheight);

	void animate();
	Image *scale(Image *source);
	
	bool isTopmost();
};

class Overlay : public Layer
{
public:
	Overlay(Core *core, Image *image);
	~Overlay();

	virtual void nextImage(int x = FERROR, int y = FERROR);
	virtual void prevImage(int x = FERROR, int y = FERROR);

	Bitmap *render();

	virtual void zoomer(double val = NULL);
	virtual void zoomat(double val = ZOOMINIT);
	virtual void zoomend();
	
	virtual void rotate(int val = NULL);
	virtual void rotateReset(bool novalid = false);

	virtual void setFitmode(int mode = DEFAULT);
	virtual void unsetFitmode();
	virtual void setSidedraw();

	virtual void setSidemode(int mode = NULL);

	RECT getOverlayRect();

protected:
	void hide();
};

class Thumblay : public Overlay
{
private:
	Cell *lastCell;
	int ticker;
	int picker;

public:
	Thumblay(Core *core, Image *image);
	~Thumblay();

	void update(bool init = false);

	void nextImage(int x = FERROR, int y = FERROR);
	void prevImage(int x = FERROR, int y = FERROR);

	void scroll(int hor, int ver);
	void scrollSet(int x, int y);
	void scrollHor(int val);
	void scrollVer(int val);

	void zoomer(double val = NULL);
	void zoomat(double val = ZOOMINIT);
	void zoomend();
	
	void rotate(int val = NULL);
	void rotateReset(bool novalid = false);

	void setFitmode(int mode = DEFAULT);
	void unsetFitmode();
	void setSidedraw();

	void setSidemode(int mode = NULL);

	Cell *getLastCell();

private:
	void subrender();

	void setPicker(int newpick);
	int getPicker(int x, int y);

};

class Listlay : public Overlay
{
private:
	Cell *lastCell;
	int ticker;

public:
	Listlay(Core *core, Image *image);
	~Listlay();

	void update(bool init = false);

	void nextImage(int x = FERROR, int y = FERROR);
	void prevImage(int x = FERROR, int y = FERROR);

	void scroll(int hor, int ver);
	void scrollSet(int x, int y);
	void scrollHor(int val);
	void scrollVer(int val);

	void zoomer(double val = NULL);
	void zoomat(double val = ZOOMINIT);
	void zoomend();
	
	void rotate(int val = NULL);
	void rotateReset(bool novalid = false);

	void setFitmode(int mode = DEFAULT);
	void unsetFitmode();
	void setSidedraw();

	void setSidemode(int mode = NULL);

	Cell *getLastCell();

private:
	void subrender();
};