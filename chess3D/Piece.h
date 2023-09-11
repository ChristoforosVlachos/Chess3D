#pragma once
#include <vector>
#include <string>
#include <common/model.h>
#include <glfw3.h>

enum moveType {
	RESET, NORMAL, CAPTURE, CASTLING_KINGSIDE, CASTLING_QUEENSIDE, EN_PASSANT_THREAT, EN_PASSANT_CAPTURE, PROMOTE, CAPTURE_AND_PROMOTE
};

struct move {
	int row;
	int column;
	enum moveType type;
};

enum movement { STOP, WALK, JUMP, GET_CAPTURED, ROOK_CASTLE, GET_PROMOTED };

class Piece
{
	public:
		Piece();
		Piece(std::vector<Piece*>& board, Drawable* model, Drawable* model_low, int faction, int row, int column);
		int getRow();
		void setRow(int);
		int getColumn();
		void setColumn(int);
		Drawable* getModel();
		Drawable* getModelLowPoly();
		int getFaction();
		virtual void getLegalMoves(std::vector<Piece*> board, std::vector<struct move>& legalMoves) = 0;
		void startMoving(enum movement);
		glm::mat4 getModelMatrix(bool isLastPass);
		bool shouldFinalizeMove;
		enum movement moving;
		double movementStartTime;
		float deleteHeight;
		float min_y;
		float max_y;
	protected:
		Drawable* model;
		Drawable* model_low;
		int row, column;
		int faction;
		int previousRow, previousColumn;
		glm::mat4 getModelMatrixWalk(bool isLastPass);
		glm::mat4 getModelMatrixJump();
		glm::mat4 getModelMatrixCaptured(bool isLastPass);
		glm::mat4 getModelMatrixRookCastle(bool isLastPass);
		glm::mat4 getModelMatrixPromoted(bool isLastPass);
};

