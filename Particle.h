#include <random>
#include <iostream>
#include <cmath>
#include <time.h>
#include <SFML/Graphics.hpp>

//The Particle class
//Contains all important features of one particle, like position, angle, velocity and other properties
struct Particle{
	
	Particle();
	Particle(float maxX, float maxY);
	~Particle();
	
	void setPosition(float x, float y);
	void init(float maxX, float maxY);
	void advanceParticle(float maxX, float maxY, float dt);
	void calculatePhiStep();
	void setNeighborhood(unsigned int left, unsigned int right);
	void determineColor();
	
	
	float x = 0.0;
	float y = 0.0;
	float v = 0.67;
	float phi = 0.0;
	float radiusBody = 1.0;
	float lifetime = 10000;
	float alpha = 180.0;
	float beta = 17.0;
	float deltaPhi = 0.0;
	float radiusDetection = 5.0;
	unsigned int localNeighborhoodSizeLeft = 0;
	unsigned int localNeighborhoodSizeRight = 0;
	
	sf::Color color;
	
	
	static std::random_device device;
	static std::mt19937 generator;
	static unsigned int seed;
	static std::uniform_real_distribution<float> distributionVelocity;
	static std::uniform_real_distribution<float> distributionPhi;
	static std::uniform_real_distribution<float> distributionX;
	static std::uniform_real_distribution<float> distributionY;
	static std::uniform_real_distribution<float> distributionRadiusBody;
	
};

//Random generator as static for all particles to be used
std::random_device Particle::device;
unsigned Particle::seed = device();
std::mt19937 Particle::generator(Particle::seed);
std::uniform_real_distribution<float> Particle::distributionVelocity(1.0,10.0);
std::uniform_real_distribution<float> Particle::distributionPhi(0.0,360.0);
std::uniform_real_distribution<float> Particle::distributionX(0.0,10);
std::uniform_real_distribution<float> Particle::distributionY(0.0,10);
std::uniform_real_distribution<float> Particle::distributionRadiusBody(5.5,15.0);


template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

//Particle default constructor
Particle::Particle()
{
	std::cout << "Call to Particle Constructor\n";
	this->x = 1.0;
	this->y = 1.0;
}

//Particle default destructor
Particle::~Particle()
{
	std::cout << "Call to Particle Destructor\n";
}

//Inits the particle to fill the whole space of the world randomly
void Particle::init(float maxX, float maxY)
{
	distributionX = std::uniform_real_distribution<float>(0.0, maxX);
	distributionY = std::uniform_real_distribution<float>(0.0, maxY);
	//this->v = distributionVelocity(generator);
	this->phi = distributionPhi(generator);
	this->x = distributionX(generator);
	this->y = distributionY(generator);
	//this->radius = distributionRadius(generator);
	//this-> lifetime = 50000;
	color = sf::Color::Green;
}

//Sets the position of a particle to x and y
void Particle::setPosition(float x, float y)
{
	std::cout << "Set the position of the particle to x=" << x << " y=" << y << std::endl;
	this->x = x;
	this->y = y;
}

//Advances the particles one delta step and checks for collisions with walls.
void Particle::advanceParticle(float maxX, float maxY, float dt)
{
	phi += deltaPhi*dt;
	x += v*sin(phi/180 * M_PI)*dt;
	y += v*cos(phi/180 * M_PI)*dt;
	
	if (x <= 0){
		phi = 360 - phi;
		//m_v *= 1.1;
	}
	if (x >= maxX){
		phi = 360 - phi;
		//m_v *= 1.1;
	}
	if (y <= 0){
		phi = 180 - phi;
		//m_v *= 1.1;
	}
	if (y >= maxY){
		phi = 180 - phi;
		//m_v *= 1.1;
	}
	
	//lifetime -= 1;
	//if (lifetime < 0){
	//	m_v = 0.0;
	//	m_radius = 0.0;
	//	m_x = -100.0 - fabs(m_x);
	//	m_y = -100.0 - fabs(m_y);
	//}
}

//Calcultes a deltaPhi step via the equation given in "https://www.ncbi.nlm.nih.gov/pmc/articles/PMC5346932/"
void Particle::calculatePhiStep()
{
	this->deltaPhi = alpha + beta *(localNeighborhoodSizeLeft + localNeighborhoodSizeRight) * sgn(-localNeighborhoodSizeRight + localNeighborhoodSizeLeft);
}

//Sets the neighborhood particle variables, saving how many particles are left and right
void Particle::setNeighborhood(unsigned int left, unsigned int right)
{
	this->localNeighborhoodSizeLeft = left;
	this->localNeighborhoodSizeRight = right;
}


//Determines color of particle via local neighborhood
void Particle::determineColor()
{
	if (15 > (localNeighborhoodSizeLeft + localNeighborhoodSizeRight) > 13) {
		color = sf::Color::White;
		cout << "White!\n";
		return;
	}
		
	if (35 > (localNeighborhoodSizeLeft + localNeighborhoodSizeRight) > 15) {
		color = sf::Color::Blue;
		return;
	}
	
	if ((localNeighborhoodSizeLeft + localNeighborhoodSizeRight) > 35) {
		color = sf::Color::Yellow;
		return;
	}
	
	if ((localNeighborhoodSizeLeft + localNeighborhoodSizeRight) < 13) {
		color = sf::Color::Green;
		return;
	}
	
}
