
struct Box
{
	int x = -1, y = -1, w = -1, h = -1;
};

class C_BaseEntity;
class ClientClass;
class Color;
class Vector;
class Drops
{
public:
	bool transf(const Vector& in, Vector& out);
};

extern Drops _drops;