#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

#define MUNCHIECOUNT 50
#define GHOSTCOUNT 4

// Just need to include main header file
#include "S2D/S2D.h"

// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.

//structure definition
struct player
{
	float speedMultiplier;
	int currentFrameTime;
	int direction;
	int frame;
	Rect* pacmanSourceRect;
	Texture2D* texture;
	Vector2* position;
	bool dead;
};

struct movingEnemy
{
	Vector2* position;
	Texture2D* texture;
	Rect* ghostSourceRect;
	Rect* ghostRect;
	int direction;
	float speed;
};

struct munchie
{
	int munchieframeCount;
	int munchieFrame;
	int munchieCurrentFrameTime;
	int munchieFrameCount;
	Rect* munchieRect;
	Rect* munchieSourceRect;
	Texture2D* munchieBlueTexture;
	Texture2D* munchieInvertedTexture;
	Vector2* munchiePosition;
};

struct cherry
{
	Vector2* cherryPosition;
	int cherryFrame;
	int cherryCurrentFrameTime;
	int cherryFrameCount;
	Rect* cherryRect;
	Rect* cherrySourceRect;
	Texture2D* cherryRedTexture;
	Texture2D* cherryInvertedTexture;
};

class Pacman : public Game
{
private:
	
		// input methods
		void Input(int elapsedTime, Input::KeyboardState * keyboardState, Input::MouseState * mouseState);

		

	
		//Check methods
		void CheckPaused(Input::KeyboardState * state, Input::Keys pauseKey);
		void CheckRandom(Input::KeyboardState* MouseState, Input::Keys R);
		void CheckViewportCollision();
		void CheckGhostCollisions();
		void CheckMunchieCollisions();
		void CheckAudioInitialised();
		void CheckPopLoaded();
	
	//Update methods
	void UpdatePacman(int elapsedTime);
	void UpdateMunchie(munchie*, int elapsedTime);
	void UpdateCherry(cherry*, int elapsedTime);
	void UpdateGhost(movingEnemy*, int elapsedTime);

		// Data to represent Pacman
		player* _pacman;
		float _pacmanSpeedMultiplier;
		int _pacmanCurrentFrameTime;
		int _pacmanDirection;
		int _pacmanFrame;
		Rect* _pacmanSourceRect;
		Texture2D* _pacmanTexture;
		Vector2* _pacmanPosition;
		bool _pacmanDead;
		

	//Data to represent Ghost
		movingEnemy* _ghosts[GHOSTCOUNT];
		Vector2* _ghostPosition;
		Texture2D* _ghostTexture;
		Rect* _ghostSourceRect;
		Rect* _ghostRect;
		int _ghostDirection;
		float _ghostSpeed;


	// Data to represent Munchie
		munchie* _munchies[MUNCHIECOUNT];
		int _frameCount;
		int _munchieFrame;
		int _munchieCurrentFrameTime;
		int _munchieFrameCount;
		Rect* _munchieRect;
		Rect* _munchieSourceRect;
		Texture2D* _munchieBlueTexture;
		Texture2D* _munchieInvertedTexture;
		Vector2 _munchiePosition;

	// Data to represent Cherry
		cherry* _cherry;
		Vector2* _cherryPosition;
		int _cherryFrame;
		int _cherryCurrentFrameTime;
		int _cherryFrameCount;
		Rect* _cherryRect;
		Rect* _cherrySourceRect;
		Texture2D* _cherryRedTexture;
		Texture2D* _cherryInvertedTexture;

	// Position for String
	Vector2* _stringPosition;

	//Constant data for Game Variables
	const float _cPacmanSpeed;
	const int _cPacmanFrameTime;
	const int _cMunchieFrameTime;
	const int _cCherryFrameTime;

	// Data For Menu
	Texture2D* _menuBackground;
	Rect* _menuRectangle;
	Vector2* _menuStringPosition;
	bool _paused;

	bool _pKeyDown; //P key for pause
	bool _rKeyDown; //R key for random cherry location

	//data for audio
	SoundEffect* _pop;
	SoundEffect* _nom;

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
};