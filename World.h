#include <vector>
#include <cmath>
#include "Particle.h"

using namespace std;

//The world class. Has all particles and loops the logic
struct World{
	World();
	World(unsigned int numberParticles, int x, int y, float dt);
	~World();
	
	void startWorld();
	void advanceParticles();
	void checkForCollisions();
	void loopWorld();
	void checkEvents();
	void drawParticles();
	void calculateDeltaPhis();
	void checkNeighborhood();
	
	int maxX = 512;
	int maxY = 512;
	
	float dt = 0.05;
	
	float xDistance = 0.0;
	float yDistance = 0.0;
	float angleForConnectionVector = 0.0;
	
	vector<Particle> particles;	
	
	sf::RenderWindow window;
	sf::CircleShape shape;
};

//Default constructor
World::World()
{
	std::cout << "Call to World Constructor\n";
}

//Default destructor
World::~World()
{
	std::cout << "Call to World Destructor\n";
}


//Constructor to inti all particles
World::World(unsigned int numberParticles, int x, int y, float dt)
{
	particles = vector<Particle>(numberParticles);	
	maxX = x;
	maxY = y;
	for (unsigned int i = 0; i < particles.size(); i++){
		particles[i].init(maxX, maxY);
	}
	
	this->dt = dt;
	shape.setFillColor(sf::Color::Green);
}

//Starts the world, inits a graphic handler window
void World::startWorld()
{
	window.create(sf::VideoMode(maxX, maxY), "PPS!");
	loopWorld();
	
}

//Loops the whole World one delta time step
void World::loopWorld()
{
	while (window.isOpen())
    {
		//Advance Logic
		checkNeighborhood();
		calculateDeltaPhis();
		advanceParticles();
        
        //Draw Graphics
        window.clear();
        checkEvents();
        drawParticles();
        window.display();
    }
	
}

//Checks for sfml events, like exiting the window
void World::checkEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();
			
		//if (event.mouseButton.button == sf::Mouse::Right)
		//{
		//	std::cout << "the right button was pressed" << std::endl;
		//	std::cout << "mouse x: " << event.mouseButton.x << std::endl;
		//	std::cout << "mouse y: " << event.mouseButton.y << std::endl;
		//}
	}
}


//Draws the particles to the graphic handler window
void World::drawParticles()
{
	for (unsigned int i = 0; i< particles.size(); i++)
	{
		particles[i].determineColor();
		shape.setPosition(particles[i].x, particles[i].y);
		shape.setRadius(particles[i].radiusBody);
		shape.setFillColor(particles[i].color);
		window.draw(shape);
			
	}
}

//Calculates the delta phi steps for all particles inside the world
void World::calculateDeltaPhis()
{
	for (unsigned int i = 0; i < particles.size(); i++){
		particles[i].calculatePhiStep();
	}
}

//Advances all particles by one time step
void World::advanceParticles()
{
	for (unsigned int i = 0; i < particles.size(); i++){
		particles[i].advanceParticle(maxX, maxY, dt);
	}
}

//Checks for collision of particles and reinits them if true
void World::checkForCollisions()
{
	for (unsigned int i = 0; i < particles.size(); i++){
		for (unsigned int j = 0; j < particles.size(); j++){
				if (i == j)
					continue;
			if (((particles[i].x - particles[j].x)*(particles[i].x - particles[j].x) + (particles[i].y - particles[j].y)*(particles[i].y - particles[j].y)) < (particles[i].radiusBody + particles[j].radiusBody)*(particles[i].radiusBody + particles[j].radiusBody)){
				particles[j].init(maxX, maxY);
				particles[i].init(maxX, maxY);
			}
		}
	}
}

//Checks all particles for their local neighborhood and calculates the number of local particles
void World::checkNeighborhood()
{
	for (unsigned int i = 0; i < particles.size(); i++){
		for (unsigned int j = 0; j < particles.size(); j++){
			
			particles[i].localNeighborhoodSizeLeft = 0;
			particles[i].localNeighborhoodSizeRight = 0;
			
			if (i == j)
				continue;
			xDistance = particles[i].x - particles[j].x;		
			yDistance = particles[i].y - particles[j].y;
			
			//First check for particles in range
			if ((xDistance*xDistance + yDistance*yDistance) < (particles[j].radiusDetection)*(particles[j].radiusDetection)){
				
				//Now check for side
				angleForConnectionVector = atan(yDistance/xDistance) *180 /M_PI + 90.0;
				
				//First particle
				if ( (angleForConnectionVector - particles[i].phi) < 180)
					particles[i].localNeighborhoodSizeLeft += 1;
				else
					particles[i].localNeighborhoodSizeRight += 1;
				
				
				//Second particle	
				//angleForConnectionVector += 180;
				//if (angleForConnectionVector > 360)
				//	angleForConnectionVector -= 360;
				
				//if ( (angleForConnectionVector - particles[j].phi) < 180)
				//	particles[j].localNeighborhoodSizeLeft += 1;
				//else
				//	particles[j].localNeighborhoodSizeRight += 1;
					
			}
		}
	}
}
