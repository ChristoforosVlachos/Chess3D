#include "Piece.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#define Y_OFFSET 0.01

Piece::Piece() {
	Piece::setRow(0);
	Piece::setColumn(0);
}

Piece::Piece(std::vector<Piece*>& board, Drawable* model, Drawable* model_low, int faction, int row, int column) {
	this->model = model;
	this->model_low = model_low;
	this->faction = faction;
	this->row = row;
	this->column = column;
	this->moving = STOP;
	this->deleteHeight = 0;
	this->shouldFinalizeMove = false;

	this->movementStartTime = 0;
	this->previousColumn = 0;
	this->previousRow = 0;

	float miny = model->vertices[0].y;
	float maxy = miny;
	for (glm::vec3 vertex : model->vertices) {
		if (vertex.y < miny) miny = vertex.y;
		else if (vertex.y > maxy) maxy = vertex.y;
	}
	this->min_y = miny * 0.01f;
	this->max_y = maxy * 0.01f;

	board.push_back(this);
}

int Piece::getRow() {
	return this->row;
}

void Piece::setRow(int row) {
	this->row = row;
}

int Piece::getColumn() {
	return this->column;
}

void Piece::setColumn(int column) {
	this->column = column;
}

Drawable* Piece::getModel() {
	return this->model;
}

Drawable* Piece::getModelLowPoly() {
	return this->model_low;
}

int Piece::getFaction() {
	return this->faction;
}

void Piece::startMoving(enum movement moving) {
	this->moving = moving;
	this->previousRow = row;
	this->previousColumn = column;
	this->movementStartTime = glfwGetTime();
}

glm::mat4 Piece::getModelMatrix(bool isLastPass) {
	switch (moving) {
		case STOP:
			return glm::translate(glm::mat4(), glm::vec3((2 * (column - 1) + 1) * 0.3766875, Y_OFFSET, (2 * (row - 1) + 1) * -0.3766875)) * glm::scale(glm::mat4(), glm::vec3(0.01f));
		case WALK:
			return getModelMatrixWalk(isLastPass);
		case JUMP:
			return getModelMatrixJump();
		case GET_CAPTURED:
			return getModelMatrixCaptured(isLastPass);
		case ROOK_CASTLE:
			return getModelMatrixRookCastle(isLastPass);
		case GET_PROMOTED:
			return getModelMatrixPromoted(isLastPass);
		default:
			std::cout << "Unknown move type!" << std::endl;
	}
}

glm::mat4 Piece::getModelMatrixWalk(bool isLastPass) {
	double dt = glfwGetTime() - movementStartTime;

	glm::vec2 direction = normalize(glm::vec2(column, row) - glm::vec2(previousColumn, previousRow));
	double distance = length(glm::vec2(column, row) - glm::vec2(previousColumn, previousRow));
	glm::vec2 newPos = glm::vec2(previousColumn, previousRow) + static_cast<float>(dt * distance) * direction;
	double x = newPos.x;
	double y = newPos.y;

	if (dt >= 1) {
		moving = STOP;
		shouldFinalizeMove = true;
		return getModelMatrix(isLastPass);
	}
	else {
		return glm::translate(glm::mat4(), glm::vec3((2 * (x - 1) + 1) * 0.3766875, Y_OFFSET, (2 * (y - 1) + 1) * -0.3766875)) * glm::scale(glm::mat4(), glm::vec3(0.01f));
	}
}

glm::mat4 Piece::getModelMatrixJump() {
	double dt = glfwGetTime() - movementStartTime;
	glm::vec3 e = glm::vec3(column, row, 0) - glm::vec3(previousColumn, previousRow, 0);
	glm::vec3 n = normalize(cross(glm::vec3(column, row, 0) - glm::vec3(previousColumn, previousRow, 0), glm::vec3(column, row, 10) - glm::vec3(previousColumn, previousRow, 0)));
	glm::vec3 direction = glm::mat3(rotate(glm::mat4(), static_cast<float>(3.14159 * cos(3.14159/2*dt)), n)) * e;
	glm::vec3 center = glm::vec3(previousColumn, previousRow, 0) + 0.5f * length(e) * normalize(e);
	glm::vec3 newPos = center + 0.5f*direction;
	double x = newPos.x;
	double y = newPos.y;
	double z = newPos.z;

	if (dt >= 1) {
		moving = STOP;
		shouldFinalizeMove = true;
		return getModelMatrix(false);
	}
	else {
		return glm::translate(glm::mat4(), glm::vec3((2 * (x - 1) + 1) * 0.3766875, 2*z + Y_OFFSET, (2 * (y - 1) + 1) * -0.3766875)) * glm::scale(glm::mat4(), glm::vec3(0.01f));
	}
}

glm::mat4 Piece::getModelMatrixCaptured(bool isLastPass) {
	double dt = glfwGetTime() - movementStartTime;
	if (dt < 1) {
		return glm::translate(glm::mat4(), glm::vec3((2 * (column - 1) + 1) * 0.3766875, 3 * dt + Y_OFFSET, (2 * (row - 1) + 1) * -0.3766875)) * glm::scale(glm::mat4(), glm::vec3(0.01f));
	}
	else if (dt < 2) {
		deleteHeight = dt - 1;
		return glm::translate(glm::mat4(), glm::vec3((2 * (column - 1) + 1) * 0.3766875, 3 + Y_OFFSET, (2 * (row - 1) + 1) * -0.3766875)) * glm::scale(glm::mat4(), glm::vec3(0.01f));
	}
	else {
		if (isLastPass) {
			moving = STOP;
		}
		deleteHeight = 0;
		shouldFinalizeMove = true;
		return glm::scale(glm::mat4(), glm::vec3(0));
	}
}

glm::mat4 Piece::getModelMatrixRookCastle(bool isLastPass) {
	double dt = glfwGetTime() - movementStartTime;
	if (dt < 1) {
		return glm::translate(glm::mat4(), glm::vec3((2 * (previousColumn - 1) + 1) * 0.3766875, Y_OFFSET, (2 * (previousRow - 1) + 1) * -0.3766875)) * glm::scale(glm::mat4(), glm::vec3(0.01f));
	}
	else if (dt < 2) {
		return glm::translate(glm::mat4(), glm::vec3((2 * (previousColumn - 1) + 1) * 0.3766875, -(dt-1) + Y_OFFSET, (2 * (previousRow - 1) + 1) * -0.3766875)) * glm::scale(glm::mat4(), glm::vec3(0.01f));
	}
	else if (dt < 3) {
		return glm::translate(glm::mat4(), glm::vec3((2 * (column - 1) + 1) * 0.3766875, (dt-2)-1 + Y_OFFSET, (2 * (row - 1) + 1) * -0.3766875)) * glm::scale(glm::mat4(), glm::vec3(0.01f));
	}
	else {
		moving = STOP;
		//shouldFinalizeMove = true;
		return getModelMatrix(isLastPass);
	}
}

glm::mat4 Piece::getModelMatrixPromoted(bool isLastPass) {
	double dt = glfwGetTime() - movementStartTime;
	if (dt < 2) {
		// morphing in progress...
		return glm::translate(glm::mat4(), glm::vec3((2 * (column - 1) + 1) * 0.3766875, Y_OFFSET, (2 * (row - 1) + 1) * -0.3766875)) * glm::scale(glm::mat4(), glm::vec3(0.01f));
	}
	else {
		moving = STOP;
		shouldFinalizeMove = true;
		return getModelMatrix(isLastPass);
	}
}