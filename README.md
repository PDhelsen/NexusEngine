# Nexus Engine

Nexus Engine is a small C++ game engine made on my spare time to learn and improve my knowledge of 3D game engine.

# Supported Platform, Compiler,  Requirements

Nexus Engine is supported only Windows and MSCV compiler (it uses a few MSCV instrics functions).
The project generates the visual studio files with Premake.
Both Premake and Visual Studio build tools are expected to be available through environment variables in order to run the Scripts.

# How to build ?

It is required to have built and deploy NexusFramework first as the engine is based on it.

Run the Scripts/Solution/Solution.bat to generate the Visual Studio files, then run the Scripts/Build/Command/Build* to build the solution.
Finally in order to use the engine dll to create project (i.e. NexusSandbox), run the Scripts/Deploy <path>. This last step will copy the binaries and the includes to the provided path and ask the user to store that path into the environment variable, that will allows the project to find the corresponding dlls and includes.
