#include "targetver.h"
#include "Shaders.h"
#include "Application.h"
#include "utilities.h" 
#include "soloud.h"
#include "soloud_wav.h"
#include "soloud_speech.h"
#include "soloud_file.h"
#include "soloud_thread.h"

GLint screenWidth = 640;
GLint screenHeight = 720;


GLint Init(ESContext* esContext)
{
	Application::GetInstance()->Init();
	return 0;
}

void Draw(ESContext* esContext)
{
	Application::GetInstance()->Render();
	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
}

void Update(ESContext* esContext, GLfloat deltaTime)
{
	Application::GetInstance()->Update(deltaTime);
}

void Key(ESContext* esContext, unsigned char key, bool bbIsPresseded)
{
	Application::GetInstance()->HandleKeyEvent(key, bbIsPresseded);
}

void Mouse(ESContext* esContext, GLint x, GLint y, bool bbIsPresseded)
{
	Application::GetInstance()->HandleTouchEvent(x, y, bbIsPresseded);
}

void CleanUp()
{
	Application::GetInstance()->Exit();
	Application::FreeInstance();
}

GLint _tmain(GLint argc, _TCHAR* argv[])
{
	//test mp3
	//SoLoud::Soloud soloud;  // SoLoud engine core
	//SoLoud::Wav wav;

	//soloud.init();

	//FILE* p = fopen("Keep_it_Mello.mp3", "r");
	//if (!p)
	//{
	//	return -1;
	//}
	//SoLoud::DiskFile* f = new SoLoud::DiskFile(p);
	//wav.loadFile(f);

	//soloud.play(wav);
	// Define a couple of variables
	SoLoud::Soloud soloud;  // SoLoud engine core
	SoLoud::Speech speech;  // A sound source (speech, in this case)

	// Configure sound source
	speech.setText("1 2 3   1 2 3   Hello world. Welcome to So-Loud.");

	// initialize SoLoud.
	soloud.init();

	// Play the sound source (we could do this several times if we wanted)
	soloud.play(speech);

	// Wait until sounds have finished
	while (soloud.getActiveVoiceCount() > 0)
	{
		// Still going, sleep for a bit
		SoLoud::Thread::sleep(100);
	}

	// Clean up SoLoud
	soloud.deinit();
	//end test
	ESContext esContext;
	esInitContext(&esContext);
	esCreateWindow(&esContext, "LET'S SURF", screenWidth, screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);
	if (Init(&esContext) != 0)
		return 0;

	esRegisterDrawFunc(&esContext, Draw);
	esRegisterUpdateFunc(&esContext, Update);
	esRegisterKeyFunc(&esContext, Key);
	esRegisterMouseFunc(&esContext, Mouse);
	esMainLoop(&esContext);

	//releasing OpenGL resources
	CleanUp();

	//identifying memory leaks
	//MemoryDump();
	//printf("Press any key...\n");
	//_getch();

	return 0;
}

//#endif