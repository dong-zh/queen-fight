# Assignment 1 Justification

If you think any of the questions within the sections are not applicable, please write "N/A".

## Section 1: It runs

### How have you fulfilled this requirement?

This project completes this task and I've tested that the app builds with no errors and runs under the assignment environment provided by the course. The application was developed in Ubuntu (WSL2) using VS Code.

### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

The tutor should see a square 900 * 900 window. The game will start straight away.

### Where in the code should tutors look to see that you've fulfilled this requirement?

I've added new source files in `src` and new sprites in `res/img`.

## Section 2: Visibility & Shaders

### How have you fulfilled this requirement?

The project builds and runs successfully. The shaders are in `res/shaders`.

### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

They should see the Thrasher at the bottom of the screen and Giga-Queen at the top of the screen.

### Where in the code should tutors look to see that you've fulfilled this requirement?

The tutor should look in `res/shaders` to see `frag.frag` and `vert.vert`.

## Section 3: Creation and Use of 2D Shapes

### How have you fulfilled this requirement?

The project completes all of criterion 3.

### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

Everything on screen are rectangles with textures on top Giga-Queen, Thrasher, the city background, and game over screens are all rectangles.

During the battle, Giga-Queen and the thrasher moves around. This is done using matrix transforms. The tutor can look inside `thrasher.cpp` in the function `Thrasher::dodgeLeft()` for an example movement and speed calculations. Most states also scale the underlying rectangle to better fit the texture's aspect ratio. Once these matrices are calculated, they're passed into `Thrasher::draw()` which draws the shapes on screen.

The motions on screen use the render loop to animate the objects.

### Where in the code should tutors look to see that you've fulfilled this requirement?

`shapes.cpp` is where the rectangles are made. This is mostly from the tutorial code.

`thrasher.cpp` and `queen.cpp` has a matrix transforms too. Look in their respective `::draw()` functions.

## Section 4: Use of Textures

### How have you fulfilled this requirement?

This project completes all of criterion 4.

### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

Giga-Queen, Thrasher, and cityscape background are different rectangles with different textures applied. During battle, the textures change depending on what state the state machines are in. At game over, A rectangle fills the screen with the game over or winning message depending on whether the game was on.

### Where in the code should tutors look to see that you've fulfilled this requirement?

`thrasher.cpp` contains the state transitions and game logic. In each of the state functions, the tutor can see the corresponding textures for that particular state.

### Section 5: Subjective Mark

### How have you fulfilled this requirement?

This project is a recreation of (a part of) the Giga-Queen fight from the recently released Deltarune Chapter 2. The application's title is anagram of Deltarune, which itself is an anagram of the original Undertale. The original fight can be watched [here](https://youtu.be/EUYySQozClU?t=360) (obviously contains Deltarune Chapter 2 spoilers). The health of both Thrasher and Giga-Queen are printed in the terminal every time somebody gets hit.

### When your application is run, what should the tutor look for or do to see that you have fulfilled this requirement?

The tutor can play the game using the arrow keys. Left and right arrow to dodge, and up to punch.

If I haven't got the sound working yet, they should play [this](https://www.youtube.com/watch?v=vViBhJ_sAh8) in the background for the full experience.

### Where in the code should tutors look to see that you've fulfilled this requirement?

`queen.cpp` and `thrasher.cpp` has all the logic. They're state machines The code is a mess and goes against everything I've learnt in COMP2511. I don't know enough C++ to apply the design patterns effectively.

If the tutor really wants to understand the code, `thrasher.cpp` is better documented. This was my first time writing a state machine (in a language I'm not familiar with) so there's a lot of repeated code. I didn't really know how to apply the state pattern in C++. `thrasher.cpp` is very similar to `queen.cpp` since they're both state machines.

## Use of External Libraries

`chicken3421` and `shapes.hpp` from the tutorials

### What does the library do?

They provider helper functions for making the window and draws rectangles
### Why did you decide to use it?

It's easier and we were allowed to use code from the tutorials.

## Credits

Deltarune was developed by Toby Fox. Temmie Chang is the main artist. All of the sprites were extracted from the original game. All credit for this project's idea and art goes to Toby and his team.
