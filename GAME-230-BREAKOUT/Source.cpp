#include "GameObject.h"
#include "Brick.h"
#include "Paddle.h"
#include "Ball.h"
#include "Score.h"


const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 800;
void update_state(float dt);
void render_frame();
void load_level_one();
void load_level_two();
void load_level_three();
void load_next_level();
void empty_scene();
int level;
int lives_left;
int num_paddles = 1;
bool lost = false;
float current_score = 0;
Vector2f speed;
float speedX = 150;
float speedY = 200;
vector<GameObject*> SceneGraph;
vector<Paddle*> Paddles;
RenderWindow window;
Texture t1, t2, t3, t4;
Text replay_message;
Font font;
Sound win;
Sound lose;
Sound brick_break;
SoundBuffer buff;
SoundBuffer buff2;
SoundBuffer buff3;
vector<Texture> texts;




int main()
{
	
	window.create(VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT), "Put something clever here");
	
	t1.loadFromFile("tex1.png");
	t2.loadFromFile("tex2.png");
	t3.loadFromFile("tex3.png");
	t4.loadFromFile("text1.png");
	texts.push_back(t4);
	texts.push_back(t3);
	texts.push_back(t2);
	texts.push_back(t1);
	speed.x = speedX;
	speed.y = speedY;
	level = 1;
	lives_left = 3;
	replay_message.setCharacterSize(24);
	replay_message.setString("Your score was: ??\nPress Enter to play again!");
	replay_message.setPosition(0, 0);
	font.loadFromFile("font.TTF");
	replay_message.setFont(font);
	
	buff.loadFromFile("win.wav");
	win.setBuffer(buff);
	buff2.loadFromFile("brick_break.wav");
	brick_break.setBuffer(buff2);
	buff3.loadFromFile("lose.wav");
	lose.setBuffer(buff3);


 	load_level_one();

	bool alt_down = false;

	Clock clock;

	while (window.isOpen())
	{
		Event  event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == Event::KeyPressed) {
				if (event.key.code == Keyboard::LAlt && !alt_down) {
					alt_down = true;
					load_next_level();
				}
			}
			if (event.type == Event::KeyReleased) {
				if (event.key.code == Keyboard::LAlt) {
					alt_down = false;
				}
			}
		}

		float dt = clock.restart().asSeconds();

		update_state(dt);
		render_frame();
		window.display();
	}

	return 0;
}

void update_state(float dt)
{
	if (!lost) {
		for (auto i = SceneGraph.begin(); i != SceneGraph.end(); ++i) {
			(*i)->update(dt);
		}
		int numBricks = 0;
		for (auto i = SceneGraph.begin(); i != SceneGraph.end(); ++i) {
			if (auto j = dynamic_cast<Brick*>(*i)) {
				numBricks++;
				if (j->getLives() <= 0) {
					delete j;
					SceneGraph.erase(i);
					brick_break.play();
					break;
				}
				else {
					j->setTexture(texts[j->getLives()]);
				}
			}
		}
		if (numBricks == 0) {
			
			load_next_level();
		}
		if (lives_left == 0) {
			lose.play();
			empty_scene();
			lost = true;
		}
	}
	else {
		if (Keyboard::isKeyPressed(Keyboard::Return)) {
			speed.x = speedX;
			speed.y = speedY;
			level = 1;
			lives_left = 3;
			current_score = 0;
			load_level_one();
			lost = false;
		}
	}
	if (Keyboard::isKeyPressed(Keyboard::P) && num_paddles < 2) {
		Paddle * paddle = new Paddle(SCREEN_WIDTH, SCREEN_HEIGHT, speed.x * 1.5, lives_left, 1);
		Paddles.push_back(paddle);
		SceneGraph.push_back(paddle);
		num_paddles++;
	}
}
void render_frame()
{
	window.clear(Color(128));
	if (!lost) {
		for (auto i = SceneGraph.begin(); i != SceneGraph.end(); ++i) {
			(*i)->render(window);
		}
	}
	else {
		ostringstream temp_s;
		temp_s << "Your score was: " << current_score << "\nPress Enter to play again!";
		replay_message.setString(temp_s.str());
		window.draw(replay_message);
	}
	
}

void empty_scene() {
	for (auto i = SceneGraph.begin(); i != SceneGraph.end(); ++i) {
		delete *i;
	}
	SceneGraph.clear();
	Paddles.clear();
}

void load_level_one() {
	for (int i = 0; i < num_paddles; ++i) {
		Paddle * paddle = new Paddle(SCREEN_WIDTH, SCREEN_HEIGHT, speed.x * 1.5, lives_left, i);
		Paddles.push_back(paddle);
		SceneGraph.push_back(paddle);
	}
	Ball * ball = new Ball(SCREEN_WIDTH, SCREEN_HEIGHT, 10, Paddles, speed);
	Score * score = new Score(current_score, SCREEN_WIDTH, SCREEN_HEIGHT);
	for (float i = SCREEN_HEIGHT * .1, k = 2; i < SCREEN_HEIGHT * .3; i += SCREEN_HEIGHT * .1, k--) {
		for (float j = SCREEN_WIDTH * .1; j < SCREEN_WIDTH * .9; j += SCREEN_WIDTH * .1) {
			Brick *b = new Brick(true, false, j, i, SCREEN_WIDTH, SCREEN_HEIGHT, k, ball, score);
			SceneGraph.push_back(b);
		}
	}
	SceneGraph.push_back(ball);
	SceneGraph.push_back(score);
}
void load_level_two() {
	for (int i = 0; i < num_paddles; ++i) {
		Paddle * paddle = new Paddle(SCREEN_WIDTH, SCREEN_HEIGHT, speed.x * 1.5, lives_left, i);
		Paddles.push_back(paddle);
		SceneGraph.push_back(paddle);
	}
	Ball * ball = new Ball(SCREEN_WIDTH, SCREEN_HEIGHT, 10, Paddles, speed);
	Score * score = new Score(current_score, SCREEN_WIDTH, SCREEN_HEIGHT);
	for (float i = 0, k = 2; i < SCREEN_HEIGHT * .5; i += SCREEN_HEIGHT * .2, k--) {
		for (float j = SCREEN_WIDTH * .1; j < SCREEN_WIDTH; j += SCREEN_WIDTH * .2) {
			Brick *b = new Brick(false, false, j, i, SCREEN_WIDTH, SCREEN_HEIGHT, 3, ball, score);
			SceneGraph.push_back(b);
		}
	}
	for (float i = SCREEN_HEIGHT * .1, k = 2; i <= SCREEN_HEIGHT * .5; i += SCREEN_HEIGHT * .2, k--) {
		for (float j = 0; j < SCREEN_WIDTH; j += SCREEN_WIDTH * .2) {
			Brick *b = new Brick(true, false, j, i, SCREEN_WIDTH, SCREEN_HEIGHT, 2, ball, score);
			SceneGraph.push_back(b);
		}
	}
	SceneGraph.push_back(ball);
	SceneGraph.push_back(score);
}
void load_level_three() {
	for (int i = 0; i < num_paddles; ++i) {
		Paddle * paddle = new Paddle(SCREEN_WIDTH, SCREEN_HEIGHT, speed.x * 1.5, lives_left, i);
		Paddles.push_back(paddle);
		SceneGraph.push_back(paddle);
	}
	Ball * ball = new Ball(SCREEN_WIDTH, SCREEN_HEIGHT, 10, Paddles, speed);
	SceneGraph.push_back(ball);
	Brick *b;
	Score * score = new Score(current_score, SCREEN_WIDTH, SCREEN_HEIGHT);

	for (float i = 0, j = 0, k = 1 ; j< SCREEN_WIDTH; i += SCREEN_HEIGHT * .05, j += SCREEN_WIDTH * .1, k += 1 ) {
		if((int)k % 2)
			b = new Brick(false, false, j, i, SCREEN_WIDTH, SCREEN_HEIGHT, 3, ball, score);
		else
			b = new Brick(false, true, j, i, SCREEN_WIDTH, SCREEN_HEIGHT, 3, ball, score);
		SceneGraph.push_back(b);
	}
	for (float i = 0, j = SCREEN_WIDTH * .9, k = 1; j >= 0; i += SCREEN_HEIGHT * .05, j -= SCREEN_WIDTH * .1, k+=1) {
		if ((int)k % 2)
			b = new Brick(false, false, j, i, SCREEN_WIDTH, SCREEN_HEIGHT, 3, ball, score);
		else
			b = new Brick(false, true, j, i, SCREEN_WIDTH, SCREEN_HEIGHT, 3, ball, score);
		SceneGraph.push_back(b);
	}
	SceneGraph.push_back(score);
	
}
void load_next_level() {
	win.play();
	empty_scene();
	level++;
	speed.x += 25;
	speed.y += 25;
	if (level == 2) {
		load_level_two();
	}
	else if (level == 3) {
		load_level_three();
	}
	else if (level == 4) {
		load_level_one();
		level = 1;
	}
}