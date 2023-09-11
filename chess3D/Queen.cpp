#include "Queen.h"

Queen::Queen(std::vector<Piece*>& board, Drawable* model, Drawable* model_low, int faction, int row, int column) :Piece(board, model, model_low, faction, row, column) {

}

void Queen::getLegalMoves(std::vector<Piece*> board, std::vector<struct move>& legalMoves) {
	int simpleBoard[8][8];

	memset(simpleBoard, 0, sizeof(simpleBoard[0][0]) * 64);
	for (Piece* p : board) {
		simpleBoard[p->getRow() - 1][p->getColumn() - 1] = p->getFaction();
	}

	legalMoves.push_back({ this->row, this->column, RESET });

	int i, j;

	i = this->row;
	while (true) {
		i--;
		if (i < 1) break;
		if (simpleBoard[i - 1][this->column - 1] > 0) {
			if (simpleBoard[i - 1][this->column - 1] == this->getFaction()) break;
			else {
				legalMoves.push_back({ i, this->column, CAPTURE });
				break;
			}
		}
		legalMoves.push_back({ i, this->column, NORMAL });
	}

	i = this->row;
	while (true) {
		i++;
		if (i > 8) break;
		if (simpleBoard[i - 1][this->column - 1] > 0) {
			if (simpleBoard[i - 1][this->column - 1] == this->getFaction()) break;
			else {
				legalMoves.push_back({ i, this->column, CAPTURE });
				break;
			}
		}
		legalMoves.push_back({ i, this->column, NORMAL });
	}

	i = this->column;
	while (true) {
		i--;
		if (i < 1) break;
		if (simpleBoard[this->row - 1][i - 1] > 0) {
			if (simpleBoard[this->row - 1][i - 1] == this->getFaction()) break;
			else {
				legalMoves.push_back({ this->row, i, CAPTURE });
				break;
			}
		}
		legalMoves.push_back({ this->row, i, NORMAL });
	}

	i = this->column;
	while (true) {
		i++;
		if (i > 8) break;
		if (simpleBoard[this->row - 1][i - 1] > 0) {
			if (simpleBoard[this->row - 1][i - 1] == this->getFaction()) break;
			else {
				legalMoves.push_back({ this->row, i, CAPTURE });
				break;
			}
		}
		legalMoves.push_back({ this->row, i, NORMAL });
	}

	i = this->row;
	j = this->column;
	while (true) {
		i--; j--;
		if (i < 1 || j < 1) break;
		if (simpleBoard[i - 1][j - 1] > 0) {
			if (simpleBoard[i - 1][j - 1] == this->getFaction()) break;
			else {
				legalMoves.push_back({ i, j, CAPTURE });
				break;
			}
		}
		legalMoves.push_back({ i, j, NORMAL });
	}

	i = this->row;
	j = this->column;
	while (true) {
		i--; j++;
		if (i < 1 || j > 8) break;
		if (simpleBoard[i - 1][j - 1] > 0) {
			if (simpleBoard[i - 1][j - 1] == this->getFaction()) break;
			else {
				legalMoves.push_back({ i, j, CAPTURE });
				break;
			}
		}
		legalMoves.push_back({ i, j, NORMAL });
	}

	i = this->row;
	j = this->column;
	while (true) {
		i++; j--;
		if (i > 8 || j < 1) break;
		if (simpleBoard[i - 1][j - 1] > 0) {
			if (simpleBoard[i - 1][j - 1] == this->getFaction()) break;
			else {
				legalMoves.push_back({ i, j, CAPTURE });
				break;
			}
		}
		legalMoves.push_back({ i, j, NORMAL });
	}

	i = this->row;
	j = this->column;
	while (true) {
		i++; j++;
		if (i > 8 || j > 8) break;
		if (simpleBoard[i - 1][j - 1] > 0) {
			if (simpleBoard[i - 1][j - 1] == this->getFaction()) break;
			else {
				legalMoves.push_back({ i, j, CAPTURE });
				break;
			}
		}
		legalMoves.push_back({ i, j, NORMAL });
	}

	return;
}