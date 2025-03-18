#include "../include/piece.h"

Piece::Piece(size_t pos, size_t size, Dir dir)
	: m_position(pos), m_size(size), m_dir(dir), m_mask(0)
{
	size_t p = pos;
	for (size_t i = 0; i < size; ++i) {
		m_mask |= (Bitmap)1 << p;
		p += (size_t)dir;
	}
}

void Piece::Move(int steps) {
	const int d = (int)m_dir * steps;
	m_position += d;
	if (steps > 0)
		m_mask <<= d;
	else
		m_mask >>= -d;
}