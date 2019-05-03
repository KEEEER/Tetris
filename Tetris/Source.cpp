#include <SFML\Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>
#include <map>
using namespace sf;
using namespace std;

Texture tetris_square[2],background;

const int windowMaxX = 450;
const int windowMaxY = 900;
const int moveStep = 30;
const int fallStep = 30;
const int fallenRate = 10;
const int tetrisX = 30;
const int tetrisY = 30;

vector<vector<int> > shape(7, vector<int>(4));
map<int, int> m;

struct mapPixel {
	bool isFilled;
	Texture t;
	Sprite picSetting;
	mapPixel(bool b, Sprite s) :isFilled(b), picSetting(s) {}
	mapPixel(bool b, Texture texture) :isFilled(b),t(texture){}
};

struct tetrisShape {
	int X, Y, base;
	vector<mapPixel> tetris;
	vector<Sprite> s;
	vector<int> dots;
	tetrisShape(int b, vector<int> d) :base(b), X(0), Y(0) {
		dots.resize(d.size());
		s.resize(d.size());
		for (int i = 0; i < d.size(); i++) {
			dots[i] = d[i];
		}
		cout << "hey" << endl;
	}
	
	void setXY(int x, int y) {
		X = x;
		Y = y;
	}
	void setDots(vector<int> &d) {
		for (int i = 0; i < d.size(); i++) {
			dots[i] = d[i];
		}
	}

	void setTexture(Texture &t) {
		for (int i = 0; i < 4; i++) {
			s[i].setTexture(t);
		}
	}
	vector<vector<int> > getSites() {
		vector<vector<int> > v(4, vector<int>(2));
		for (int i = 0; i < 4; i++) {
			v[i][0] = X + (dots[i] % base) * tetrisX;
			v[i][1] = Y + (dots[i] / base) * fallStep;
		}
		return v;
	}
	void rotate() {
		bool shift = true;
		for (int i = 0; i < 4; i++) {
			dots[i] = m[dots[i]];
			if (dots[i] == 0 || dots[i] == 3 || dots[i] == 6) shift = false;
		}
		if (shift) {
			for (int i = 0; i < 4; i++) {
				dots[i]--;
			}
		}

	}
}shapeT(3, vector<int>{1, 3, 4, 5});

void makeShape() {
	shape[0] = vector<int>{ 1,3,4,5 };
	shape[1] = vector<int>{ 1,2,3,4 };
	shape[2] = vector<int>{ 0,1,4,5 };
	shape[3] = vector<int>{ 1,2,4,5 };
	shape[4] = vector<int>{ 2,3,4,5 };
	shape[5] = vector<int>{ 0,3,4,5 };
	shape[6] = vector<int>{ 0,3,6,9 };
}
void makeRotate() {
	m[0] = 2;
	m[1] = 5;
	m[2] = 8;
	m[3] = 1;
	m[4] = 4;
	m[5] = 7;
	m[6] = 0;
	m[7] = 3;
	m[8] = 6;
}

bool isReachBottom(vector<vector<mapPixel> > &tetrisMap, vector<vector<int> > dots) {
	for (int i = 0; i < dots.size(); i++) {
		int X = dots[i][0] / tetrisX;
		int Y = (dots[i][1] + fallStep) / tetrisY;
		if (dots[i][1] + fallStep < windowMaxY) {
			if (tetrisMap[Y][X].isFilled) return true;
		}
		else return true;
	}
	return false;
	
}
bool isReachRight(vector<vector<mapPixel> > &tetrisMap, vector<vector<int> > dots) {
	for (int i = 0; i < dots.size(); i++) {
		int X = (dots[i][0] + tetrisX) / tetrisX;
		int Y = dots[i][1] / tetrisY;
		if (dots[i][0] + tetrisX < windowMaxX) {
			if (tetrisMap[Y][X].isFilled) return true;
		}
		else return true;
	}
	return false;
}
bool isReachLeft(vector<vector<mapPixel> > &tetrisMap, vector<vector<int> > dots) {
	for (int i = 0; i < dots.size(); i++) {
		int X = (dots[i][0] - tetrisX) / tetrisX;
		int Y = dots[i][1] / tetrisY;
		if (X < 0) return true;
		if (dots[i][0] - tetrisX < windowMaxX) {
			if (tetrisMap[Y][X].isFilled) return true;
		}
		else return true;
	}
	return false;
}

void checkLine(vector<vector<mapPixel> > &tetrisMap) {
	for (int i = tetrisMap.size() - 1 ; i >= 0 ; i--) {
		bool isFullFilled = true;
		for (int j = tetrisMap[i].size() - 1; j >= 0 ; j--) {
			if (tetrisMap[i][j].isFilled == false) {
				isFullFilled = false;
				break;
			}
		}
		if (isFullFilled) {
			for (int a = i; a >= 1; a--) {
				for (int b = tetrisMap[a].size() - 1; b >= 0; b--) {
					tetrisMap[a][b].isFilled = tetrisMap[a - 1][b].isFilled;
				}
			}
			i++;
			//checkLine(tetrisMap);
		}
	}
}

void addMap(vector<vector<mapPixel> > &tetrisMap, tetrisShape ts, int rand) {
	vector<vector<int> > dots(shapeT.getSites());
	for (int i = 0; i < dots.size(); i++) {
		int X = dots[i][0] / tetrisX;
		int Y = dots[i][1] / tetrisY;
		tetrisMap[Y][X].isFilled = true;
		tetrisMap[Y][X].picSetting.setPosition(dots[i][0], dots[i][1]);
		tetrisMap[Y][X].t = tetris_square[rand];
		tetrisMap[Y][X].picSetting.setTexture(tetrisMap[Y][X].t);
		cout << tetrisMap[Y][X].picSetting.getTexture() << endl;
		checkLine(tetrisMap);
	}
}
void drawTetris(RenderWindow &window, tetrisShape &ts) {
	vector<vector<int> > v(ts.getSites());
	for (int i = 0; i < 4; i++) {
		ts.s[i].setPosition(v[i][0], v[i][1]);
		window.draw(ts.s[i]);	
	}
}
void drawMap(RenderWindow &window, vector<vector<mapPixel> > &tetrisMap) {
	for (int i = 0; i < tetrisMap.size(); i++) {
		for (int j = 0; j < tetrisMap[i].size(); j++) {
			if (tetrisMap[i][j].isFilled) {
				window.draw(tetrisMap[i][j].picSetting);
			}
		}
	}
}
void drawHelpLine(RenderWindow &window, vector<vector<mapPixel> > &tetrisMap, tetrisShape shapeTemp) {
	sf::RectangleShape rectangle;
	sf::RectangleShape rectangleCover;
	rectangle.setFillColor(sf::Color(255, 255, 255));
	rectangleCover.setFillColor(sf::Color(211, 211, 211));
	rectangle.setSize(sf::Vector2f((float)tetrisX - 2 , (float)tetrisY - 2));
	rectangleCover.setSize(sf::Vector2f(windowMaxX, windowMaxY));
	rectangleCover.setPosition(0, 0);
	window.draw(rectangleCover);
	for (int i = 0; i < windowMaxX / tetrisX ; i++) {
		for (int j = 0; j < windowMaxY / tetrisY; j++) {
			rectangle.setPosition(i*tetrisX , j*tetrisY);
			window.draw(rectangle);
		}
	}
	rectangle.setFillColor(sf::Color(211, 211, 211));
	while (!isReachBottom(tetrisMap, shapeTemp.getSites())) {
		shapeTemp.setXY(shapeTemp.X, shapeTemp.Y + fallStep);
	}
	vector<vector<int> > site(shapeTemp.getSites());
	for (int i = 0; i < 4; i++) {
		rectangle.setPosition(site[i][0], site[i][1]);
		window.draw(rectangle);
	}
}
int main() {
	makeShape();
	makeRotate();
	string p[2];
	p[0] = "./graphics/square_small.png";
	p[1] = "./graphics/blue_rect_small.png";
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> rDots(0.0, 7.0);
	std::uniform_real_distribution<double> color(0.0, 2.0);
	shapeT.setDots(shape[(int)rDots(mt)]);
	int fallen = 0;
	RenderWindow window(VideoMode(windowMaxX, windowMaxY), "Tetris");
	background.loadFromFile("./graphics/unfilled_square_small.jpg");
	tetris_square[0].loadFromFile("./graphics/square_small.png");
	tetris_square[1].loadFromFile("./graphics/blue_rect_small.png");
	vector<vector<mapPixel> > tetrisMap(windowMaxY / tetrisY, vector<mapPixel>(windowMaxX / tetrisX, mapPixel(false,background)));
	int colorTemp = (int)color(mt);
	shapeT.setTexture(tetris_square[colorTemp]);
	while (window.isOpen()) {
		sf::Event e;
		string preEvent = "Queen";
		while (window.pollEvent(e)) {
			if (e.type == Event::Closed) {
				window.close();
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
				preEvent = "Right";
				if (!isReachRight(tetrisMap, shapeT.getSites())) {
					shapeT.setXY(shapeT.X + tetrisX, shapeT.Y);
				}
				cout << "R pressed" << endl;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
				preEvent = "Left";
				if (!isReachLeft(tetrisMap, shapeT.getSites())) {
					shapeT.setXY(shapeT.X - tetrisX, shapeT.Y);
				}
				cout << "L pressed" << endl;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
				preEvent = "Down";
				while (!isReachBottom(tetrisMap, shapeT.getSites())) {
					shapeT.setXY(shapeT.X, shapeT.Y + fallStep);
					window.clear(Color::White);
					drawTetris(window, shapeT);
					drawMap(window, tetrisMap);
					window.display();
				}
				cout << "D pressed" << endl;
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
				preEvent = "Up";
				shapeT.rotate();
				cout << "Up pressed" << endl;

				
			}
		}
		if (++fallen > fallenRate) {
			fallen = 0;
			if (!isReachBottom(tetrisMap, shapeT.getSites())) {
				shapeT.setXY(shapeT.X, shapeT.Y + fallStep);
			}
			else {
				addMap(tetrisMap, shapeT, colorTemp);
				colorTemp = (int)color(mt);
				shapeT.setXY(0, 0);
				shapeT.setDots(shape[(int)rDots(mt)]);
				shapeT.setTexture(tetris_square[colorTemp]);
			}
		}
		//window.clear(Color::Black);
		window.clear(Color::White);
		drawHelpLine(window, tetrisMap, shapeT);
		drawTetris(window, shapeT);
		drawMap(window, tetrisMap);
		
		window.display();
		
		sf::sleep(sf::milliseconds(30));
		for (int i = 0; i < tetrisMap[0].size(); i++) {
			if (tetrisMap[0][i].isFilled) {
				sf::sleep(sf::milliseconds(3000));
				return -1;
			}
		}
	}
}