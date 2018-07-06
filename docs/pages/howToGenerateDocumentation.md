@page howToGenerateDocumentation How to generate documentation

1. launch a shell (powershell, command prompt, bash, etc)
2. cd into the `QtGameEngine/docs` directory
3. run `doxygen DoxyfileFull` or `doxygen DoxyfileFast`. `doxygen DoxyfileFull` will include more diagrams in the documentation but take longer. 

The generated documentation is placed inside the `QtGameEngine/docs/doxygenOutput` folder.

Note: Obviously, you need to have `doxygen` installed and it should be found in your `PATH` environment variable.