#include "Pacman.h"
#include <iostream>
#include <sstream>
#include <time.h>

//constructor method
Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.2f), _cPacmanFrameTime(150), _cMunchieFrameTime(500), _cCherryFrameTime(500)
{
	//local variable
	int i;
	srand(time(NULL));
	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i] = new munchie();
		_munchies[i]->munchieCurrentFrameTime = 0;
		_munchies[i]->munchiePosition = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
		_munchies[i]->munchieRect = new Rect(0.0f, 0.0f, 12, 12);
	}

	for (i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i] = new movingEnemy();
		_ghosts[i]->direction = 0;
		_ghosts[i]->speed = 0.05f;
		_ghosts[i]->position = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	}

	_pacman = new player();
	_pacman->dead = false;
	_pacmanCurrentFrameTime = 0;
	_pacmanFrame = 0;
	_munchieCurrentFrameTime = 0;
	_cherryCurrentFrameTime = 0;

	_pacman->direction = 0;
	_pacman->frame = 0;
	_pacman->currentFrameTime = 0;
	_pacman->speedMultiplier = 1.0f;

	

	_cherry = new cherry();
	_cherry->cherryFrame = 0;
	_cherry->cherryCurrentFrameTime = 0;

	_frameCount = 0;
	_paused = false;
	_pKeyDown = false;

	//Initialise important Game aspects;
	Graphics::Initialise(argc, argv, this, 1024, 768, false, 25, 25, "Pacman", 60);
	Input::Initialise();
	Audio::Initialise();

	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacmanTexture;
	delete _pacmanSourceRect;
	delete _munchieBlueTexture;
	delete _munchieInvertedTexture;
	delete _munchieRect;
	delete _cherryRedTexture;
	delete _cherryInvertedTexture;
	delete _cherrySourceRect;

	//clean up pointers
	delete _pacman->texture;
	delete _pacman->pacmanSourceRect;
	delete _pacman->position;
	delete _pacman;

	delete _munchies[MUNCHIECOUNT]->munchieBlueTexture;
	delete _munchies[MUNCHIECOUNT]->munchieInvertedTexture;
	delete _munchies[MUNCHIECOUNT]->munchieSourceRect;
	delete _munchies[MUNCHIECOUNT];

	delete _cherry->cherryRedTexture;
	delete _cherry->cherryInvertedTexture;
	delete _cherry->cherrySourceRect;
	delete _cherry;

	delete _ghosts[GHOSTCOUNT]->texture;
	delete _ghosts[GHOSTCOUNT]->ghostSourceRect;
	delete _ghosts[GHOSTCOUNT]->position;
	delete _ghosts[GHOSTCOUNT];

	delete _pop;
}

void Pacman::LoadContent()
{
	// Load Pacman
	_pacmanTexture = new Texture2D();
	_pacmanTexture->Load("Textures/Pacman.tga", false);
	_pacmanPosition = new Vector2(350.0f, 350.0f);
	_pacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_pacmanDirection = 0;
	
	_pacman->texture = new Texture2D();
	_pacman->texture->Load("Textures/Pacman.tga", false);
	_pacman->position = new Vector2(150.0f, 150.0f);
	_pacman->pacmanSourceRect = new Rect(0.0f, 0.0f, 32, 32);
	_pacman->direction = 0;

	//load sound
	_pop = new SoundEffect();
	_pop->Load("Sounds/pop.wav");
	_nom = new SoundEffect();
	_nom->Load("Sounds/nom.wav");

	//load ghost
	_ghostTexture = new Texture2D();
	_ghostTexture->Load("Textures/GhostBlue.png", false);
	_ghostPosition = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
	_ghostSourceRect = new Rect(0.0, 0.0, 20, 20);
	_ghostRect = new Rect(0.0f, 0.0f, 20, 20);

	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		_ghosts[i]->texture = new Texture2D();
		_ghosts[i]->texture->Load("Textures/GhostBlue.png", false);
		_ghosts[i]->ghostRect = new Rect(0.0f, 0.0f, 20, 20);
		_ghosts[i]->ghostSourceRect = new Rect(0.0, 0.0, 20, 20);
	}

	// Load Munchie
	_munchieBlueTexture = new Texture2D();
	_munchieBlueTexture->Load("Textures/Munchie.png", true);
	_munchieInvertedTexture = new Texture2D();
	_munchieInvertedTexture->Load("Textures/Munchie.png", true);
	_munchieRect = new Rect(0.0f, 0.0f, 12, 12);
	_munchieSourceRect = new Rect(0, 0, 12, 12);


	//for munchies
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->munchieBlueTexture = _munchieBlueTexture;
		_munchies[i]->munchieInvertedTexture = _munchieInvertedTexture;
		_munchies[i]->munchieRect = _munchieRect;
		_munchies[i]->munchieSourceRect = _munchieSourceRect;
	}
	//_munchies[i]->_munchiebluetexture = _munchieBlueTexture;
	// Load Cherry
	_cherryRedTexture = new Texture2D();
	_cherryRedTexture->Load("Textures/CherrySpriteSheet2.png", true);
	_cherryInvertedTexture = new Texture2D();
	_cherryInvertedTexture->Load("Textures/CherrySpriteSheet2.png", true);
	_cherryPosition = new Vector2((Graphics::GetViewportWidth()) / 2, (Graphics::GetViewportHeight()) / 2);
	_cherryRect = new Rect(0.0f, 0.0f, 32, 32);
	_cherrySourceRect = new Rect(0.0f, 0.0f, 32, 32);

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	//set menu parameters
	_menuBackground = new Texture2D();
	_menuBackground->Load("Textures/Transparency.png", false);
	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());
	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
}

void Pacman::Update(int elapsedTime)
{
	float pacmanSpeed = _cPacmanSpeed * elapsedTime * _pacman->speedMultiplier;
	_pacmanCurrentFrameTime += elapsedTime;
	{
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	// Gets the current state of the Mouse
	Input::MouseState* mouseState = Input::Mouse::GetState();

	//Checks if P key is pressed
	if (keyboardState->IsKeyDown(Input::Keys::P))
	{
		_pKeyDown = true;
		_paused = !_paused;
	}

	if (keyboardState->IsKeyUp(Input::Keys::P))
		_pKeyDown = false;
	
	if (!_paused)
	{
		
		UpdatePacman(elapsedTime);
		Input(elapsedTime, keyboardState, mouseState);
		for (int i = 0; i < GHOSTCOUNT; i++)
		{
			UpdateGhost(_ghosts[i], elapsedTime);
		}
		CheckGhostCollisions();
		CheckMunchieCollisions();

			// Checks if D key is pressed
			if (keyboardState->IsKeyDown(Input::Keys::D))
			{
				_pacmanPosition->X += pacmanSpeed; //Moves Pacman across X axis
				_pacmanDirection = 0;
			}

			//Checks if A key is pressed
			else if (keyboardState->IsKeyDown(Input::Keys::A))
			{
				_pacmanPosition->X -= pacmanSpeed; //Moves Pacman across X axis
				_pacmanDirection = 2;
			}

			//Checks if W key is pressed
			else if (keyboardState->IsKeyDown(Input::Keys::W))
			{
				_pacmanPosition->Y -= pacmanSpeed; //Moves Pacman across Y axis
				_pacmanDirection = 3;
			}

			//Checks if S key is pressed
			else if (keyboardState->IsKeyDown(Input::Keys::S))
			{
				_pacmanPosition->Y += pacmanSpeed; //Moves Pacman across Y axis
				_pacmanDirection = 1;
			}

			if (_pacmanCurrentFrameTime > _cPacmanFrameTime)
			{
				_pacmanFrame++;

				if (_pacmanFrame >= 2)
					_pacmanFrame = 0;

				_pacmanCurrentFrameTime = 0;
			}

			//speed multiplier
			if (keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT))
			{
				//apply multiplier
				_pacman->speedMultiplier = 2.0f;
			}
			else
			{
				//reset multiplier
				_pacman->speedMultiplier = 1.0f;
			}

			//randomising cherry position
			if (keyboardState->IsKeyDown(Input::Keys::R))
			{
				//apply random location
				_cherryPosition = new Vector2((rand() % Graphics::GetViewportWidth()), (rand() % Graphics::GetViewportHeight()));
			}
			
	}



		//Checks if Pacman is trying to dissapear right
		if (_pacmanPosition->X + _pacmanSourceRect->Width > Graphics::GetViewportWidth()) //1024 is game width
		{
			// Pacman hit right wall - reset his position
			_pacmanPosition->X = Graphics::GetViewportWidth() - _pacmanSourceRect->Width;
		}
		//Checks if Pacman is trying to dissapear left
		if (_pacmanPosition->X <= 0) //1024 is game width
		{
			// Pacman hit left wall - reset his position
			_pacmanPosition->X = 0;
		}
		//checks if pacman is trying to dissapear top
		if (_pacmanPosition->Y <= 0)
		{
			//pacman hit top wall - reset his position
			_pacmanPosition->Y = 0;
		}
		//checks if pacman is trying to dissapear down 768 pix
		if (_pacmanPosition->Y + _pacmanSourceRect->Height > Graphics::GetViewportHeight()) //768 is game height
		{
			//pacman hit bottom wall - reset his position
			_pacmanPosition->Y = Graphics::GetViewportHeight() - _pacmanSourceRect->Height;
		}
		_frameCount++;
	}

	_pacmanSourceRect->Y = _pacmanSourceRect->Height * _pacmanDirection;
	_pacmanSourceRect->X = _pacmanSourceRect->Width * _pacmanFrame;

	


	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		_munchies[i]->munchieCurrentFrameTime += elapsedTime;

		if (_munchies[i]->munchieCurrentFrameTime >= _cMunchieFrameTime)
		{
			_munchies[i]->munchieFrameCount++;

			if (_munchies[i]->munchieFrameCount >= 2)
				_munchies[i]->munchieFrameCount = 0;

			_munchies[i]->munchieCurrentFrameTime = 0;

			_munchies[i]->munchieSourceRect->X = _munchies[i]->munchieSourceRect->Width * _munchies[i]->munchieFrameCount;
		}
	}

	_munchieCurrentFrameTime += elapsedTime;

	if (_munchieCurrentFrameTime >= _cMunchieFrameTime)
	{
		_munchieFrameCount++;

		if (_munchieFrameCount >= 2)
			_munchieFrameCount = 0;

		_munchieCurrentFrameTime = 0;

		_munchieSourceRect->X = _munchieSourceRect->Width * _munchieFrameCount;

	}

	_cherryCurrentFrameTime += elapsedTime;

	if (_cherryCurrentFrameTime >= _cCherryFrameTime)
	{
		_cherryFrameCount++;

		if (_cherryFrameCount >= 2)
			_cherryFrameCount = 0;

		_cherryCurrentFrameTime = 0;

		_cherrySourceRect->X = _cherrySourceRect->Width * _cherryFrameCount;
		_cherrySourceRect->Y = _cherrySourceRect->Height * _cherryFrameCount;
	}

	if (!_pop->IsLoaded())
	{
		std::cout << "_pop member sound effect has not loaded" << std::endl;
	}

	if (!_nom->IsLoaded())
	{
		std::cout << "_nom member sound effect has not loaded" << std::endl;
	}

}

void CheckAudioInitialised()
{
	if (!Audio::IsInitialised())
	{
		std::cout << "Audio is not initialised" << std::endl;
	}
}

void CheckPopLoaded()
{
	
}

//updates pacman
void Pacman::UpdatePacman(int elapsedTime)
{

}

//DON'T KNOW WHY THIS IS SHOWING UP RED
	void Pacman::UpdateGhost(movingEnemy* ghost, int elapsedTime)
	{
		for (int i = 0; i < GHOSTCOUNT; i++)
		{
			if (_ghosts[i]->direction == 0) //moves right
			{
				_ghosts[i]->position->X += _ghosts[i]->speed * elapsedTime;
			}
			if (_ghosts[i]->direction == 1) //moves left
			{
				_ghosts[i]->position->X -= _ghosts[i]->speed * elapsedTime;
			}
			if (_ghosts[i]->direction == 2) //moves up
			{
				_ghosts[i]->position->Y -= _ghosts[i]->speed * elapsedTime;
			}
			if (_ghosts[i]->direction == 3) //moves down
			{
				_ghosts[i]->position->Y += _ghosts[i]->speed * elapsedTime;
			}
			if (_ghosts[i]->position->X + _ghosts[i]->ghostSourceRect->Width >= Graphics::GetViewportWidth() - _ghosts[i]->ghostSourceRect->Width) //hits right edge
			{
				_ghosts[i]->direction = rand() % 3; //change direction
			}
			else if (ghost->position->X <= 0) //hits left edge
			{
				_ghosts[i]->direction = rand() % 3;
			}
			else if (ghost->position->Y + _ghosts[i]->ghostSourceRect->Height >= Graphics::GetViewportHeight() - _ghosts[i]->ghostSourceRect->Height) //hits bottom
			{
				_ghosts[i]->direction = rand() %3;
			}
			else if (_ghosts[i]->position->Y <= 0) //hits top
			{
				_ghosts[i]->direction = 3;
			}
		}
	}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacmanPosition->X << " Y: " << _pacmanPosition->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing

	//draws pacman
	if (_pacman->dead == false)
	{
		SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanSourceRect); // Draws Pacman
	}
	else if (_pacman->dead == true)
	{
		Audio::Play(_pop);
		_pacman->position = new Vector2(-100, -100);
		
		std::stringstream menuStream;
		menuStream << "YOU DIED!";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}

	//draws munchies
	for (int i = 0; i < MUNCHIECOUNT; i++)
	{
		if (_munchies[i]->munchieFrameCount < 30)
		{
			SpriteBatch::Draw(_munchies[i]->munchieInvertedTexture, _munchies[i]->munchiePosition, _munchies[i]->munchieSourceRect);
		}
		else
		{
			SpriteBatch::Draw(_munchies[i]->munchieBlueTexture, _munchies[i]->munchiePosition, _munchies[i]->munchieSourceRect); //draws munchies

			_munchies[i]->munchieFrameCount++;

			if (_munchies[i]->munchieFrameCount >= 60)
				_munchies[i]->munchieFrameCount = 0;
		}
	}

	//draws ghosts
	for (int i = 0; i < GHOSTCOUNT; i++)
	{
		SpriteBatch::Draw(_ghosts[i]->texture, _ghosts[i]->position, _ghosts[i]->ghostSourceRect);

	}

	

	{
		// Draws Red Cherry
		SpriteBatch::Draw(_cherryRedTexture, _cherryPosition, _cherrySourceRect);
		


	}
	
	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);

	if (_paused)
	{
		std::stringstream menuStream;
		menuStream << "PAUSED!";

		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr);
		SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}
	SpriteBatch::EndDraw(); // Ends Drawing
}

void Pacman::Input(int elapsedTime, Input::KeyboardState* keyboardState, Input::MouseState* mouseState)
{

}

void Pacman::CheckGhostCollisions()
{
	//local variables
	int i = 0;
	int bottom1 = _pacman->position->Y + _pacman->pacmanSourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->position->X;
	int left2 = 0;
	int right1 = _pacman->position->X + _pacman->pacmanSourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->position->Y;
	int top2 = 0;

	for (i = 0; i < GHOSTCOUNT; i++)
	{
		//populate variables with ghost data
		bottom2 = _ghosts[i]->position->Y + _ghosts[i]->ghostSourceRect->Height;
		left2 = _ghosts[i]->position->X;
		right2 = _ghosts[i]->position->X + _ghosts[i]->ghostSourceRect->Width;
		top2 = _ghosts[i]->position->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			Audio::Play(_pop);
			_pacman->dead == true;
			i = GHOSTCOUNT;
		}
		else if ((left1 > right2) && (left1 < right2) && (top1 > top2) && (top1 < top2))
		{
			Audio::Play(_pop);
			_pacman->dead = true;
			i = GHOSTCOUNT;
		}
		else if (_pacman->position == _ghosts[i]->position)
		{
			Audio::Play(_pop);
			_pacman->dead = true;
			i = GHOSTCOUNT;
		}
	}
}

void Pacman::CheckMunchieCollisions()
{
	//local variables
	int i = 0;
	int bottom1 = _pacman->position->Y + _pacman->pacmanSourceRect->Height;
	int bottom2 = 0;
	int left1 = _pacman->position->X;
	int left2 = 0;
	int right1 = _pacman->position->X + _pacman->pacmanSourceRect->Width;
	int right2 = 0;
	int top1 = _pacman->position->Y;
	int top2 = 0;

	for (i = 0; i < MUNCHIECOUNT; i++)
	{
		//populate variables with munchie data
		bottom2 = _munchies[i]->munchiePosition->Y + _munchies[i]->munchieRect->Height;
		left2 = _munchies[i]->munchiePosition->X;
		right2 = _munchies[i]->munchiePosition->X + _munchies[i]->munchieRect->Width;
		top2 = _munchies[i]->munchiePosition->Y;

		if ((bottom1 > top2) && (top1 < bottom2) && (right1 > left2) && (left1 < right2))
		{
			Audio::Play(_nom);
			_munchies[i]->munchiePosition = new Vector2(-200, -200);
		}

		if ((_pacman->position->X >= _munchies[i]->munchiePosition->X) && (_pacman->position->Y <= _munchies[i]->munchiePosition->Y))
		{
			Audio::Play(_nom);
			_munchies[i]->munchiePosition = new Vector2(-200, -200);
		}

	}
}
