Welcome!
========

Qt Game Engine (QGE) is a game engine written in C++ and built on top of the amazing Qt framework. It presents a very **easy** and more importantly, a **fun** to use interface for creating your own 2d games from a top down or angled (e.g. isometric-like) viewpoint. The game engine is very **well written**, **well documented**, and **easily extensible**.

Click the picture below to view a video of the engine in action.
[![demo teaser video](images/demoVideoSnapshot.png)](https://www.youtube.com/watch?v=WUk3jxSSSqQ "demo teaser video")

I've written the engine with my top priority being that it is fun to use. For me, building things by plugging a bunch of different parts together and wiring them correctly is insanely fun, so that is how i've designed the engine to be. 

How to use QGE in your project
==============================
If you just want to use the engine to build your own game (not necessarly contribute to the engine codebase itself):
1. Download/extract.
2. Copy the `qge` folder to the same directory as your `.pro` file.
3. Add `include(qge/qge.pri)` in your `.pro` file.

That's it! Now you can simply `#include <qge/SomeHeader.h>` to use QGE classes. I sincerely hope you enjoy *using* the game engine as much as I enjoyed *making* it.

Tutorials/Documentation
=============
I've created some [YouTube tutorials](https://www.youtube.com/playlist?list=PLMgDVIa0Pg8Wglcri_5zQjhoOVONbSVAJ) on creating a basic game using the engine. That is a great place to start! Next, you may want to check out the example projects in the `QtGameEngine/examples` folder. As you're working on the tutorials/examples, have the [documentation](doxygenOutput/html/index.html) open so you can look at the documentation of the various classes you use along your journey.

Contributing
============
Currently, I work full time and am a full time master's student, but I promise to allocate as much free time as I can muster to support the continued development of this engine.

I warmly welcome anyone who would like to contribute to the engine. If you would like to contribute, familiarize yourself with *using* the game engine first, then head on over to the [contribution guidelines page](CONTRIBUTING.md). Additionally, please skim/read the [code of conduct](CODE_OF_CONDUCT.md), which could honestly be summarized as:
    
    Be kind, classy, and professional.
