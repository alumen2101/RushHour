#pragma once

#include "env.h"

class Piece {
public:
	size_t m_position;
	size_t m_size;
	Dir m_dir;
	Bitmap m_mask;

	explicit Piece(size_t pos, size_t size, Dir dir);

	void Move(int steps);
};