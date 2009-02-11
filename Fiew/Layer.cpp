#include "stdafx.h"
#include "Core.h"

Layer::Layer(Core *core, Image *image)
{
	this->core = core;
	this->cell = NULL;

	this->image = image;
	this->next = NULL;
	this->prev = NULL;

	this->scali = NULL;
	this->scaln = NULL;
	this->scalp = NULL;

	this->fulldraw = true;

	this->sidedraw = false;
	this->zoomdraw = false;
	this->passmess = false;
	this->fullscreen = false;
	this->cancel = false;

	this->dimension = NULL;
	this->frameCount = 0;
	this->frameThat = 0;

	this->Brush_Back = new SolidBrush(CLR_WHITE);
	this->Brush_DarkBack = new SolidBrush(CLR_DIRK);
	this->Brush_LiteBack = new SolidBrush(CLR_LITE);

	this->Pen_Border = new Pen(CLR_FRAME_LIGHT,1);
	this->Pen_DarkBorder = new Pen(CLR_FRAME_DARK,1);

	this->FontSize = FONTSIZE;

	this->FontFamily_Arial = new FontFamily(FONT,NULL);

	this->Font_Default = 
		new Font(this->FontFamily_Arial,(REAL)this->FontSize,FontStyleRegular,UnitPixel);

	this->mut_image = NULL;
	this->mut_animloop = NULL;
	this->mut_terminator = NULL;

	this->thrd_anim = NULL;

	this->offset = NULL;
	this->menuheight = NULL;

	this->fitmode = FITSCREENOV;
	this->sidemode = NULL;
	this->type = TYPE_NONE;

	this->zoom = ZOOMINIT;
	this->rot = ROT_0;
	this->gifdir = RIGHT;

	this->init();
	this->locate(TOP);
}

Layer::~Layer()
{
	this->reset();

	delete this->Brush_Back;
	delete this->Brush_DarkBack;
	delete this->Brush_LiteBack;

	delete this->Pen_Border;
	delete this->Pen_DarkBorder;

	delete this->FontFamily_Arial;

	delete this->Font_Default;
}

void Layer::init()
{
	if( this->image == NULL )
		this->loadContent(DEFAULT);
}

void Layer::setPassmess()
{
	this->passmess = true;
}

void Layer::reset()
{
	if( WaitForSingleObject(this->mut_animloop,0) == WAIT_TIMEOUT ){
		ReleaseMutex(this->mut_terminator);
		WaitForSingleObject(this->mut_animloop,INFINITE);
		ReleaseMutex(this->mut_animloop);
	}
	if( this->mut_image != NULL )
		CloseHandle(this->mut_image);
	this->mut_image = NULL;

	if( this->mut_animloop != NULL )
		CloseHandle(this->mut_animloop);
	this->mut_animloop = NULL;

	if( this->mut_terminator != NULL )
		CloseHandle(this->mut_terminator);
	this->mut_terminator = NULL;

	if( this->thrd_anim != NULL )
		CloseHandle(this->thrd_anim);
	this->thrd_anim = NULL;

	if( this->image != NULL )
		delete this->image;
	this->image = NULL;
	if( this->next != NULL )
		delete this->next;
	this->next = NULL;
	if( this->prev != NULL )
		delete this->prev;
	this->prev = NULL;

	if( this->scali != NULL )
		delete this->scali;
	this->scali = NULL;
	if( this->scaln != NULL )
		delete this->scaln;
	this->scaln = NULL;
	if( this->scalp != NULL )
		delete this->scalp;
	this->scalp = NULL;

	if( this->dimension != NULL )
		delete this->dimension;
	this->dimension = NULL;
	this->frameCount = 0;
	this->frameThat = 0;

	this->gifdir = RIGHT;
}
void Layer::repos()
{
	this->sidemode = NULL;
	this->rotateReset(true);
	this->setFitmode(NULL);
}

void Layer::loadContent(int init)
{
	this->reset();

	Cacher *cacher = this->core->getCacher();
	if( cacher != NULL ){
		Cell *tmp = NULL;
		if( this->sidedraw == true ){
			if( cacher->prev() ){
				tmp = cacher->getThat();
				if( tmp != NULL )
					this->prev = tmp->getImage();
				cacher->next();
			}
		}
		this->cell = cacher->getThat();
		if( this->cell != NULL ){
			WaitForSingleObject(this->mut_image,INFINITE);
			this->image = this->cell->getImage();
			ReleaseMutex(this->mut_image);
		}
		if( this->sidedraw == true ){
			if( cacher->next() ){
				tmp = cacher->getThat();
				if( tmp != NULL )
					this->next = tmp->getImage();
				cacher->prev();
			}
		}
	}
	this->afterLoadContent(init);
}

void Layer::afterLoadContent(int init)
{
	int mode = init;
	if( this->sidedraw == false )
		mode = TOP;

	this->rotateSet();
	this->locate(mode);
	this->setFitmode();

	if( this->core->getGui() != NULL && init != DEFAULT )
		this->core->getGui()->update();
}

void Layer::nextContent(int init)
{
	Cacher *cacher = this->core->getCacher();
	if( cacher != NULL ){
		HCURSOR last = NULL;
		if( this->core->getGui() != NULL )
			last = this->core->getGui()->setCursor(CURSOR_ARRWAIT);

		if( cacher->next() )
			this->loadContent(init);

		if( this->core->getGui() != NULL )
			this->core->getGui()->setCursor(last);		
	}
}
void Layer::prevContent(int init)
{
	Cacher *cacher = this->core->getCacher();
	if( cacher != NULL ){
		HCURSOR last = NULL;
		if( this->core->getGui() != NULL )
			last = this->core->getGui()->setCursor(CURSOR_ARRWAIT);

		if( cacher->prev() )
			this->loadContent(init);

		if( this->core->getGui() != NULL )
			this->core->getGui()->setCursor(last);
	}
}

void Layer::nextImage(int x, int y)
{
	if( this->nextFrame(false) == true ){
		this->offset = 0;
		this->nextContent(TOP);
	}
	else
		this->zoomend();
}
void Layer::prevImage(int x, int y)
{
	if( this->prevFrame(false) == true ){
		this->offset = 0;
		this->prevContent(TOP);
	}
	else
		this->zoomend();
}

bool Layer::nextFrame(bool back)
{
	if( this->frameCount < 2 )
		return true;
	if( back == false &&
		WaitForSingleObject(this->mut_animloop,0) == WAIT_TIMEOUT )
		return true;

	bool result = false;

	this->frameThat++;
	if( this->frameThat >= this->frameCount )
		this->frameThat = 0;
	if( this->frameThat == 0 )
		result = true;

	this->image->SelectActiveFrame(this->dimension,this->frameThat);

	return result;
}
bool Layer::prevFrame(bool back)
{
	if( this->frameCount < 2 )
		return true;
	if( back == false &&
		WaitForSingleObject(this->mut_animloop,0) == WAIT_TIMEOUT )
		return true;

	bool result = false;

	if( this->frameThat == 0 )
		result = true;
	if( this->frameThat == 0 )
		this->frameThat = this->frameCount;

	this->frameThat--;
	this->image->SelectActiveFrame(this->dimension,this->frameThat);

	return result;
}

Bitmap *Layer::render()
{
	this->locate();

	WaitForSingleObject(this->mut_image,INFINITE);

	int frame = FRAME;
	Bitmap *scene = NULL;
	if( this->fulldraw == true )
		scene = new Bitmap(this->cwidth + 2*frame,this->cheight + 2*frame);
	else
		scene = new Bitmap(this->width,this->height);

	Graphics *tmpgfx = Graphics::FromImage(scene);

	Image *tmp = NULL;
	tmpgfx->Clear(CLR_WHITE);

	if( this->fulldraw == true ){
		if( this->image != NULL ){
			double zoom = NULL;
			int width, height;

			if( this->scali == NULL )
				tmp = this->image;
			else
				tmp = this->scali;

			if( this->fullscreen == false )
				tmpgfx->DrawRectangle(this->Pen_Border,
									  this->x - frame,
									  this->y - frame,
									  this->width + frame + 1,
									  this->height + frame + 1);

			tmpgfx->DrawImage(tmp,
							  Rect(this->x,
								   this->y,
								   this->width,
								   this->height),
							  0,
							  0,
							  tmp->GetWidth(),
							  tmp->GetHeight(),
							  UnitPixel);

			if( this->prev != NULL && this->sidedraw == true ){
				if( this->scalp == NULL )
					tmp = this->prev;
				else 
					tmp = this->scalp;
				width = tmp->GetWidth();
				height = tmp->GetHeight();
				if( this->scalp == NULL )
					zoom = this->getZoom(width,height,this->cwidth,this->cheight);
				else 
					zoom = ZOOMINIT;

				int pwidth = (int)(zoom * width);
				int pheight = (int)(zoom * height);
				int px = (int)((this->cwidth - pwidth)/2);
				if( this->sidemode == LEFT && px < 0 )
					px = 0;
				else if( this->sidemode == RIGHT && px < 0 )
					px = this->cwidth - pwidth;

				int py = this->y - pheight - MARGIN - this->offrollVer;

				if( this->fullscreen == false )
					tmpgfx->DrawRectangle(this->Pen_Border,
										  px - frame,
										  py - frame,
										  pwidth + frame + 1,
										  pheight + frame + 1);

				tmpgfx->DrawImage(tmp,Rect(px,py,pwidth,pheight),0,0,
								  tmp->GetWidth(),tmp->GetHeight(),UnitPixel);
			}
			if( this->next != NULL && this->sidedraw == true ){
				if( this->scaln == NULL )
					tmp = this->next;
				else
					tmp = this->scaln;
				width = tmp->GetWidth();
				height = tmp->GetHeight();
				if( this->scaln == NULL )
					zoom = this->getZoom(width,height,this->cwidth,this->cheight);
				else
					zoom = ZOOMINIT;

				int nwidth = (int)(zoom * width);
				int nheight = (int)(zoom * height);
				int nx = (int)((this->cwidth - nwidth)/2);
				if( this->sidemode == LEFT && nx < 0 )
					nx = 0;
				else if( this->sidemode == RIGHT && nx < 0 )
					nx = this->cwidth - nwidth;

				int ny = this->y + this->height + MARGIN + this->offrollVer;

				if( this->fullscreen == false )
					tmpgfx->DrawRectangle(this->Pen_Border,
										  nx - frame,
										  ny - frame,
										  nwidth + frame + 1,
										  nheight + frame + 1);

				tmpgfx->DrawImage(tmp,Rect(nx,ny,nwidth,nheight),0,0,
								  tmp->GetWidth(),tmp->GetHeight(),UnitPixel);
			}
		}
		this->x = 0;
		this->y = 0;
		this->width = scene->GetWidth();
		this->height = scene->GetHeight();
	}
	else {
		if( this->scali == NULL )
			tmp = this->image;
		else 
			tmp = this->scali;

		tmpgfx->DrawImage(tmp,
						  Rect(0,0,this->width,this->height),
						  0,0,tmp->GetWidth(),tmp->GetHeight(),
						  UnitPixel);
	}
	ReleaseMutex(this->mut_image);

	this->fulldraw = true;
	this->animate();

	delete tmpgfx;

	return scene;
}

void Layer::scroll(int hor, int ver)
{
	int invalid = 2;
	if( (this->rollHor == this->maxrollHor && hor > 0) ||
		(this->rollHor == -this->maxrollHor && hor < 0 ) ||	hor == 0 )
		invalid--;
	if( (((this->rollVer == this->maxrollVer && ver > 0) ||
		(this->rollVer == this->minrollVer && ver < 0)) && 
		 this->sidedraw == false) || ver == 0 )
		invalid--;

	if( invalid > 0 ){
		this->rollHor += hor;
		this->rollVer += ver;
		this->boundRoll();
		this->invalidate();
	}
}
void Layer::scrollSet(int x, int y)
{
	this->rollHor = x;
	this->rollVer = y;
	this->boundRoll();
	this->invalidate();
}
void Layer::scrollHor(int val)
{
	if( (this->rollHor == this->maxrollHor && val > 0) ||
		(this->rollHor == -this->maxrollHor && val < 0 ) )
		return;

	this->rollHor += val;
	this->boundRoll();
	this->invalidate();
}
void Layer::scrollVer(int val)
{
	if( ((this->rollVer == this->maxrollVer && val > 0) ||
		(this->rollVer == this->minrollVer && val < 0)) && this->sidedraw == false )
		return;

	this->rollVer += val;
	this->boundRoll();
	this->invalidate();
}

void Layer::zoomer(double val)
{
	if( this->zoom == ZOOMSTEP && val < 0 )
		return;
	if( val == NULL )
		return;

	this->zoombegin();
	this->unsetFitmode();

	this->zoom += val;
	this->boundZoom();
	this->invalidate();
}
void Layer::zoomat(double val)
{
	this->zoombegin();
	this->unsetFitmode();

	this->zoom = val;
	this->boundZoom();
	this->invalidate();
}
void Layer::zoombegin()
{
	this->zoomdraw = true;
}
void Layer::zoomend()
{
	this->zoomdraw = false;

	if( this->scali != NULL )
		delete this->scali;
	if( this->scaln != NULL )
		delete this->scaln;
	if( this->scalp != NULL )
		delete this->scalp;
	this->scali = NULL;
	this->scaln = NULL;
	this->scalp = NULL;
	
	if( this->zoom < ZOOMINIT ){
		this->scali = this->scale(this->image);
		if( this->next != NULL )
			this->scaln = this->scale(this->next);
		if( this->prev != NULL )
			this->scalp = this->scale(this->prev);
	}
	this->invalidate();
}

void Layer::rotate(Image *image)
{
	if( image == NULL )
		return;

	switch(this->rot){
		case 1:
			image->RotateFlip(Rotate90FlipNone);
			break;
		case 2:
			image->RotateFlip(Rotate180FlipNone);
			break;
		case 3:
			image->RotateFlip(Rotate270FlipNone);
			break;
	}
}
void Layer::rotate(int val)
{
	if( val == NULL )
		return;

	bool rotati = true;
	if( WaitForSingleObject(this->mut_animloop,0) == WAIT_TIMEOUT )
		rotati = false;

	this->rot = (this->rot + val) % 4;
	switch(val){
		case 1:
			if( rotati == true )
				this->image->RotateFlip(Rotate90FlipNone);
			if( this->next != NULL )
				this->next->RotateFlip(Rotate90FlipNone);
			if( this->prev != NULL )
				this->prev->RotateFlip(Rotate90FlipNone);
			break;
		case 2:
			if( rotati == true )
				this->image->RotateFlip(Rotate180FlipNone);
			if( this->next != NULL )
				this->next->RotateFlip(Rotate180FlipNone);
			if( this->prev != NULL )
				this->prev->RotateFlip(Rotate180FlipNone);
			break;
		case 3:
			if( rotati == true )
				this->image->RotateFlip(Rotate270FlipNone);
			if( this->next != NULL )
				this->next->RotateFlip(Rotate270FlipNone);
			if( this->prev != NULL )
				this->prev->RotateFlip(Rotate270FlipNone);
			break;
	}
	this->setFitmode();
}
void Layer::rotateSet()
{
	bool rotati = true;
	if( WaitForSingleObject(this->mut_animloop,0) == WAIT_TIMEOUT )
		rotati = false;

	switch(this->rot){
		case 1:
			if( rotati == true )
				this->image->RotateFlip(Rotate90FlipNone);
			if( this->next != NULL )
				this->next->RotateFlip(Rotate90FlipNone);
			if( this->prev != NULL )
				this->prev->RotateFlip(Rotate90FlipNone);
			break;
		case 2:
			if( rotati == true )
				this->image->RotateFlip(Rotate180FlipNone);
			if( this->next != NULL )
				this->next->RotateFlip(Rotate180FlipNone);
			if( this->prev != NULL )
				this->prev->RotateFlip(Rotate180FlipNone);
			break;
		case 3:
			if( rotati == true )
				this->image->RotateFlip(Rotate270FlipNone);
			if( this->next != NULL )
				this->next->RotateFlip(Rotate270FlipNone);
			if( this->prev != NULL )
				this->prev->RotateFlip(Rotate270FlipNone);
			break;
	}
}
void Layer::rotateReset(bool novalid)
{
	bool rotati = true;
	if( WaitForSingleObject(this->mut_animloop,0) == WAIT_TIMEOUT )
		rotati = false;

	switch(this->rot){
		case 1:
			if( rotati == true )
				image->RotateFlip(Rotate270FlipNone);
			if( this->next != NULL )
				this->next->RotateFlip(Rotate270FlipNone);
			if( this->prev != NULL )
				this->prev->RotateFlip(Rotate270FlipNone);
			break;
		case 2:
			if( rotati == true )
				image->RotateFlip(Rotate180FlipNone);
			if( this->next != NULL )
				this->next->RotateFlip(Rotate180FlipNone);
			if( this->prev != NULL )
				this->prev->RotateFlip(Rotate180FlipNone);
			break;
		case 3:
			if( rotati == true )
				this->image->RotateFlip(Rotate90FlipNone);
			if( this->next != NULL )
				this->next->RotateFlip(Rotate90FlipNone);
			if( this->prev != NULL )
				this->prev->RotateFlip(Rotate90FlipNone);
			break;
	}
	this->rot = 0;
	if( novalid == false )
		this->zoomend();
}

int Layer::getX()
{
	return this->x;
}
int Layer::getY()
{
	return this->y;
}
int Layer::getWidth()
{
	return this->width;
}
int Layer::getHeight()
{
	return this->height;
}
int Layer::getMaxrollHor()
{
	if( this->image == NULL )
		return 0;

	RECT client;
	GetClientRect(this->core->getWindowHandle(),&client);

	WaitForSingleObject(this->mut_image,INFINITE);
	int width = (int)(this->zoom * this->image->GetWidth());
	ReleaseMutex(this->mut_image);

	return abs( min((int)((client.right - width)/2),0) );
}
int Layer::getMaxrollVer()
{
	if( this->image == NULL )
		return 0;

	RECT client;
	GetClientRect(this->core->getWindowHandle(),&client);

	WaitForSingleObject(this->mut_image,INFINITE);
	int height = (int)(this->zoom * this->image->GetHeight());
	ReleaseMutex(this->mut_image);

	return abs( min((int)((client.bottom - height)/2),0) );
}

File *Layer::getFile()
{
	if( this->cell != NULL )
		return this->cell->getFile();
	return NULL;
}

int Layer::getImageWidth()
{
	WaitForSingleObject(this->mut_image,INFINITE);
	int width = this->image->GetWidth();
	ReleaseMutex(this->mut_image);
	return width;
}
int Layer::getImageHeight()
{
	WaitForSingleObject(this->mut_image,INFINITE);
	int height = this->image->GetHeight();
	ReleaseMutex(this->mut_image);
	return height;
}
double Layer::getZoom()
{
	return this->zoom;
}
double Layer::getZoom(int width, int height)
{
	if( this->fitmode == NULL )
		return this->zoom;

	RECT client = this->getClientSize();
	return this->getZoom(width,height,client.right,client.bottom);
}
double Layer::getZoom(int width, int height, int cwidth, int cheight)
{
	if( this->fitmode == NULL )
		return this->zoom;

	width += 2 * FRAME;
	height += 2 * FRAME;

	double dx, dy;
	if( this->fitmode == FITSCREEN || this->fitmode == FITSCREENOV ||
		this->fitmode == FITWIDTH || this->fitmode == FITHEIGHT ){
		dx = (double)cwidth / (double)width;
		dy = (double)cheight / (double)height;
	}
	else if( this->fitmode == FITNUMPAD ){
		dx = (double)(3 * cwidth) / (double)width;
		dy = (double)(3 * cheight) / (double)height;
		dx += ZOOMINIT;
		dy += ZOOMINIT;
	}
	if( this->fitmode == FITSCREENOV )
		return min(ZOOMINIT,min(dx,dy));
	if( this->fitmode == FITSCREEN || this->fitmode == FITNUMPAD )
		return min(dx,dy);

	if( this->fitmode == FITWIDTH )
		return dy;
	if( this->fitmode == FITHEIGHT )
		return dx;

	return this->zoom;
}
bool Layer::getSidedraw()
{
	return this->sidedraw;
}
int Layer::getFitmode()
{
	return this->fitmode;
}

void Layer::setFitmode(int mode)
{
	//double newzoom = ZOOMINIT;

	if( this->fitmode == mode || mode == NULL ){
		this->fitmode = NULL;
		this->zoom = ZOOMINIT;
		this->zoomend();
		return;
	}
	else if( mode != DEFAULT ){
		this->fitmode = mode;
		this->sidedraw = false;
	}

	WaitForSingleObject(this->mut_image,INFINITE);
	if( this->image != NULL )
		this->zoom = this->getZoom(this->image->GetWidth(),this->image->GetHeight());
	ReleaseMutex(this->mut_image);

	this->zoomend();
}
void Layer::unsetFitmode()
{
	if( this->fitmode != NULL ){
		this->fitmode = NULL;
		if( this->core->getGui() != NULL )
			this->core->getGui()->updateMenu();
	}
}

void Layer::setSidemode(int mode)
{
	if( this->sidemode == mode )
		this->sidemode = NULL;
	else
		this->sidemode = mode;

	if( this->sidedraw == false && this->getMaxrollHor() == 0 )
		return;

	if( this->sidemode == LEFT )
		this->rollHor = this->getMaxrollHor();
	else if( this->sidemode == RIGHT )
		this->rollHor = -this->getMaxrollHor();

	this->invalidate();
}
int Layer::getSidemode()
{
	return this->sidemode;
}

void Layer::setSidedraw()
{
	if( this->sidedraw == false ){
		this->sidedraw = true;
		if( this->fitmode != NULL )
			this->setFitmode(NULL);
	}
	else
		this->sidedraw = false;

	this->loadContent(NULL);
}
void Layer::setMenuheight(int val)
{
	this->menuheight = val;
}
void Layer::setGifDir(int dir)
{
	WaitForSingleObject(this->mut_image,INFINITE);
	if( dir == DEFAULT ){
		if( this->gifdir == RIGHT )
			this->gifdir = LEFT;
		else
			this->gifdir = RIGHT;
	}
	else {
		if( this->gifdir != RIGHT && this->gifdir != LEFT )
			this->gifdir = RIGHT;
		else
			this->gifdir = dir;
	}
	ReleaseMutex(this->mut_image);
}
void Layer::setCancel(bool set)
{
	this->cancel = set;
}

bool Layer::setWall()
{
	bool result = false;

	if( this->image != NULL && this->cell != NULL ){
		CLSID bmpsid;
		Core::getEncoder(L"image/bmp", &bmpsid);
		FwCHAR *path = NULL;

		if( this->cell->getFile()->isArchived() == true ){
			if( this->core->getExplorer() != NULL ){
				path = new FwCHAR();
				path->getFolderFrom( this->core->getExplorer()->getArchivePath() );
				path->mergeWith( this->cell->getFile()->getFileName() );
			}
			else
				path = new FwCHAR( this->cell->getFile()->getFileName()->toWCHAR() );
		}
		else {
			path = new FwCHAR( this->cell->getFile()->getFilePath()->toWCHAR() );
		}
		if( this->cell->getFile()->getType() != TYPE_BITMAP )
			path->mergeWith(L".bmp");

		this->image->Save(path->toWCHAR(),&bmpsid,NULL);

		result = SystemParametersInfo(SPI_SETDESKWALLPAPER,0,path->toWCHAR(),
									  SPIF_UPDATEINIFILE | SPIF_SENDWININICHANGE );
		delete path;
	}
	return result;
}

bool Layer::isCancel()
{
	return this->cancel;
}
bool Layer::isContent()
{
	if( this->image != NULL )
		return true;
	return false;
}
bool Layer::isPassMess()
{
	bool result = this->passmess;
	this->passmess = false;
	return result;
}

void Layer::locate(int init)
{
	RECT client = this->getClientSize();
	this->cwidth = client.right;
	this->cheight = client.bottom;

	if( this->image == NULL ){
		this->width = this->cwidth;
		this->height = this->cheight;
	}
	else {
		WaitForSingleObject(this->mut_image,INFINITE);
		this->zoom = this->getZoom(this->image->GetWidth(),this->image->GetHeight(),
								   this->cwidth,this->cheight);

		this->width = (int)(this->zoom * this->image->GetWidth());
		this->height = (int)(this->zoom * this->image->GetHeight());
		ReleaseMutex(this->mut_image);

		if( WaitForSingleObject(this->mut_animloop,0) == WAIT_TIMEOUT ){
			if( this->rot == ROT_90 || this->rot == ROT_270 ){
				int tmp = this->width;
				this->width = this->height;
				this->height = tmp;
			}
		}
		else
			ReleaseMutex(this->mut_animloop);
	}
	int hodiff = this->cwidth - this->width;
	int vediff = this->cheight - this->height;

	this->x = (int)(hodiff / 2);
	this->y = (int)(vediff / 2);

	this->maxrollHor = abs( min(this->x,0) );
	this->maxrollVer = abs( min(this->y,0) );

	this->minrollVer = -this->maxrollVer;
	this->offrollVer = 0;//max(this->y,0);

	if( init != NULL ){
		this->rollHor = 0;
		if( this->sidemode == LEFT )
			this->rollHor = this->maxrollHor;
		else if( this->sidemode == RIGHT )
			this->rollHor = -this->maxrollHor;

		if( init == TOP )
			this->rollVer = this->maxrollVer + this->offset;
		else if( init == BOT )
			this->rollVer = this->minrollVer + this->offset;
		this->offset = 0;
	}
	if( this->image != NULL && this->sidedraw == true ){
		if( this->prev != NULL )
			if( this->y > 0 )
				this->maxrollVer = this->height;
			else
				this->maxrollVer = -this->y + this->cheight + MARGIN;

		if( this->next != NULL )
			if( this->y > 0 )
				this->minrollVer = -this->height;
			else
				this->minrollVer = this->y - this->cheight - MARGIN;
	}
	this->boundRoll();

	this->x += this->rollHor;
	this->y += this->rollVer - this->menuheight;
}

void Layer::boundRoll()
{
	this->rollHor = min(this->rollHor,this->maxrollHor);
	this->rollHor = max(this->rollHor,-this->maxrollHor);

	if( this->rollVer > this->maxrollVer ){
		this->offset = this->rollVer - this->maxrollVer;
		this->rollVer = min(this->rollVer,this->maxrollVer);
		if( this->image != NULL && this->sidedraw == true )
			this->prevContent();
	}
	if( this->rollVer < this->minrollVer ){
		this->offset = this->rollVer - this->minrollVer;
		this->rollVer = max(this->rollVer,this->minrollVer);
		if( this->image != NULL && this->sidedraw == true )
			this->nextContent();
	}
}

void Layer::boundZoom()
{
	this->zoom = max(this->zoom,ZOOMSTEP);
}

RECT Layer::getClientSize()
{
	RECT client;
	client.top = 0;
	client.left = 0;
	client.right = 0;
	client.bottom = 0;

	this->fullscreen = false;
	if( this->core->getGui() != NULL )
		this->fullscreen = this->core->getGui()->isFullscreen();

	if( this->fullscreen == true ){
		client.right = GetDeviceCaps(GetDC(NULL),HORZRES);
		client.bottom = GetDeviceCaps(GetDC(NULL),VERTRES);
	}
	else 
		GetClientRect(this->core->getWindowHandle(),&client);

	return client;
}

void Layer::invalidate(bool full)
{
	this->fulldraw = full;
	this->core->getDrawer()->invalidate(full);
}

Image *Layer::scale(Image *source)
{
	if( source == NULL )
		return NULL;

	double zoom = this->zoom;
	if( source != this->image )
		zoom = this->getZoom(source->GetWidth(),source->GetHeight());

	int width = (int)(zoom * source->GetWidth());
	int height = (int)(zoom * source->GetHeight());

	Bitmap *bmp = new Bitmap(width,height);
	Graphics *gfx = Graphics::FromImage(bmp);
	gfx->DrawImage(source,0,0,width,height);
	
	delete gfx;
	return bmp;
}

void Layer::animate()
{
	if( this->isTopmost() == false )
		return;
	if( WaitForSingleObject(this->mut_animloop,0) == WAIT_TIMEOUT )
		return;
	else
		ReleaseMutex(this->mut_animloop);

	if( this->cell == NULL )
		return;
	if( this->cell->getFile() == NULL )
		return;
	this->type = this->cell->getFile()->getType();
	if( this->type == TYPE_TIFFINTEL || this->type == TYPE_TIFFMOTOROLA ||
		this->type == TYPE_GIF87a || this->type == TYPE_GIF89a ){

		UINT dCount = this->image->GetFrameDimensionsCount();
		GUID *dList = new GUID[dCount];
		this->image->GetFrameDimensionsList(dList,dCount);
		this->dimension = &dList[0];
		this->frameCount = this->image->GetFrameCount(dimension);

		for( UINT i = 1; i < dCount; i++ )
			delete &dList[i];
		if( this->frameCount < 2 ){
			if( this->dimension != NULL )
				delete this->dimension;
			this->dimension = NULL;
			return;
		}
	}
	else
		return;
	if( this->type != TYPE_GIF87a && this->type != TYPE_GIF89a )
		return;	

	this->mut_image = CreateMutex(NULL,false,NULL);
	this->mut_animloop = CreateMutex(NULL,false,NULL);
	this->mut_terminator = CreateMutex(NULL,true,NULL);

	this->thrd_anim = CreateThread(NULL,NULL,
								  (LPTHREAD_START_ROUTINE)Layer::anim,
								   this,NULL,NULL);
}

DWORD WINAPI Layer::anim(LPVOID param)
{
	Layer *that = (Layer *)param;

	WaitForSingleObject(that->mut_animloop,INFINITE);

	WaitForSingleObject(that->mut_image,INFINITE);
	if( that->image == NULL ){
		ReleaseMutex(that->mut_image);
		return 0;
	}
	int propertySize = that->image->GetPropertyItemSize(PropertyTagFrameDelay);
	PropertyItem *frameDelay = (PropertyItem*) malloc(propertySize);
	that->image->GetPropertyItem(PropertyTagFrameDelay,propertySize,frameDelay);

	ReleaseMutex(that->mut_image);

	long *delays = (long *)frameDelay->value;
	bool mode = false;

	while( true ){
		if( that->isTopmost() == false )
			break;
		if( WaitForSingleObject(that->mut_terminator,0) != WAIT_TIMEOUT ){
			ReleaseMutex(that->mut_terminator);
			ReleaseMutex(that->mut_animloop);
			return 0;
		}
		Sleep( max((delays[that->frameThat] * 100),MINDELAY) );

		WaitForSingleObject(that->mut_image,INFINITE);

		if( that->gifdir == RIGHT )
			mode = that->nextFrame();
		else if( that->gifdir == LEFT )
			mode = that->prevFrame();

		if( that->scali != NULL )
			delete that->scali;
		that->scali = NULL;

		if( that->rot != ROT_0 ){
			that->scali = new Bitmap(that->image->GetWidth(),that->image->GetHeight());
			Graphics *gfx = Graphics::FromImage(that->scali);
			gfx->DrawImage(that->image,0,0,that->scali->GetWidth(),that->scali->GetHeight());
			delete gfx;
		}
		ReleaseMutex(that->mut_image);

		if( that->rot != ROT_0 )
			that->rotate(that->scali);
		that->invalidate(mode);
	}
	if( frameDelay != NULL )
		delete frameDelay;	

	ReleaseMutex(that->mut_animloop);
	return 0;
}

bool Layer::isTopmost()
{
	if( this->core->getDrawer()->getTopmost() == this )
		return true;
	return false;
}