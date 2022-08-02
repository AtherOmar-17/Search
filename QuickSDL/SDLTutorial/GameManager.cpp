//---------------------------------------------------------------------//
// GameManager.cpp                                                     //
// Used to initialize and release all other manager                     //
// Contains the game loop as well as the Update and Render functions   //
// Used to make sure all functions are called in the correct order     //
//                                                                     //
// By: Ather Omar                                                      //
//---------------------------------------------------------------------//
#include "GameManager.h"
//-----------------------------------------------------------
// QuickSDL
//-----------------------------------------------------------
namespace QuickSDL {
	//Initializing to NULL
	GameManager* GameManager::sInstance = NULL;

	GameManager* GameManager::Instance() {

		//Create a new instance if no instance was created before
		if(sInstance == NULL)
			sInstance = new GameManager();

		return sInstance;
	}

	void GameManager::Release() {

		delete sInstance;
		sInstance = NULL;
	}

	GameManager::GameManager() {

		mQuit = false;

		//Initialize SDL
		mGraphics = Graphics::Instance();

		// Quits the game if SDL fails to initialize
		if(!Graphics::Initialized())
			mQuit = true;

		//Initialize AssetManager
		mAssetMgr = AssetManager::Instance();

		//Initialize InputManager
		mInputMgr = InputManager::Instance();

		//Initialize AudioManager
		mAudioMgr = AudioManager::Instance();

		//Initialize Timer
		mTimer = Timer::Instance();

		m_grid = new Grid(40, 20, 20, 20, 2);
		m_grid->Pos(Vector2(Graphics::Instance()->SCREEN_WIDTH * 0.5f, Graphics::Instance()->SCREEN_HEIGHT * 0.5f));
	}

	GameManager::~GameManager() {

		AudioManager::Release();
		mAudioMgr = NULL;

		AssetManager::Release();
		mAssetMgr = NULL;

		Graphics::Release();
		mGraphics = NULL;

		InputManager::Release();
		mInputMgr = NULL;

		Timer::Release();
		mTimer = NULL;
	}

	void GameManager::EarlyUpdate() {

		mTimer->Reset();
		//Updating the input state before any other updates are run to make sure the Input check is accurate
		mInputMgr->Update();
	}

	void GameManager::Update() {

		//GameEntity Updates should happen here

		m_grid->Update();
	}

	void GameManager::LateUpdate() {

		//Any collision detection should happen here

		mInputMgr->UpdatePrevInput();
	}

	void GameManager::Render() {

		//Clears the last frame's render from the back buffer
		mGraphics->ClearBackBuffer();

		//All other rendering is to happen here

		m_grid->Render();

		//Renders the current frame
		mGraphics->Render();
	}

	void GameManager::Run() {

		while(!mQuit) {

			mTimer->Update();

			while(SDL_PollEvent(&mEvents) != 0) {
				//Checks if the user quit the game
				if(mEvents.type == SDL_QUIT) {

					mQuit = true;
				}
			}

			//Limits the frame rate to FRAME_RATE
			if(mTimer->DeltaTime() >= (1.0f / FRAME_RATE)) {

				EarlyUpdate();
				Update();
				LateUpdate();
				Render();
			}
		}
	}
}