#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#define PI 3.14159265

const int H = 400;
const int W = 400;
const int r = 10;
const float L = 8.416e-10;
const float unit = 5.26e-13;
const float m = 3.3e-26;

using namespace sf;
using namespace std;

void XWallCollisionCalculation(float& x, float& y, float& vx, const float vy, const float var) {
	float x1 = x - vx;
	float y1 = y - vy;
	float len = sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
	float t1 = len / sqrt(vx * vx + vy * vy);
	float t3 = (var - x1) / vx;
	float y3 = y1 + vy * t3;
	vx = -vx;
	float t2 = t1 - t3;
	x = var + vx * t2;
	y = y3 + vy * t2;
}

void YWallCollisionCalculation(float& x, float& y, const float vx, float& vy, const float var) {
	float x1 = x - vx;
	float y1 = y - vy;
	float len = sqrt((x - x1) * (x - x1) + (y - y1) * (y - y1));
	float t1 = len / sqrt(vx * vx + vy * vy);
	float t3 = (var - y1) / vy;
	float x3 = x1 + vx * t3;
	vy = -vy;
	float t2 = t1 - t3;
	x = x3 + vx * t2;
	y = var + vy * t2;
}

struct Atom {
	float x = 50;
	float y = 50;
	float v = 4;
	int angle = -180 + rand() % 181;
	float vx = v * cos(angle);
	float vy = v * sin(angle);
	float r = 10;
	float Step(double& N) {
		x += vx;
		y += vy;
		return CheckWallCollision(N);
	}
	void CheckEachOtherCollision(Atom& neighbor) {
		float dist = sqrt((x - neighbor.x) * (x - neighbor.x) + (y - neighbor.y) * (y - neighbor.y));
		float c = (neighbor.x - x) / dist;
		float s = (neighbor.y - y) / dist;
		if (dist == 0) { dist = 0.001; }
		dist = dist - 2 * r; 

		if (dist < 0) {
			float theor_x = x + vx;
			float theor_y = y + vy;
			float theor_xn = neighbor.x + neighbor.vx;
			float theor_yn = neighbor.y + neighbor.vy;
			float theor_dist = sqrt((theor_x - theor_xn) * (theor_x - theor_xn) + (theor_y - theor_yn) * (theor_y - theor_yn));
			if (dist <= theor_dist) {
				float Vn1 = neighbor.vx * s + neighbor.vy * c;
				float Vn2 = vx * s + vy * c;
				float dt = -dist / (Vn1 - Vn2); 
				if (dt > 1) dt = 0.6;
				if (dt < -1) dt = -0.6;
				x -= vx * dt;
				y -= vy * dt;
				neighbor.x -= neighbor.vx * dt;
				neighbor.y -= neighbor.vy * dt;
				dist = sqrt((x - neighbor.x) * (x - neighbor.x) + (y - neighbor.y) * (y - neighbor.y));
				float s = (neighbor.x - x) / dist;
				float c = (neighbor.y - y) / dist;
				Vn1 = neighbor.vx * s + neighbor.vy * c;
				Vn2 = vx * s + vy * c;
				float Vt1 = -neighbor.vx * c + neighbor.vy * s;
				float Vt2 = -vx * c + vy * s;


				float o = Vn2;
				Vn2 = Vn1;
				Vn1 = o;


				vx = Vn2 * s - Vt2 * c;
				vy = Vn2 * c + Vt2 * s;
				angle = atan2(vy, vx) * 180 / PI;
				neighbor.vx = Vn1 * s - Vt1 * c;
				neighbor.vy = Vn1 * c + Vt1 * s;
				neighbor.angle = atan2(neighbor.vy, neighbor.vx) * 180 / PI;
				x += vx * dt;
				y += vy * dt;
				neighbor.x += neighbor.vx * dt;
				neighbor.y += neighbor.vy * dt;
			}
		}
	}
	float CheckWallCollision(double& N) {
		if (x - r <= 0 || x + r >= W) {
			if (x - r <= 0) {
				XWallCollisionCalculation(x, y, vx, vy, r + 1);
				angle = 180 - angle;
				N++;
			}
			else {
				XWallCollisionCalculation(x, y, vx, vy, W - r - 1);
				angle = 180 - angle;
				N++;
			}
			//cout << N << ' ';
		}
		if (y - r <= 30 || y + r >= H) {
			if (y - r <= 30) {
				YWallCollisionCalculation(x, y, vx, vy, r + 31);
				angle = -angle;
				N++;
			}
			else {
				YWallCollisionCalculation(x, y, vx, vy, H - r - 1);
				angle = -angle;
				N++;
			}
			//cout << N << ' ';
		}
		return N;
	}
};

class Vessel: Atom {
private:
	vector<Atom> atoms;
public:
	Vessel(int n) {
		int rows = n / 16 + 1;
		int cols = 16;

		for (int i = 0; i < rows; i++) {
			if (i == rows - 1) {
				cols = n - rows * 16;
				if (cols < 0) {
					cols = n;
				}
			}
			for (int j = 0; j < cols; j++) {
				Atom a;
				if (j == 0) {
					a.x = 10;
				}
				else {
					a.x = 35 + 25 * (j - 1);
				}
				if (i == 0) {
					a.y = 40;
				}
				else {
					a.y = 65 + 25 * (i - 1);
				}
				atoms.push_back(a);
			}
		}
	}
	vector<Atom> Get() {
		return atoms;
	}
	float Steps() {
		double N = 0;
		for (int i = 0; i < atoms.size(); i++) {
			atoms[i].Step(N);
			for (int j = 0; j < atoms.size(); j++) {
				if (i != j) {
					atoms[i].CheckEachOtherCollision(atoms[j]);
				}
			}
		}
		float F = N;
		return F;
	}
	void Rise() {
		for (auto& atom : atoms) {
			atom.v += 0.5;
			atom.vx = atom.v * cos(atom.angle);
			atom.vy = atom.v * sin(atom.angle);
		}
	}
	void Low() {
		for (auto& atom : atoms) {
			atom.v -= 0.5;
			atom.vx = atom.v * cos(atom.angle);
			atom.vy = atom.v * sin(atom.angle);
		}
	}
};

int main() {
	srand(time(NULL));

	Vessel vessel(100);
	float N = 0;
	float t = 0;

	Color white = Color(255, 255, 255);

	RenderWindow window(VideoMode(W, H), "Vessel");
	CircleShape shape(0);
	RectangleShape plusButton(Vector2f(25, 25));
	RectangleShape minusButton(Vector2f(25, 25));
	Font font;
	font.loadFromFile("geo_1.ttf");
	Text temperature;
	Text plus;
	Text minus;

	while (window.isOpen()) {
		Vector2i mousePos = Mouse::getPosition(window);

		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed)
				window.close();

			if (event.type == Event::MouseButtonPressed) {
				if (event.key.code == Mouse::Left) {
					if (plusButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
						vessel.Rise();
					}
				}
			}

			if (event.type == Event::MouseButtonPressed) {
				if (event.key.code == Mouse::Left) {
					if (minusButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
						vessel.Low();
					}
				}
			}
		}

		window.clear();

		temperature.setFont(font);
		temperature.setString("Temperature");
		temperature.setCharacterSize(25);
		temperature.setPosition(25, 0);
		plusButton.setFillColor(Color(0, 0, 0));
		minusButton.setFillColor(Color(0, 0, 0));
		minusButton.setPosition(170, 0);
		plus.setFont(font);
		plus.setString("+");
		plus.setCharacterSize(25);
		plus.setPosition(5, 0);
		plus.setFillColor(Color(255, 255, 255));
		minus.setFont(font);
		minus.setString("-");
		minus.setCharacterSize(25);
		minus.setPosition(175, 0);
		minus.setFillColor(Color(255, 255, 255));
		window.draw(temperature);
		window.draw(plusButton);
		window.draw(plus);
		window.draw(minus);

		Clock clock;
		for (auto& atom : vessel.Get()) {
			shape.setRadius(atom.r);
			shape.setOrigin(atom.r, atom.r);
			shape.setPosition(atom.x, atom.y);
			shape.setFillColor(white);
			window.draw(shape);
		}
		window.display();
		t += clock.restart().asSeconds();
		if (t >= 1) {
			N += vessel.Steps();
			t = 0;
			cout << N << ' ';
			N = 0;
		}
		else {
			N += vessel.Steps();
		}
	}

	return 0;
}