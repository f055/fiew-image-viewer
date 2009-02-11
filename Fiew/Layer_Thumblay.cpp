#include "stdafx.h"
#include "Core.h"

Thumblay::Thumblay(Core *core, Image *image) : Overlay(core,image)
{
	this->lastCell = NULL;
	this->ticker = TICKER_OFF;
	this->picker = NULL;

	if( this->image != NULL )
		delete this->image;

	if( this->core->getCacher() != NULL ){
		this->lastCell = this->core->getCacher()->getThat();
		this->core->getCacher()->setFull(true);
	}
	
	this->image = new Bitmap(OVL_SIZE,OVL_SIZE);
	this->update(true);
}

Thumblay::~Thumblay()
{
}

void Thumblay::update(bool init)
{
	this->subrender();
	this->invalidate(init);

	if( this->core->getCacher() != NULL ){
		if( this->core->getCacher()->isRunning() == true ){
			this->ticker++;
			this->ticker = this->ticker % TICKER_STEPS;
			SetTimer(this->core->getWindowHandle(),TIMER_THB,THB_TOUT,NULL);
			return;
		}
	}
	if( this->ticker > TICKER_OFF ){
		this->ticker = TICKER_OFF;
		SetTimer(this->core->getWindowHandle(),TIMER_THB,THB_TOUT,NULL);
	}
}

void Thumblay::nextImage(int x, int y)
{
	this->setCancel();
	this->hide();
}
void Thumblay::prevImage(int x, int y)
{
	if( x != FERROR && y != FERROR ){
		int newpick = this->getPicker(x,y);
		this->setPicker(newpick);
	}
}

void Thumblay::setPicker(int newpick)
{
	if( this->core->getCacher() != NULL ){
		if( newpick == this->picker )
			this->hide();
		else if( newpick != INT_MAX ){
			HCURSOR last = NULL;
			if( this->core->getGui() != NULL )
				last = this->core->getGui()->setCursor(CURSOR_ARRWAIT);

			int i, diff = abs(newpick - this->picker);
			bool result = false;

			for( i = 0; i < diff; i++ ){
				if( newpick < this->picker ){
					if( this->core->getCacher()->prev() == false )
						break;
					this->picker--;
					if( this->picker < -THB_COUNT )
						this->picker = -THB_COUNT + THB_ROW - 1;

					result = true;
				}
				else {
					if(	this->core->getCacher()->next() == false )
						break;
					this->picker++;
					if( this->picker > THB_COUNT )
						this->picker = THB_COUNT - THB_ROW + 1;

					result = true;
				}
			}
			if( result == true ){
				this->subrender();
				if( this->core->getGui() != NULL )
					this->core->getGui()->updateText();

				this->invalidate(false);
			}
			if( this->core->getGui() != NULL )
				this->core->getGui()->setCursor(last);
		}
	}
}

int Thumblay::getPicker(int x, int y)
{
	RECT lay = this->getOverlayRect();
	int picker = INT_MAX;

	if( x > lay.left && x < lay.right &&
		y > lay.top && y < lay.bottom ){

		x -= (lay.left + OVL_MARGIN);
		y -= (lay.top + OVL_MARGIN);

		int col = (int)floor( (double)(x / (THB_SMSIZE + THB_SPACE)) );
		int row = (int)floor( (double)(y / (THB_SMSIZE + THB_SPACE)) );

		if( col < THB_ROW && row < THB_ROW )
			picker = col - THB_COUNT + THB_ROW * row;
	}
	return picker;
}

void Thumblay::scroll(int hor, int ver)
{
	return;
}
void Thumblay::scrollSet(int x, int y)
{
	return;
}
void Thumblay::scrollHor(int val)
{
	if( val < 0 )
		this->setPicker(this->picker + 1);
	else if( val > 0 )
		this->setPicker(this->picker - 1);

	/*if( this->core->getCacher() != NULL ){
		bool result = false;
		HCURSOR last = NULL;
		if( this->core->getGui() != NULL )
			last = this->core->getGui()->setCursor(CURSOR_ARRWAIT);

		if( val < 0 )
			result = this->core->getCacher()->next();

		else if( val > 0 )
			result = this->core->getCacher()->prev();

		if( result == true ){
			this->subrender();
			if( this->core->getGui() != NULL )
				this->core->getGui()->updateText();

			this->invalidate(false);
		}
		if( this->core->getGui() != NULL )
			this->core->getGui()->setCursor(last);
	}*/
}
void Thumblay::scrollVer(int val)
{
	if( val < 0 )
		this->setPicker(this->picker + THB_ROW);
	else if( val > 0 )
		this->setPicker(this->picker - THB_ROW);

	/*if( this->core->getCacher() != NULL ){
		bool result = false;
		HCURSOR last = NULL;
		if( this->core->getGui() != NULL )
			last = this->core->getGui()->setCursor(CURSOR_ARRWAIT);

		int i = 0;
		if( val < 0 )
			for( i = 0; i < THB_ROW; i++ )
				if( this->core->getCacher()->next() == true )
					result = true;
				else
					break;
			
		else if( val > 0 )
			for( i = 0; i < THB_ROW; i++ )
				if( this->core->getCacher()->prev() == true )
					result = true;
				else
					break;
			
		if( result == true ){
			this->subrender();
			if( this->core->getGui() != NULL )
				this->core->getGui()->updateText();

			this->invalidate(false);
		}
		if( this->core->getGui() != NULL )
			this->core->getGui()->setCursor(last);
	}*/
}

void Thumblay::zoomer(double val)
{
	return;
}
void Thumblay::zoomat(double val)
{
	return;
}
void Thumblay::zoomend()
{
	return;
}

void Thumblay::rotate(int val)
{
	return;
}
void Thumblay::rotateReset(bool novalid)
{
	return;
}

void Thumblay::setFitmode(int mode)
{
	return;
}
void Thumblay::unsetFitmode()
{
	return;
}
void Thumblay::setSidedraw()
{
	return;
}
void Thumblay::setSidemode(int mode)
{
	return;
}

void Thumblay::subrender()
{
	Graphics *gfx = Graphics::FromImage(this->image);
	gfx->Clear(CLR_WHITE);

	Cacher *cacher = this->core->getCacher();
	if( cacher != NULL ){
		Image *thumb = NULL;
		Cell *cell = NULL;
		int i, x, y, mx, my, count;
		bool top, bot, left, right;

		right = false;
		left = false;
		top = false;
		bot = false;

		mx = OVL_MARGIN + 2 * (THB_SMSIZE + THB_SPACE);
		my = mx;

		x = y = OVL_MARGIN;
		count = 0;

		for( i = -THB_COUNT - this->picker; i <= THB_COUNT - this->picker; i++ ){
			cacher->lockCache();

			if( cacher->getCache() != NULL ){
				cell = cacher->getCache()->gettoThat(i);
				if( cell != NULL ){
					thumb = cell->getImageThumb();
					if( thumb != NULL ){
						if( i != 0 ){
							gfx->DrawImage(thumb,x,y,THB_SMSIZE,THB_SMSIZE);
							gfx->DrawRectangle(this->Pen_Border,
											   x,
											   y,
											   THB_SMSIZE,
											   THB_SMSIZE);
						}	
						else {
							mx = x + (int)((THB_SIZE - THB_SMSIZE)/4);
							my = y + (int)((THB_SIZE - THB_SMSIZE)/4);
						}
					}
					if( i == -THB_ROW )
						top = true;
					if( i == THB_ROW )
						bot = true;
					if( i == -1 && cacher->getCache()->isThatHead() == false )
						left = true;
					if( i == 1 && cacher->getCache()->isThatTail() == false )
						right = true;
				}
			}
			count++;
			x += THB_SMSIZE + THB_SPACE;
			if( count >= THB_ROW ){
				x = OVL_MARGIN;
				y += THB_SMSIZE + THB_SPACE;
				count = 0;
			}
			cacher->unlockCache();
		}
		int frame = 2;
		cacher->lockCache();
		if( cacher->getCache() != NULL ){
			cell = cacher->getThat();
			if( cell != NULL ){
				thumb = cell->getImageThumb();
				if( thumb != NULL ){
					gfx->FillRectangle(this->Brush_Back,
									   mx - THB_SIZE/4,
									   my - THB_SIZE/4,
									   THB_SIZE,
									   THB_SIZE);
					gfx->DrawImage(thumb,
								   mx - THB_SIZE/4,
								   my - THB_SIZE/4,
								   THB_SIZE,
								   THB_SIZE);
					gfx->DrawRectangle(this->Pen_Border,
									   mx - THB_SIZE/4,
									   my - THB_SIZE/4,
									   THB_SIZE,
									   THB_SIZE);
					gfx->DrawRectangle( this->Pen_DarkBorder,
										mx - frame - THB_SIZE/4,
										my - frame - THB_SIZE/4,
										THB_SIZE + 2*frame,
										THB_SIZE + 2*frame );
				}
			}
		}
		cacher->unlockCache();

		int size = 3;
		int width = 10;
		int ax = (int)(mx - frame - THB_SIZE/4);
		int ay = (int)(my - frame - THB_SIZE/4);
		int asize = THB_SIZE + 2*frame;
		Point arrow[3];

		if( top == true ){
			arrow[0].X = (int)(ax + (THB_SIZE/2) - width);
			arrow[0].Y = ay;
			arrow[1].X = (int)(ax + (THB_SIZE/2));
			arrow[1].Y = ay - width;
			arrow[2].X = (int)(ax + (THB_SIZE/2) + width);
			arrow[2].Y = ay;
			gfx->FillPolygon(this->Brush_DarkBack,arrow,size);
		}
		if( bot == true ){
			arrow[0].X = (int)(ax + (THB_SIZE/2) - width);
			arrow[0].Y = ay + asize;
			arrow[1].X = (int)(ax + (THB_SIZE/2));
			arrow[1].Y = ay + asize + width;
			arrow[2].X = (int)(ax + (THB_SIZE/2) + width);
			arrow[2].Y = ay + asize;
			gfx->FillPolygon(this->Brush_DarkBack,arrow,size);
		}
		if( left == true ){
			arrow[0].X = ax;
			arrow[0].Y = (int)(ay + (THB_SIZE/2) - width);
			arrow[1].X = ax - width;
			arrow[1].Y = (int)(ay + (THB_SIZE/2));
			arrow[2].X = ax;
			arrow[2].Y = (int)(ay + (THB_SIZE/2) + width);
			gfx->FillPolygon(this->Brush_DarkBack,arrow,size);
		}
		if( right == true ){
			arrow[0].X = ax + asize;
			arrow[0].Y = (int)(ay + (THB_SIZE/2) - width);
			arrow[1].X = ax + asize + width;
			arrow[1].Y = (int)(ay + (THB_SIZE/2));
			arrow[2].X = ax + asize;
			arrow[2].Y = (int)(ay + (THB_SIZE/2) + width);
			gfx->FillPolygon(this->Brush_DarkBack,arrow,size);
		}
	}
	if( this->ticker > TICKER_OFF ){
		int tsize = TICKER_SIZE;
		int tx,ty;
		if( this->ticker == 0 ){
			tx = OVL_SIZE - 2*TICKER_INDENT;
			ty = OVL_SIZE - 2*TICKER_INDENT;
		}
		if( this->ticker == 1 ){
			tx = OVL_SIZE - TICKER_INDENT;
			ty = OVL_SIZE - 2*TICKER_INDENT;
		}
		if( this->ticker == 2 ){
			tx = OVL_SIZE - TICKER_INDENT;
			ty = OVL_SIZE - TICKER_INDENT;
		}
		if( this->ticker == 3 ){
			tx = OVL_SIZE - 2*TICKER_INDENT;
			ty = OVL_SIZE - TICKER_INDENT;
		}
		gfx->FillRectangle(this->Brush_DarkBack,tx,ty,tsize,tsize);
	}
	delete gfx;
}

Cell *Thumblay::getLastCell()
{
	return this->lastCell;
}