#include "Knight.h"

Knight::Knight(std::vector<Piece*>& board, Drawable* model, Drawable* model_low, int faction, int row, int column) :Piece(board, model, model_low, faction, row, column) {

}

void Knight::getLegalMoves(std::vector<Piece*> board, std::vector<struct move>& legalMoves) {
	int simpleBoard[8][8];

	memset(simpleBoard, 0, sizeof(simpleBoard[0][0]) * 64);
	for (Piece* p : board) {
		simpleBoard[p->getRow() - 1][p->getColumn() - 1] = p->getFaction();
	}

	legalMoves.push_back({ this->row, this->column, RESET });

	int i = this->row;
	int j = this->column;
	i += 2; j += 1;
	if (!(i > 8 || j > 8)) {
		if (simpleBoard[i - 1][j - 1] == 0) {
			legalMoves.push_back({ i, j, NORMAL });
		}
		else if (simpleBoard[i - 1][j - 1] == this->faction);
		else {
			legalMoves.push_back({ i, j, CAPTURE });
		}
	}
	i = this->row;
	j = this->column;
	i += 1; j += 2;
	if (!(i > 8 || j > 8)) {
		if (simpleBoard[i - 1][j - 1] == 0) {
			legalMoves.push_back({ i, j, NORMAL });
		}
		else if (simpleBoard[i - 1][j - 1] == this->faction);
		else {
			legalMoves.push_back({ i, j, CAPTURE });
		}
	}
	i = this->row;
	j = this->column;
	i -= 1; j += 2;
	if (!(i < 1 || j > 8)) {
		if (simpleBoard[i - 1][j - 1] == 0) {
			legalMoves.push_back({ i, j, NORMAL });
		}
		else if (simpleBoard[i - 1][j - 1] == this->faction);
		else {
			legalMoves.push_back({ i, j, CAPTURE });
		}
	}
	i = this->row;
	j = this->column;
	i -= 2; j += 1;
	if (!(i < 1 || j > 8)) {
		if (simpleBoard[i - 1][j - 1] == 0) {
			legalMoves.push_back({ i, j, NORMAL });
		}
		else if (simpleBoard[i - 1][j - 1] == this->faction);
		else {
			legalMoves.push_back({ i, j, CAPTURE });
		}
	}
	i = this->row;
	j = this->column;
	i -= 2; j -= 1;
	if (!(i < 1 || j < 1)) {
		if (simpleBoard[i - 1][j - 1] == 0) {
			legalMoves.push_back({ i, j, NORMAL });
		}
		else if (simpleBoard[i - 1][j - 1] == this->faction);
		else {
			legalMoves.push_back({ i, j, CAPTURE });
		}
	}
	i = this->row;
	j = this->column;
	i -= 1; j -= 2;
	if (!(i < 1 || j < 1)) {
		if (simpleBoard[i - 1][j - 1] == 0) {
			legalMoves.push_back({ i, j, NORMAL });
		}
		else if (simpleBoard[i - 1][j - 1] == this->faction);
		else {
			legalMoves.push_back({ i, j, CAPTURE });
		}
	}
	i = this->row;
	j = this->column;
	i += 1; j -= 2;
	if (!(i > 8 || j < 1)) {
		if (simpleBoard[i - 1][j - 1] == 0) {
			legalMoves.push_back({ i, j, NORMAL });
		}
		else if (simpleBoard[i - 1][j - 1] == this->faction);
		else {
			legalMoves.push_back({ i, j, CAPTURE });
		}
	}
	i = this->row;
	j = this->column;
	i += 2; j -= 1;
	if (!(i > 8 || j < 1)) {
		if (simpleBoard[i - 1][j - 1] == 0) {
			legalMoves.push_back({ i, j, NORMAL });
		}
		else if (simpleBoard[i - 1][j - 1] == this->faction);
		else {
			legalMoves.push_back({ i, j, CAPTURE });
		}
	}
}