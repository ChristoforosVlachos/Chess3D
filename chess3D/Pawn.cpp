#include "Pawn.h"

Pawn::Pawn(std::vector<Piece*>& board, Drawable* model, Drawable* model_low, int faction, int row, int column) :Piece(board, model, model_low, faction, row, column) {
	hasMoved = false;
	canBeCapturedEnPassant = false;
}

void Pawn::getLegalMoves(std::vector<Piece*> board, std::vector<struct move>& legalMoves) {
	int simpleBoard[8][8];

	memset(simpleBoard, 0, sizeof(simpleBoard[0][0]) * 64);
	for (Piece* p : board) {
		simpleBoard[p->getRow() - 1][p->getColumn() - 1] = p->getFaction();
	}

	legalMoves.push_back({ this->row, this->column, RESET });

	int front = this->getFaction() == 1 ? 1 : -1;

	int i = this->row;
	i+= front;
	if (i < 1 || i > 8) return;
	if (simpleBoard[i - 1][this->column - 1] == 0) {
		if (i == 1 || i == 8) { // promote
			legalMoves.push_back({ i, this->column, PROMOTE });
		}
		else {
			legalMoves.push_back({ i, this->column, NORMAL });
		}
	}
	if (!(this->column + 1 > 8) && simpleBoard[i - 1][this->column] == this->faction % 2 + 1) {
		if (i == 1 || i == 8) {
			legalMoves.push_back({ i, this->column + 1, CAPTURE_AND_PROMOTE });
		}
		else {
			legalMoves.push_back({ i, this->column + 1, CAPTURE });
		}
	}
	if (!(this->column - 1 < 1) && simpleBoard[i - 1][this->column - 2] == this->faction % 2 + 1) {
		if (i == 1 || i == 8) {
			legalMoves.push_back({ i, this->column - 1, CAPTURE_AND_PROMOTE });
		}
		else {
			legalMoves.push_back({ i, this->column - 1, CAPTURE });
		}
	}
	if (!hasMoved) {
		i += front;
		if (i < 1 || i > 8) return; // impossible, but just in case...
		if (simpleBoard[i - 1][this->column - 1] == 0) {
			legalMoves.push_back({ i, this->column, EN_PASSANT_THREAT });
		}
	}
	// en passant
	i = this->row;
	// right
	if (!(this->column + 1 > 8) && simpleBoard[i - 1][this->column] == this->faction % 2 + 1) {
		for (Piece* p : board) {
			if (p->getColumn() == this->column + 1 && p->getRow() == i) {
				if (Pawn* pawn = dynamic_cast<Pawn*>(p)) {
					if (pawn->canBeCapturedEnPassant) {
						legalMoves.push_back({ i + front, this->column + 1, EN_PASSANT_CAPTURE });
					}
				}
				break;
			}
		}
	}
	// left
	if (!(this->column - 1 < 1) && simpleBoard[i - 1][this->column - 2] == this->faction % 2 + 1) {
		for (Piece* p : board) {
			if (p->getColumn() == this->column - 1 && p->getRow() == i) {
				if (Pawn* pawn = dynamic_cast<Pawn*>(p)) {
					if (pawn->canBeCapturedEnPassant) {
						legalMoves.push_back({ i + front, this->column - 1, EN_PASSANT_CAPTURE });
					}
				}
				break;
			}
		}
	}

}