#ifndef A3_NETWORK_DEMO_STATE
#define A3_NETWORK_DEMO_STATE

#include "animal3D/animal3D.h"
#include "animal3D-A3DG/a3graphics/a3_TextRenderer.h"
#include "a3Peer.h"
#include "a3KeyboardInput.h"

enum a3GameSelection
{
	UNCHOSEN = -1,
	TICTACTOE,
	CHECKERS,
	MANCALA,
	BATTLESHIP,
	NUM_SELCTIONS
};

enum a3NetworkMode
{
	CONFIG = 0,
	GAME_SELECT,
	PLAYER_SELECT,
	LOBBY,
	SPECTATING,
	CHALLENGING,
	END_GAME,
	NUM_STATES
};

// persistent demo state data structure
struct a3_DemoState
{
	//---------------------------------------------------------------------
	// general variables pertinent to the state

	// terminate key pressed
	a3i32 exitFlag;

	// global vertical axis: Z = 0, Y = 1
	a3i32 verticalAxis;

	// asset streaming between loads enabled (careful!)
	a3i32 streaming;

	// window and full-frame dimensions
	a3ui32 windowWidth, windowHeight;
	a3real windowWidthInv, windowHeightInv, windowAspect;
	a3ui32 frameWidth, frameHeight;
	a3real frameWidthInv, frameHeightInv, frameAspect;
	a3i32 frameBorder;


	//---------------------------------------------------------------------
	// objects that have known or fixed instance count in the whole demo

	// text renderer
	a3i32 textInit, textMode, textModeCount;
	a3_TextRenderer text[1];

	// input
	a3_MouseInput mouse[1];
	a3_KeyboardInput keyboard[1];
	a3_XboxControllerInput xcontrol[4];

	// pointer to fast trig table
	a3f32 trigTable[4096 * 4];

	// --------------------------------------------------------------------
	// NETWORKING STUFF
	a3NetworkMode activeMode = CONFIG;
	a3GameSelection activeGame;
	a3KeyboardInput keyboardInput;
	a3Peer* pPeer;
	a3_Timer renderTimer[1];



	// store remote info by index
	//struct RemoteInformation
	//{
	//	char remoteName[MAX_USERNAME];
	//	int otherdata;
	//};
	//RemoteInformation remote[];

	//GetConnectionList(),GetIndexFromSystemAddress()
	// on connect accept, send back index so they can send it back later so server doesn't need to look them up
};

#endif