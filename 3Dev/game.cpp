#define NOMINMAX
#include <Texture.h>
#include <Camera.h>
#include <Light.h>
#include <Skybox.h>
#include <Model.h>
#include <Animation.h>
#include <Shape.h>

struct player {
	float x, z, speed, health = 100, damage;
	int enemy;
	void ai(float ex, float ez, float time) {
		if (z > ez) z -= speed * time;
		if (z < ez) z += speed * time;
		if (x > ex) x -= speed * time;
		if (x < ex) x += speed * time;
	}
};

int main() {
	srand(int(time(0)));
	const int players_amount = 100; //Change it if you want another number of players
	
	//Settings for SFML window
	sf::ContextSettings s;
	s.depthBits = 24;
	//////////////////////////

	//SFML window
	sf::RenderWindow w(sf::VideoMode(1366, 768), "3Dev", sf::Style::Default, s);
	w.setFramerateLimit(60);
	//OpenGL settings
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_NOTEQUAL, 0);
	glDepthMask(GL_TRUE);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(45.f, 1366.f / 768.f, 0.1, 50000.f);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	//////////////////
	//Loading some textures
	GLuint texture = LoadTexture("t.jpg");

	GLuint skybox[6];
	skybox[0] = LoadTexture("resources/skybox3/skybox_front.bmp");
	skybox[1] = LoadTexture("resources/skybox3/skybox_back.bmp");
	skybox[2] = LoadTexture("resources/skybox3/skybox_left.bmp");
	skybox[3] = LoadTexture("resources/skybox3/skybox_right.bmp");
	skybox[4] = LoadTexture("resources/skybox3/skybox_bottom.bmp");
	skybox[5] = LoadTexture("resources/skybox3/skybox_top.bmp");
	///////////////////////
	Camera cam(300, 20, 300, 2);
	/////////////////////////////////////////////////
	Model cyborg("cyborg.obj", "cyborg_diffuse.png", 0, 0, 0); //Cyborg model
	Shape map(1000, 2, 1000, 0, -2, 0);
	//Settings for lighting
	float amb[4] = { 2, 2, 2, 1 }; //Ambient
	float dif[4] = { 1, 1, 1, 1 }; //Diffuse
	float spec[4] = { 5, 0, 0, 1 }; //Specular
	float lin[1] = { 0.3 }; //Linear attenuation
	///////////////////////
	//Creating lights
	Light l(GL_LIGHT0, 0, 0, 0);
	Light l1(GL_LIGHT1, 0, 0, 0);
	////////////////
	//Light parameters
	l1.SetParameters(amb, GL_AMBIENT); 
	l.SetParameters(dif, GL_DIFFUSE); 
	l.SetParameters(spec, GL_SPECULAR); 
	l.SetParameters(lin, GL_LINEAR_ATTENUATION); 
	//////////////////
	//Material
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 64);
	sf::Clock clock;
	std::vector<player> a;
	for(int i = 0; i < players_amount; i++) {
		player temp;
		temp.x = rand() % 1000;
		temp.z = rand() % 1000;
		temp.enemy = rand() % players_amount;
		temp.damage = 0.1 + 1 * rand() / (float)RAND_MAX;
		temp.speed = 0.5 + 1 * rand() / (float)RAND_MAX;
		a.push_back(temp);
	}
	//Main cycle
	while (w.isOpen()) {
		sf::Event event;
		while (w.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				w.close();
			}
			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)) {
				w.close();
			}
		}

		float time = clock.getElapsedTime().asMilliseconds();
		clock.restart();
		time = time / 40;
		if (time > 5) time = 5;
		
		l.SetPosition(cam.x, cam.y, cam.z);
		
		l.Update();
		
		//Camera controls
		cam.Move(time);
		cam.Mouse(w);
		/////////////////

		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		cam.Look();
		map.Draw(texture);
		
		for(int i = 0; i < a.size(); i++) {
			a[i].ai(a[a[i].enemy].x, a[a[i].enemy].z, time);
			if (a[i].x <= a[a[i].enemy].x + 10 && a[i].x >= a[a[i].enemy].x - 10) {
				if (a[i].z <= a[a[i].enemy].z + 10 && a[i].z >= a[a[i].enemy].z - 10) {
					a[a[i].enemy].health -= a[i].damage * time;
				}
			}
			cyborg.SetPosition(a[i].x, 0, a[i].z);
			cyborg.Draw();
			if(a[a[i].enemy].health <= 0) {
				a[i].enemy = rand() % a.size();
			}
			if(a[i].health <= 0) {
				a.erase(a.begin() + i);
				std::cout << a.size() << " players left" << std::endl;
			}
		}
		
		glDisable(GL_LIGHTING);
		glTranslatef(cam.x, cam.y, cam.z);
		RenderSkybox(skybox, 2000); //Drawing skybox
		glTranslatef(-cam.x, -cam.y, -cam.z);
		glEnable(GL_LIGHTING);
		
		w.display();
	}
} 
