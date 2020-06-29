#pragma once

typedef enum {
	E,
	NE,
	N,
	NW,
	W,
	SW,
	S,
	SE
} eRegDir;

eRegDir Next(eRegDir current, int next = 1, bool clockwise = true);

inline eRegDir OppositeDirection(eRegDir current) { return Next(current, 4); }

inline eRegDir OrthogonalDirection(eRegDir current, bool clockwise = true) { return Next(current, 2, clockwise); }

inline eRegDir NegativeDirection(eRegDir current) { return eRegDir((8 - current) % 8); }

inline bool NeighbourDirections(eRegDir dir1, eRegDir dir2) { return (dir1 == Next(dir2)) || (dir2 == Next(dir1)); }
