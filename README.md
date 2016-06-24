# Team Indecisive #
A 3D Engine written in C++ and DirectX 11 by Luis Checa in collaboration with Jordan Issa and Israfel Sskaara. This was a team project to be submitted for our university's Advance Windows Games Programming module.
The project was originally made in Visual Studio 2013, but was later upgraded to Visual Studio 2015.

The purpose was to critically evaluate options, and select appropriate techniques for the collaborative development of a windows-based computer game. while allowing to systematically understand and explain modern game development concepts and techniques.

Our engine resulted in a core engine housing the interfaces which other components would implement, such as Input, Sound, Physics and AI.

Each team member developed at least one component, attributed as follows:
	- Input and Sound: Jordan Issa
	- Physics and Testing: Israfel Sskaara
	- AI, Loader, Core Engine, and Team Leader: Luis Checa

The team leader had the additional responsabilities of aiding the other members when stuck, making sure the code could be reused and fixing bug.

## Engine and Components ##
The core engine has the following interfaces:
	- IDrawable: Any component that needs to be drawn, such as graphics.
	- IUpdatable: Any component that needs to be updated, such as Physics or AI.
	- IGameObject: A base object it will usually need to be both Drawable and Updatable.
	- IReadable: Any component that needs to read and parse data.
	- IResourceManager: The resource manager handles the data for most application long objects.
	- IGraphics: The minimum methods that need to be used outside of the component itself.

Components:
	- Input, can map actions to specific key presses by using lambdas.
	- Sound, can load and play sounds (currently WAV files only).
	- Physics, can apply forces, such as acceleration, collision, drag, sliding to objects.
	- AI, can navigate a path found from a collection of waypoints and costs.
	- Loader, can parse a file to construct a scene graph for display.

## External Sources ##
[DDSTextureLoader](../blob/master/TI_Engine/DDSTextureLoader.h) from Microsoft's [DirectXTex][1]
[Logger](../blob/master/TI_Engine/Logger.h) from the [Cinder][2] engine

## Building ##
The engine is built with Visual Studio 2015. All projects produce a DLL file, except TI_Game which uses those DLLs to make an EXE with a demo.

Dependencies:
	- TI_Engine has no dependecies other than DirectX.
	- TI_AI, TI_Input, TI_Physics and TI_Sound depend on TI_Engine.
	- TI_Loader depends on all of the above.
	- TI_Game depends on TI_Engine and TI_Loader.

The build order results in
TI_Engine, TI_Physics, TI_Sound, TI_AI, TI_Input, TI_Loader, TI_Game.


[1]: https://github.com/Microsoft/DirectXTex
[2]: http://libcinder.org
