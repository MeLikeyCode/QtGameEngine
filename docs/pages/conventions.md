@page conventions General Coding Conventions

Object Lifetime Conventions
===========================
- container objects own lifetime of containee objects and this is generally implemented by leveraging QObject’s parent/child system
- methods for adding/removing a containee are in the container
- when there is only one containee in the container (i.e. just a setter) and the setter is called again to swap, container will release ownership of the old containee and the client code calling the setter will be responsible for the released objects lifetime

Naming Conventions
==================
- private members (both member functions and variables) should be postfixed with one underscore
    - public members that are meant for internal use (e.g. some slots) should should be treated as private
- class, struct, enum, should use CamelCaseNaming
- functions should use camelCaseWithLowerInitialLetter

Other Conventions
=================
- signals pass the sender as the first argument (this makes it easy for clients to connect to multiple objects)
- expensive “updates” must be called manually (to allow client to make multiple changes and then call update just once)
	-e.g. Map::addPathingMap()/removePathingMap() does not call Map::updateOverallPathingMap(), client makes a bunch of changes to the map, then *manually* calls updateOverallPathingMap() once.
- document every public function you make