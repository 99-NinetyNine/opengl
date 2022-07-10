#include <OGL3D/Game/OGame.h>
//#include <OGL3D/Game/project.h>

//#include <OGL3D/Game/Tesseletation.h>
//#include <OGL3D/Game/CubeBox.h>
#include <iostream>
int main() {
	try
	{
		OGame game;
      	game.run();
	}
	catch (const std::exception& e)
	{
		std::wclog << e.what() << std::endl;
		return 1;
	}
	return 0;
}