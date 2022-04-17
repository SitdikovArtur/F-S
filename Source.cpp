#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <stdlib.h>
#include <time.h>

const int H = 400;
const int W = 400;

using namespace sf;
using namespace std;

struct Atom {
	float m = 1;
	float x = 10 + rand() % (W - 9);
	float y = 10 + rand() % (H - 9);
	float dx = 0.5;
	float dy = 0.5;
	float r = 10;
	void Step() {
		x += dx;
		y += dy;

		if ((x + r >= W) || (x - r <= 0)) {
			if (x - r <= 0) {
				x = 11;
			}
			else {
				x = 389;
			}
			dx = -dx;
		}

		if ((y + r >= H) || (y - r <= 0)) {
			if (y - r <= 0) {
				y = 11;
			}
			else {
				y = 389;
			}
			dy = -dy;
		}
	}
	void Translate(const float& dt) {
		x -= dx * dt;
		y -= dy * dt;
	}
};

bool operator==(const Atom& lhs, const Atom& rhs) {
	if (lhs.x == rhs.x && lhs.y == rhs.y) {
		return true;
	}
	else {
		return false;
	}
}

pair<Atom, Atom> Collision(Atom& lhs, Atom& rhs) {
	float d = sqrt((lhs.x - rhs.x) * (lhs.x - rhs.x) + (lhs.y - rhs.y) * (lhs.y - rhs.y));
	if (d <= lhs.r + rhs.r) {
		if (d == 0) { d = 0.001; }
		float v2 = lhs.dx * (lhs.x - rhs.x) / d + lhs.dy * (lhs.y - rhs.y) / d;
		float v1 = rhs.dx * (lhs.x - rhs.x) / d + rhs.dy * (lhs.y - rhs.y) / d;
		float dt = (lhs.r + rhs.r - d) / (v1 + v2);
		if (dt > 0.6) dt = 0.6;
		if (dt < -0.6) dt = -0.6;
		lhs.Translate(dt);
		rhs.Translate(dt);

		d = sqrt((lhs.x - rhs.x) * (lhs.x - rhs.x) + (lhs.y - rhs.y) * (lhs.y - rhs.y));
		if (d == 0) { d = 0.001; }
		v2 = lhs.dx * (lhs.x - rhs.x) / d + lhs.dy * (lhs.y - rhs.y) / d;
		v1 = rhs.dx * (lhs.x - rhs.x) / d + rhs.dy * (lhs.y - rhs.y) / d;
		float vt2 = -lhs.dx * (lhs.y - rhs.y) / d + lhs.dy * (lhs.x - rhs.x) / d;
		float vt1 = -rhs.dx * (lhs.y - rhs.y) / d + rhs.dy * (lhs.x - rhs.x) / d;

		float temp = v2;
		v2 = v1;
		v1 = temp;

		lhs.dx = v2 * (lhs.x - rhs.x) / d - vt2 * (lhs.y - rhs.y) / d;
		lhs.dy = v2 * (lhs.y - rhs.y) / d + vt2 * (lhs.x - rhs.x) / d;
		rhs.dx = v1 * (lhs.x - rhs.x) / d - vt1 * (lhs.y - rhs.y) / d;
		rhs.dy = v1 * (lhs.y - rhs.y) / d + vt1 * (lhs.x - rhs.x) / d;
		lhs.Translate(dt);
		rhs.Translate(dt);
	}
	return make_pair(lhs, rhs);
}

class Vessel {
private:
	vector<Atom> atoms;
public:
	Vessel() {
		for (int i = 0; i < 100; i++) {
			Atom a;
			for (auto& atom : atoms) {
				float d = sqrt
			}
			atoms.push_back(a);
		}
	}
	vector<Atom> Get() {
		return atoms;
	}
	void Steps() {
		for (int i = 0; i < atoms.size(); i++) {
			atoms[i].Step();
			/*for (int j = 0; j < atoms.size(); j++) {
				if (i != j) {
					pair<Atom, Atom> step_result = Collision(atoms[i], atoms[j]);
					atoms[i] = step_result.first;
					atoms[j] = step_result.second;
				}
			}*/
		}
	}
	
};

int main() {
	srand(time(NULL));

	Vessel vessel;

	Color white = Color(255, 255, 255);

	RenderWindow window(VideoMode(W, H), "Vessel");
	CircleShape shape(0);

	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();
		}

		vessel.Steps();

		window.clear();

		for (auto& atom : vessel.Get()) {
			shape.setRadius(atom.r);
			shape.setOrigin(atom.r, atom.r);
			shape.setPosition(atom.x, atom.y);
			shape.setFillColor(white);
			window.draw(shape);
		}

		window.display();

	}

	return 0;
}