# BombermanUnreal 
# by Diego de Palacio
# October, 2107


INTRODUCTION

The game required from my side more time than expected, because the last time that I worked with Unreal was 6 years ago and it was Unreal Engine 3 without blueprints and with UnrealScript instead.

For that reason I had to practically learning from the scratch how it's nowadays working, check the documentation and some tutorials during the weekend before starting, and during the development of the project. 


FUTURE IMPROVEMENTS

There are many TODO's in the comments of the code, pointing tweakings that can improve the game a lot.

Besides that, the project can be also improved by using pools instead of dynamically allocate / deallocate memory.

Adding more work on the art side, including a main menu and some instructions about how to play could be really nice.

Music, sounds, more power-ups, exposal of the board configuration variables to be changed in the editor and customized for adding more levels.

Customization of the players, and maybe multiplayer on different devices, for having more than two players at the same time and with the possiblity of deploy the game in other platforms like Android or iOS.

Also, there are some cleaning / optimization tasks that can be done, like the usage of templates or more smart methods in order to avoid calling the same group of lines in different methods, like for parsing between real square coordinates to board coordinates (removing almost half of the size of the odd columns) and viceversa.

A timer for ending the game if there is no winner after some time.

Of course also the suggested optional tasks for the test, like automatic camera zooming depending on the distance of the players or adding additional NPC with AI.

With this last feature, a "solo-mode" campaign can be also created.

Particles and some translucent overlay to inform the user the type of tile in where is standing.


3RD PARTY ASSETS

The sprites that are used in the game was downloaded from:
http://bomberman.wikia.com/wiki/Power-Ups
https://thenounproject.com/term/destroy/383967/
https://storage.googleapis.com/csfirst-files/files/Explosion.png

By the way, I decided to use sprites for the bombs, blasts and power-ups, because I didn't liked the way of overlapping 3D assets and then I wanted to test how could be the look and feel with this mix between 3D assets and sprites.

A low poly version of the 3D Pawn was obtained in:
https://sketchfab.com/models/8e40312a89c24240b37ead49805ce173#

But I have to open this version in 3D Max, merging the vertices and applying a TurboSmooth in order to have the final version as it is in the game.

Finally, the materials for the ground, walls and in general the environment, was obtained from a online course that I bought specifically to prepare this test:
https://www.udemy.com/unreale4/


FINAL NOTE

Even spending more time than was planned at the beginning, I have to say that I had a lot of fun doing the game, analizyng the original version in order to optimize the development as much as possible (like the limitation of the fixed position of the indestructible walls).

I also enjoyed to create the game without using the physics library of the engine, because it was not needed and in any case thinking the game as a grid-based make the architecture more robust and more flexible to the specific bomberman-type of game.

With the current state of the game is very easy to add new power-ups, inheriting from the Modifier class and using composition with the TriggerModifierOnProcessTimer class.

I hope that you enjoy reviewing my test as I did while working on it... :)

Please let me know what you think.


Thank you for reading this and for checking my test!
