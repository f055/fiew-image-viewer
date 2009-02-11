#include "stdafx.h"
#include "Core.h"

Drawer::Drawer(Core *core)
{
	this->core = core;
	this->layers = NULL;
	this->fulldraw = true;

	this->scene = NULL;
	this->overlay = NULL;
	this->thumblay = NULL;
	this->listlay = NULL;

	this->visAbout = false;
	this->visManual = false;
	this->visThumbs = false;
	this->visList = false;
}

Drawer::~Drawer()
{
	delete this->layers;
}

void Drawer::paint(HDC hdc)
{
	Graphics *gfx = Graphics::FromHDC(hdc);

	int count = this->layers->getCount();
	Layer *lay = NULL;
	Bitmap *bmp = NULL;

	if( count > 1 && this->fulldraw == true ){
		RECT client = this->getClientSize();
		Bitmap *scene = new Bitmap(client.right,client.bottom);
		Graphics *scenegfx = Graphics::FromImage(scene);

		this->layers->gotoHead();
		do {
			lay = this->layers->getThat();
			if( lay != NULL )
				bmp = lay->render();
			if( bmp != NULL ){
				scenegfx->DrawImage(bmp,
									Rect(lay->getX(),lay->getY(),
										 lay->getWidth(),lay->getHeight()),
									0,0,bmp->GetWidth(),bmp->GetHeight(),
									UnitPixel);
				delete bmp;
			}
			bmp = NULL;
			lay = NULL;
		} while( this->layers->next() == true );

		gfx->DrawImage(scene,0,0,scene->GetWidth(),scene->GetHeight());

		delete scenegfx;
		delete scene;
	}
	else if( count == 1 || this->fulldraw == false ){
		lay = this->getTopmost();
		if( lay != NULL )
			bmp = lay->render();
		if( bmp != NULL ){
			gfx->DrawImage(bmp,
						   Rect(lay->getX(),lay->getY(),
								lay->getWidth(),lay->getHeight()),
						   0,0,bmp->GetWidth(),bmp->GetHeight(),
						   UnitPixel);
			delete bmp;
		}
		bmp = NULL;
	}
	else {
		gfx->Clear(CLR_FRAME_DARK);
	}
	delete gfx;

	this->fulldraw = true;
}

void Drawer::reset()
{
	this->hideOverlay(false);

	if( this->layers != NULL )
		delete this->layers;
	this->layers = new List<Layer>();
	
	this->scene = new Layer(this->core);
	this->overlay = NULL;
	this->thumblay = NULL;
	this->listlay = NULL;

	this->layers->add(this->scene);
}

Layer *Drawer::getTopmost()
{
	return this->layers->getTail();
}
Layer *Drawer::getScene()
{
	if( this->scene != NULL )
		if( this->scene->isContent() )
			return this->scene;
	return NULL;
}
Overlay *Drawer::getOverlay()
{
	if( this->overlay != NULL )
		if( this->overlay->isContent() )
			return (Overlay *)this->overlay;
	return NULL;
}
Thumblay *Drawer::getThumblay()
{
	if( this->visThumbs == true )
		return this->thumblay;
	return NULL;
}
Listlay *Drawer::getListlay()
{
	if( this->visList == true )
		return this->listlay;
	return NULL;
}

RECT Drawer::getClientSize()
{
	RECT client;
	client.left = client.right = 0;
	client.top = client.bottom = 0;

	if( this->getTopmost() != NULL )
		return this->getTopmost()->getClientSize();
	return client;
}

void Drawer::updateOverlay()
{
	if( this->visThumbs == true )
		this->getThumblay()->update();
	if( this->visList == true )
		this->getListlay()->update();
}

void Drawer::hideOverlay(bool invalid)
{
	if( this->overlay != NULL ){
		if( invalid == false )
			this->overlay->setCancel();
		this->layers->remove(this->overlay);

		if( this->overlay->isCancel() == false ){
			if( this->visThumbs == true || this->visList == true ){
				this->getScene()->loadContent();
				if( this->core->getCacher() != NULL )
					this->core->getCacher()->setFull(false);
				if( this->core->getGui() != NULL )
					this->core->getGui()->updateText();
			}
		}
		else {
			if( this->visThumbs == true ){
				if( this->core->getCacher() != NULL ){
					Cell *last = this->getThumblay()->getLastCell();
					this->core->getCacher()->gotoCell(last);
					this->core->getCacher()->setFull(false);
				}
			}
			if( this->visList == true ){
				if( this->core->getCacher() != NULL ){
					Cell *last = this->getListlay()->getLastCell();
					this->core->getCacher()->gotoCell(last);
					this->core->getCacher()->setFull(false);
				}
			}
		}
		delete this->overlay;
	}
	else
		invalid = false;

	this->overlay = NULL;
	this->thumblay = NULL;
	this->listlay = NULL;

	this->visAbout = false;
	this->visManual = false;
	this->visThumbs = false;
	this->visList = false;

	if( invalid == true ){
		if( this->core->getGui() != NULL ){
			this->core->getGui()->updateText();
			this->core->getGui()->setCursor(CURSOR_CLIENT);
		}
		this->invalidate();
	}
}

void Drawer::showAbout()
{
	if( this->visAbout == false ){
		this->hideOverlay(false);
		this->overlay =
			new Overlay(this->core,this->loadImageResource(IDR_ABOUT,RC_PNG));
		this->layers->add(this->overlay);
		this->visAbout = true;
	}
	else {
		this->hideOverlay(false);
		this->visAbout = false;
	}
	this->invalidate();
}
void Drawer::showManual()
{
	if( this->visManual == false ){
		this->hideOverlay(false);
		this->overlay =
			new Overlay(this->core,this->loadImageResource(IDR_MANUAL,RC_PNG));
		this->layers->add(this->overlay);
		this->visManual = true;
	}
	else {
		this->hideOverlay(false);
		this->visManual = false;
	}
	this->invalidate();
}
void Drawer::showThumbs()
{
	HCURSOR last = NULL;
	if( this->core->getGui() != NULL )
		last = this->core->getGui()->setCursor(CURSOR_WAIT);

	if( this->visThumbs == false ){
		this->hideOverlay(false);

		this->thumblay =
			new Thumblay(this->core,new Bitmap(1,1));
		this->overlay = this->thumblay;

		this->layers->add(this->overlay);
		this->visThumbs = true;
	}
	else {
		this->hideOverlay(false);
		this->visThumbs = false;
	}
	this->invalidate();

	if( this->core->getGui() != NULL )
		this->core->getGui()->setCursor(last);	
}
void Drawer::showList()
{
	HCURSOR last = NULL;
	if( this->core->getGui() != NULL )
		last = this->core->getGui()->setCursor(CURSOR_WAIT);

	if( this->visList == false ){
		this->hideOverlay(false);

		this->listlay =
			new Listlay(this->core,new Bitmap(1,1));
		this->overlay = this->listlay;

		this->layers->add(this->overlay);
		this->visList = true;
	}
	else {
		this->hideOverlay(false);
		this->visList = false;
	}
	this->invalidate();

	if( this->core->getGui() != NULL )
		this->core->getGui()->setCursor(last);	
}

bool Drawer::isAbout()
{
	return this->visAbout;
}
bool Drawer::isManual()
{
	return this->visManual;
}
bool Drawer::isThumbs()
{
	return this->visThumbs;
}
bool Drawer::isList()
{
	return this->visList;
}

Image *Drawer::loadImageResource(WORD id, LPCWSTR type)
{
	Image *img = NULL;

	HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(id), type);
	if( hResource != NULL ){
		HGLOBAL hLoader = LoadResource(NULL,hResource);
		if( hLoader != NULL ){
			byte *data = (byte *)LockResource(hLoader);
			DWORD len = SizeofResource(NULL,hResource);

			if( data != NULL && len > 0 ){
				IStream *stream = NULL;
				HGLOBAL buffer = NULL;

				if( (buffer = GlobalAlloc(GPTR,len)) != NULL )
					if( CreateStreamOnHGlobal(buffer,true,(LPSTREAM*)&stream) == S_OK )
						if( stream->Write((void *)data,len,NULL) == S_OK )
							img = Image::FromStream(stream);
				if( stream != NULL )
					stream->Release();
			}
			UnlockResource(hLoader);
		}
		FreeResource(hLoader);
	}
	return img;
}

void Drawer::setMenuheight(int val)
{
	this->layers->gotoHead();
	do {
		this->layers->getThat()->setMenuheight(val);
	} while( this->layers->next() );

	this->getTopmost()->invalidate();
}

void Drawer::invalidate(bool full)
{
	this->fulldraw = full;
	InvalidateRect(this->core->getWindowHandle(),NULL,false);
}