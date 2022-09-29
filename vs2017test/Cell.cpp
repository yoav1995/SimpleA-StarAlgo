#include "Cell.h"
#include <math.h>

Cell::Cell()
{
}

Cell::Cell(int r, int c)
{
	row = r;
	col = c;
	parent = nullptr; // has no parent
	g = 0;
}

Cell::Cell(int r, int c, Cell* p)
{
	row = r;
	col = c;
	parent = p; // has no parent
	g = parent->getG() + 0.2;
}

Cell::~Cell()
{
}

void Cell::setH(int Trow, int Tcol)
{
	h = sqrt(pow(row - Trow,2) + pow(col - Tcol,2));
}

void Cell::setG()
{
	g = parent->getG() + 1;
}

void Cell::setF()
{
	f = h + g;
}