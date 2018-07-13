@page fileFolderOrginization File/Folder Orginization

Let's look at what's inside the root `QtGameEngine` folder.
- `QtGameEngine` folder - The root folder for the project.
    - `.git` folder - used by git.
    - `docs` folder - all documentation shall go here.
    - `example projects` folder - some example projects using the game engine are here.
    - `qge` folder - source code, project include file, and resources for the game engine.
    - `test project` folder - a project that uses the engine, meant to be used as a quick test harness.
    - `.gitignore` file - specifies files that should be ignored by git version control.

Let's dive in and take a look at what's inside the `docs` folder.
- `QtGameEngine` folder
    - `docs` folder
        - `diagrams` folder - Some diagrams that help visually explain/document high level design/archeteture of the engine.
        - `DoxyfileFast`/`DoxyFileFull` files - these files tell doxygen where the source code is and where to put the documentation (along with some other documentation related options). You run `doxygen DoxyfileFast` or `doxygen DoxyfileFull` to generate the documentation. `DoxyfileFull` takes longer but produces more diagrams.
        - `doxygenOutput` folder - the generated doxygen is put inside an `html` folder which is in this folder.
        - `images` folder - images that the documentation uses are placed here.
        - `pages` folder - additional pages for the documentation.
        - `index.html` file - supposed to represent the root of the documentation website, simply redirets to `doxygenOutput/html/index.html`.
        - `README.md` file - git readme file for the repo.
        - `LICENSE.md` file - git license file for the repo.
        - `CONTRIBUTING.md`/`CODE_OF_CONDUCT.md` files - people interested in contributing to the engine should read these.
        - `_config.yml`/`Gemfile`/`Gemfile.lock` - github uses these files to automatically generate a website for us.

Let's zoom back out and this time dive in to have a look at the `qge` folder.
- `QtGameEngine` folder
    - `qge` folder
        - `graphics` folder - images that the engine uses are placed here.
        - `sounds` folder - sounds/music that the engine uses are placed here.
        - `res.qrc` file - this file allows us to easily package all the qge graphics/sounds into the executable itself
            - Note: The game engine comes with a small number of images/sounds, so it's ok to put them into the executable. If your game has a lot of sounds/images, you probably want to ship them in a folder to avoid bloating the executable.
        - `qge.pri` file - This is the file you `include()` into your game's .pro (project) file so that you can easily use the engine.
        - `.h/.cpp` files - There is a bunch of .h and .cpp files, these are the source of the engine.

Those are the most important parts of the file/folder structure you need to be aware of! 

Thanks for reading!

-Abdullah
