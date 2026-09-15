# Space Invaders

A small Space Invaders-style game written in C++ as a programming exercise.

The project focuses on gameplay architecture, object lifecycle, resource management,
collision handling, UI/HUD, audio, and simple visual effects.

**Original implementation time:** approximately 25 hours.

The project was later revisited for minor cleanup and presentation as a code sample;
that time is not included in the estimate.

## Implementation

The game-specific code was written by me and includes:

- Game flow and state management
- Player movement and shooting
- Enemy formation and behavior
- Projectile and collision handling
- Score system
- HUD and game-state UI
- Visual effects
- Resource management
- Audio integration
- Object lifecycle management

The code is intentionally kept relatively lightweight for the scope of the project.
For example, collision detection uses straightforward iteration because the maximum
number of active gameplay objects is small and does not justify a spatial
partitioning system.

## Framework

The original programming assignment provided a small low-level framework containing:

- Win32 application setup / entry point
- DirectX 9 rendering
- FMOD integration
- Basic sprite and text rendering

This framework was provided as part of the original assignment and is not presented
as my work.

All game-specific architecture and gameplay implementation were written by me.

## Technology

- C++
- Visual Studio
- DirectX 9
- FMOD

## Project Structure

`lib/game/` contains the primary game implementation and is the main area intended
for code review.

The low-level framework is separated from the game-specific code.

## Build

Open `SpaceInvaders.sln` in Visual Studio and build the solution.

The project is intended for Windows.

