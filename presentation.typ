#let lihan(x) = text(blue, x)
#let sebas(x) = text(red, x)
#let leti(x) = text(green, x)
#let aldo(x) = text(purple, x)

= *Quidmasters: Petbuilder* Presentation.

lihan = #lihan[blue]\
sebas = #sebas[red]\
leti = #leti[green]\
aldo = #aldo[purple]\

#lihan[
== Introduction

_(copiado del doc):_ This report describes the process of the development of a top-down 2D arena shooter game, combined with elements from a virtual pet game. In the game, your objective is to take care of your pet by killing the enemies that are trying to kill you and feeding them to your pet, whose hunger grows every second, all this while taking care of the fog, which forces you to keep moving around.
For the implementation of the game, Object-Oriented Programming is used in various aspects, as an example, the whole state of the game is a structure containing different information such as the position of the player, all the currently existent bullets, all the enemies, the health and hunger of your pet, etc., as well as in many other features like the bullets, the player or the enemies.
The main objective of this project was to give a twist to normal Virtual Pet Games, giving it more action elements, and to be able to create a casual yet fun game to play, with all design ideas working correctly as intended.


== Methodology

_(copiado del doc):_ For the game, we used C++20 with the help of the SFML library in its 2.6.1 version, which is a library including graphics and sound backends, so it includes everything we needed for the program. 
As for the programming itself, the first of our worries was to first make the game a game, implementing all the main necessary features before worrying about perfecting each part to make it more pleasant-looking and fun.
]

#aldo[

== Game implementation

=== How does the game engine work?

  Let's explain how the game's inner workings are by looking at the main function.
  We first open the SFML window and init the *`GameState`*. This is a class which contains all the state of the game. Everything that the player sees is merely an interpretation of the `GameState`.
  Then we create an object of type `Assets` which loads all of the game assets on its constructor.
  We set the `stage` (a field inside the `GameStage`) to be at the menu (`GameStage::Menu`).
  We then begin the game loop, which is a piece of code that will execute every frame of the game (roughly 60 to 144 times per second, depending on your monitor). We can divide the game loop into two parts: _update_ and _draw_. 
  - During the _update_ phase we call functions that both read and modify the game state. We call each of these functions *systems*. It is here where we calculate the game logic.
  - During the _draw_ phase we call functions that read from the `GameState` and write into the `sf::RenderWindow`.
]

// Flowchart:
// Program entry point;
// Open window;
// Load assets;
// while (!shouldClose) {
//   switch (GameStage) {
//     case Playing:
//       Player systems;
//       Bullet systems;
//       Enemy systems;
//       Miscellaneous systems;
//       Draw game;
//       break;
//     case Menu:
//       Update menu;
//       Draw menu;
//       break;
//     case GameOver:
//       Draw game over screen;
//       if (Enter was pressed) 
//         GameStage = Playing;
//       break;
//   }
// }
// Quit

#lihan[
=== ```cpp systems::player::movement```

This system is occupied of the player's movement. Depending on which keys are pressed (W, A, S or D) we create a vector pointing in the direction chosen by the player. We normalize this vector to avoid having a greater magnitude during diagonal movement. We add this vector to the player's position. Finally we also calculate the vector from the player to the mouse, to make the player look towards the mouse.

=== ```cpp systems::player::hunger```

Occupied of the reducing the nourishment of the pet over time. This nourishment is refilled when killing zombies.

=== ```cpp systems::player::fog```

Makes the player lose sanity when going out of the viewing area. 
This is to discourage using the outside as an escape.

=== ```cpp systems::player::loseCondition```

Checks if the either:

+ the player's health is equal to or below 0
+ the pet's nourishment is equal to or below 0
+ the player's sanity is equal to or below 0

If any of these is true, we set the `GameStage` to game over.

=== ```cpp systems::player::quidPickup```

Occupied of checking the player's collision with coins on the floor.
If this occurs, we add tha quid to the player's pouch, also playing a sound effect.

=== ```cpp systems::player::warnings```
Makes the pet scold you when he's hungry, requesting food immedieately. Or else.
Increases the volume of a heartbeat sound effect as your health lowers.
]

#leti[
=== ```cpp systems::player::guns```

Advances all of the cooldown timers for the guns.
Checks if the player is clicking, if so request to fire the currently selected gun.
If the cooldown timer for this gun is over, then we can reset the cooldown timer and spawn bullets.
Spawning bullets involves:

- Getting randomly generated values for the damage, knockback and bulletSpeed (according to the gun specification)
- Generating an amount of bullets with these values. The amount depends on the gun.
- Setting the position of these bullets to be at the player position.
- Setting the velocity to be the forward direction of the player.
- Adding the generated bullets to the global bullet vector (in `GameState`).

This system also handles gun switching. It checks if any of the 1-9 keys were pressed, and if so, it tries to switch.
If the user does not yet have that gun, the switch is simply ignored.

=== ```cpp systems::bullets::physics```

Iterates through each bullet in the global bullet vector.
For each bullet, we apply the velocity to the position.
After that, we check if the bullet's collided with any enemy simply by checking the distances with all of them.
_(Interjection: Yes. This is a bit slow. We don't use quadtrees or other optimizations like that. The time complexity is $"O"(m n)$), 
where $m$ is the amount of bullets and $n$ the amount of enemies.)_

If the bullet has collided with an enemy, we remove health from the enemy and play a sound to let the player know they have hit an enemy.

=== ```cpp systems::bullets::homing```

One weapon produces a special type of bullets: homing bullets. These bullets "chase" the enemies. They work by being affected by a gravitational/electric field generated by the enemies. This accelerational field is generated at a point $hat(p_x)$ by adding all of the accelerational fields for each enemy at $p_i$ with the formula:

$ a = sum_i C/(norm(arrow(p_i) - arrow(p_x))^2) (arrow(p_i) - arrow(p_x)) $

$C$ is a constant that we can tweak to our liking.

We approximate this field discretely by creating an 80x60 matrix of vectors,
each element representing one square unit of the game in the viewing area of the screen. We use the previously shown 
formula to calculate the field for every point.

After, we iterate through each homing bullet and get where it lands on the matrix.
If it does land on an entry in the matrix, we get the acceleration and apply it to the bullet, modifying its velocity.
]

#sebas[
=== ```cpp systems::waves```

In charge of spawning the enemies, starting and ending waves and breaks.
The system begins by checking if we are in breaktime or not.

_(sebas, you will be provided a flowchart, don't worry if it's hard to explain with words only.)_

- If not in breaktime
  - Check if all the enemies due on this wave have spawned already.
  - If no enemies are alive and all the enemies have already spawned, *the wave is over*
    - If this is so, we set the break time to 10 seconds and we set the `GameState.inBreak` to be `true`.
  - We advance the `enemySpawnTimer`. If it reaches 0:
    - We restart the timer.
    - We spawn an enemy of each class.
- If in breaktime
  - Advance the break time timer
  - If the timer reaches 0:
    - We set the set the new enemies due-to-spawn in this wave. (This basically consists of filling an array where each element corresponds to an enemy type with how many enemies of that type will spawn)
    - We set `GameState.inBreak` to `false`.

=== ```cpp systems::enemy::ai```

Accelerates the enemies towards the player.
Limits each enemy's speed according to their max speed.
Applies the velocity to each enemy's position.
If the enemy is colliding with the player, apply damage to them.

=== ```cpp systems::enemy::collision```

If the enemies get too close between each other, we apply oppossing accelerations to separate them.
This works to prevent all the enemies converting to a single blob, letting them be blobs instead.

We also limit their positions, to prevent them from straying too far from the viewing area.

=== ```cpp systems::enemy::death```
Checks each enemy's health to see if it has gone below 0.
If so, we remove the enemy from the game and spawn a coin drop.
]

#aldo[
=== ```cpp systems::message```
Yo don't look at my fucking part aye. Stay in your business bruv.
=== ```cpp systems::store```
=== ```cpp systems::music```
yoyoyo
=== ```cpp systems::hacks```
yoyoyo
=== Menu
yoyoyo
=== Graficos
yoyoyo
=== Audio
yoyoyo
]

#sebas[
== Challenges

_(copiado del doc):_
- *Player’s Experience:* One of our greatest challenges was to ensure the game was actually fun to play as, during early development, the game felt like it became repetitive too quickly. We were able to overcome this challenge later on with the addition of new weapons, new enemies, the wave system and sound effects. 
- *Enemies AI:* During the first stage of development, the enemies would instantly head straight towards the player, as the acceleration system didn’t exist at this moment, this problem made the game very difficult to the point of frustration as it was impossible to dodge the enemies, and even after the early acceleration implementation, making the enemies feel like they were actually moving instead of looking like an atomic system proved to be a challenge. Another related problem was the enemy cluttering because they had no collision with each other and we didn’t know how to implement said feature, the solution our team found to this specific problem was to make the enemies receive a small acceleration away from each other when they were close together, which may sound as a unconventional solution but it actually works very well.
- *Fixing Compilers & OS Incompatibilities:* As the game is meant to be able to run correctly on Mac and on Windows, especially because one of our members uses a Mac, we encountered many incompatibilities problems along the development that had to be fixed in order to correctly test the game.
- *Engine Architecture:* All the members worked from their own personal devices in different documents, so even if we tried to moderate it by working closely together, in the end it was impossible to prevent disorganization in our engine architecture, so that ended up being a troublesome problem we had to work with.

]

#aldo[
== Future plans
Yo this is the best m'fuckin part.
]

#leti[
== Conclusion

_(copiado del doc):_ Our team concludes that this whole game greatly helped us in our understanding of Object-Oriented Programming as well as many other areas as we were able to apply these in a much larger program, allowing us at the same time to demonstrate the knowledge and capabilities acquired during this and last year.
We encountered many different and varied problems throughout the realization of the game, from simpler to more complicated ones, forcing us to think out of the box in order to solve them and progress on the development. 
It is in our believes that, even in its simplicity and with its defects, the end product may be considered successful, as we managed to accomplish our main objective of making a fun game while being able to utilize our understanding of OOP concepts.
]
