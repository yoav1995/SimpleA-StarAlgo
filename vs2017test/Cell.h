#pragma once
class Cell
{
private:
	int row;
	int col;
	Cell* parent;
	double h, f, g;

public:
	Cell();
	Cell(int r, int c);
	Cell(int r, int c, Cell* p);
	~Cell();
	int getRow() { return row; }
	int getCol() { return col; }
	Cell* getParent() { return parent; }
	double getH() { return h; }
	void setH(int Trow, int Tcol);
	double getG() { return g; }
	void setG();
	void setF();
	double getF() { return f; }

};