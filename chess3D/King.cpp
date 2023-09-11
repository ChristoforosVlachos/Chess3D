#include "King.h"
#include "Rook.h"

King::King(std::vector<Piece*>& board, Drawable* model, Drawable* model_low, int faction, int row, int column) :Piece(board, model, model_low, faction, row, column) {
	hasMoved = false;
}

void King::getLegalMoves(std::vector<Piece*> board, std::vector<struct move>& legalMoves) {
	int simpleBoard[8][8];

	memset(simpleBoard, 0, sizeof(simpleBoard[0][0]) * 64);
	for (Piece* p : board) {
		simpleBoard[p->getRow() - 1][p->getColumn() - 1] = p->getFaction();
	}

	legalMoves.push_back({ this->row, this->column, RESET });

	int i = this->row + 1;
	int j = this->column + 1;
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
	j = this->column + 1;
	if (!(j > 8)) {
		if (simpleBoard[i - 1][j - 1] == 0) {
			legalMoves.push_back({ i, j, NORMAL });
		}
		else if (simpleBoard[i - 1][j - 1] == this->faction);
		else {
			legalMoves.push_back({ i, j, CAPTURE });
		}
	}
	i = this->row - 1;
	j = this->column + 1;
	if (!(i < 1 || j > 8)) {
		if (simpleBoard[i - 1][j - 1] == 0) {
			legalMoves.push_back({ i, j, NORMAL });
		}
		else if (simpleBoard[i - 1][j - 1] == this->faction);
		else {
			legalMoves.push_back({ i, j, CAPTURE });
		}
	}
	i = this->row - 1;
	j = this->column;
	if (!(i < 1)) {
		if (simpleBoard[i - 1][j - 1] == 0) {
			legalMoves.push_back({ i, j, NORMAL });
		}
		else if (simpleBoard[i - 1][j - 1] == this->faction);
		else {
			legalMoves.push_back({ i, j, CAPTURE });
		}
	}
	i = this->row - 1;
	j = this->column - 1;
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
	j = this->column - 1;
	if (!(j < 1)) {
		if (simpleBoard[i - 1][j - 1] == 0) {
			legalMoves.push_back({ i, j, NORMAL });
		}
		else if (simpleBoard[i - 1][j - 1] == this->faction);
		else {
			legalMoves.push_back({ i, j, CAPTURE });
		}
	}
	i = this->row + 1;
	j = this->column - 1;
	if (!(i > 8 || j < 1)) {
		if (simpleBoard[i - 1][j - 1] == 0) {
			legalMoves.push_back({ i, j, NORMAL });
		}
		else if (simpleBoard[i - 1][j - 1] == this->faction);
		else {
			legalMoves.push_back({ i, j, CAPTURE });
		}
	}
	i = this->row + 1;
	j = this->column;
	if (!(i > 8)) {
		if (simpleBoard[i - 1][j - 1] == 0) {
			legalMoves.push_back({ i, j, NORMAL });
		}
		else if (simpleBoard[i - 1][j - 1] == this->faction);
		else {
			legalMoves.push_back({ i, j, CAPTURE });
		}
	}

	// Castling
	i = this->row;
	j = this->column;
	if (!this->hasMoved) {
		// kingside
		if (simpleBoard[i - 1][j] == 0 && simpleBoard[i - 1][j + 1] == 0 && simpleBoard[i - 1][j + 2] == this->faction) {
			for (Piece* p : board) {
				if (Rook* r = dynamic_cast<Rook*>(p)) {
					if (r->getColumn() == j + 3 && r->getRow() == i && !r->hasMoved) {
						legalMoves.push_back({ i, j + 2, CASTLING_KINGSIDE });
						break;
					}
				}
			}
		}
		// queenside
		if (simpleBoard[i - 1][j - 2] == 0 && simpleBoard[i - 1][j - 3] == 0 && simpleBoard[i - 1][j - 4] == 0 && simpleBoard[i - 1][j - 5] == this->faction) {
			for (Piece* p : board) {
				if (Rook* r = dynamic_cast<Rook*>(p)) {
					if (r->getColumn() == j - 4 && r->getRow() == i && !r->hasMoved) {
						legalMoves.push_back({ i, j - 2, CASTLING_QUEENSIDE });
						break;
					}
				}
			}
		}
	}

	return;
}