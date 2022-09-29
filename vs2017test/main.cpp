//Targil 2
//itay mortov 316334697
//Yoav Herman 315822270
#include "GLUT.h"
#include <math.h>
#include <time.h>
#include <vector>
#include "Cell.h"
#include <queue>
#include <iostream>
#include "CompareCell.h"
#include "CompareCellF.h"

using namespace std;

const int MSZ = 100; // maze size
const int WALL = 0;
const int SPACE = 1;
const int START = 2;
const int TARG = 3;
const int BLACK = 5;
const int GRAY = 4;
const int PATH = 6;

bool bfs_is_running;
bool AStar_is_running;

int maze[MSZ][MSZ] = {0};


vector<Cell*> grays;
priority_queue<Cell*, vector<Cell*>, CompareCell> pq;
priority_queue<Cell*, vector<Cell*>, CompareCellF> pqA;

int Srow, Scol;
int Trow, Tcol;
Cell* startCell;

void initMaze();

void checkneighbor(int row, int col, Cell* pCorr);

void init()
{
	srand(time(0));
	initMaze();
	glClearColor(0, 0, 0.2, 0); // RGB - sets background color
	glOrtho(0, MSZ, 0, MSZ, -1, 1); // setup coordinate system
	
}

void initMaze() {
	// the border is wall by default.
	bool bfs_is_running = false;
	bool AStar_is_running = false;
	for (int i = 1;i < MSZ - 1;i++) {
		for (int j = 1;j < MSZ - 1;j++) {
			if (i % 2 == 1) {// more spaces
				if (rand() % 100 < 85) 
					maze[i][j] = SPACE;
				else
					maze[i][j] = WALL;
			}
			else {
				if (rand() % 100 < 70)
					maze[i][j] = WALL;
				else
					maze[i][j] = SPACE;
			}
		}
	}
	Scol = MSZ / 2;
	Srow = MSZ / 2;
	maze[Srow][Scol] = START;

	// add start point to grays
	Trow = rand() % MSZ;
	Tcol = rand() % MSZ;
	maze[Trow][Tcol] = TARG;
	startCell = new Cell(Srow, Scol);
	startCell->setH(Trow, Tcol);
	grays.push_back(startCell);
	pq.push(startCell);
	pqA.push(startCell);
}

void DrawMaze() {
	for (int i = 0;i < MSZ;i++) {
		for (int j = 0;j < MSZ;j++) {
			//set color
			switch (maze[i][j]) {
			case WALL:
				glColor3d(0.1, 0.6, 0.1);
				break;
			case SPACE:
				glColor3d(1, 1, 1);
				break;
			case START:
				glColor3d(0.1, 0.1,1);
				break;
			case TARG:
				glColor3d(1, 0, 0);
				break;
			case BLACK:
				glColor3d(0, 0, 0);
				break;
			case GRAY:
				glColor3d(0.4, 0.2, 0.2);
				break;
			case PATH:
				glColor3d(0.7, 0, 0.7);
				break;
			
			}
			glBegin(GL_POLYGON);
			glVertex2d(j, i); // i sorot j amodot ( i = y j = x )
			glVertex2d(j, i+1);
			glVertex2d(j+1, i+1);
			glVertex2d(j+1, i);
			glEnd();

		}
	}
}

void RestorePath(Cell* pc) {
	while (pc->getParent() != nullptr) {
		maze[pc->getRow()][pc->getCol()] = PATH;
		pc = pc->getParent();

	}
}

void checkneighbor(int row, int col, Cell* pCorr) {
	// is maze[row][col] a target
	if (maze[row][col] == TARG) {
		cout << "The solution has been found\n";
		bfs_is_running = false;
		AStar_is_running = false;
		RestorePath(pCorr);
	}
	else // this is white neighbor
	{
		Cell* neighbor;
		if (bfs_is_running) {
			neighbor = new Cell(row, col, pCorr);//and father cell (added).
			neighbor->setH(Trow, Tcol);
			neighbor->setF();
			pq.push(neighbor);
			cout << neighbor->getG() << " \n";
			maze[row][col] = GRAY;
		}
		else {//Astar
			Cell* neighbor = new Cell(row, col, pCorr);//and father cell (added).
			neighbor->setH(Trow, Tcol);
			neighbor->setF();
			pqA.push(neighbor);
			cout << neighbor->getG() << " \n";
			maze[row][col] = GRAY;
		}
	}
}

void BfsIteration() {
	if ( pq.empty()) {
		cout << "there is no solution\n";
		bfs_is_running = false;
		return;
	}
	else// grays in not empty
	{// pick the last element from grays.
		Cell* pCorrent = pq.top();
		pq.pop(); //remove pCorrent from pq and paint it black.
		const int r = pCorrent->getRow();
		const int c = pCorrent->getCol();
		if(maze[r][c] != START)
			maze[r][c] = BLACK;
		if (maze[r + 1][c] == SPACE || maze[r + 1][c] == TARG)
			checkneighbor(r + 1, c, pCorrent);
		if (bfs_is_running && (maze[r - 1][c] == SPACE || maze[r - 1][c] == TARG))
			checkneighbor(r - 1, c, pCorrent);
		if (bfs_is_running && (maze[r][c - 1] == SPACE || maze[r][c - 1] == TARG))
			checkneighbor(r, c - 1, pCorrent);
		if (bfs_is_running && (maze[r][c + 1] == SPACE || maze[r][c + 1] == TARG))
			checkneighbor(r, c + 1, pCorrent);
	}
	
}

void AStarIteration() {
	if (pqA.empty()) {
		cout << "there is no solution\n";
		AStar_is_running = false;
		return;
	}
	else// grays in not empty
	{// pick the last element from grays.
		Cell* pCurrent = pqA.top();
		pqA.pop(); //remove pCorrent from pq and paint it black.
		const int r = pCurrent->getRow();
		const int c = pCurrent->getCol();
		maze[r][c] = BLACK;
		if (maze[r + 1][c] == SPACE || maze[r + 1][c] == TARG)
			checkneighbor(r + 1, c, pCurrent);
		if (AStar_is_running && (maze[r - 1][c] == SPACE || maze[r - 1][c] == TARG))
			checkneighbor(r - 1, c, pCurrent);
		if (AStar_is_running && (maze[r][c - 1] == SPACE || maze[r][c - 1] == TARG))
			checkneighbor(r, c - 1, pCurrent);
		if (AStar_is_running && (maze[r][c + 1] == SPACE || maze[r][c + 1] == TARG))
			checkneighbor(r, c + 1, pCurrent);
	}

}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT); // fills all the frame buffer with background color
	DrawMaze();
	glutSwapBuffers();// show what was drawn in "frame buffer"
}

void idle()
{
	if (bfs_is_running)
		BfsIteration();
	if (AStar_is_running)
		AStarIteration();
	glutPostRedisplay(); // calls indirectly to display
}

void menu(int choice) {
	switch (choice) {
	case 1:
		bfs_is_running = true;
		break;
	case 2:
		AStar_is_running = true;
		break;
	case 3:
		while (!pq.empty())
			pq.pop();
		while (!pqA.empty())
			pqA.pop();
		for (int i = 1;i < MSZ - 1;i++) {
			for (int j = 1;j < MSZ - 1;j++) {
				if (maze[i][j] == GRAY || maze[i][j] == BLACK || maze[i][j] == PATH)
					maze[i][j] = SPACE;
			}
		}
		maze[Srow][Scol] = START;
		pq.push(startCell);
		pqA.push(startCell);
		bfs_is_running = false;
		AStar_is_running = false;
		break;
	}
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE); // double means it uses double buffer
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Tar 2");

	glutDisplayFunc(display); // refresh function
	glutIdleFunc(idle); // runs all the time in the background

	glutCreateMenu(menu);
	glutAddMenuEntry("Run BFS", 1);
	glutAddMenuEntry("Run Astar", 2);
	glutAddMenuEntry("Clear matrix", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	init();

	glutMainLoop();
}