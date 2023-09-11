#pragma once
#include "Piece.h"
#include <vector>

class Knight : public Piece
{
	public:
		Knight(std::vector<Piece*>& board, Drawable* model, Drawable* model_low, int faction, int row, int column);
		void getLegalMoves(std::vector<Piece*> board, std::vector<struct move>& legalMoves);
};

