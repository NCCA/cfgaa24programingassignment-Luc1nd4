# Fish Flocking Simulation using Reynolds Flocking Algorithm

My initial idea was to create a fish flocking sim to immitate a fish shoal as close to the real world as I could, I also wish to implement a UI that allows adjustment of different paramaters that updates the movement of the fish in real time.
This would involve:
-A flock class
-A boid class
-Importing and objs, and having them follow the fish movement and rotations
-A UI which allows for dynamic interaction of fish movement
-The fish to move in a circular motion.

### Explanation of program and algorithm:
The Craig Reynolds "Boids" flocking system algorithm models the complex flocking behavior of birds, fish or any large number of animals using simple rules applied to a group of entities called "boids".
Each boid follows these basic rules:
- Separation: Avoid crowding neighbors (local flockmates).
- Alignment: Steer towards the average heading of local flockmates.
- Cohesion: Steer towards the average position of local flockmates.
Based on the rules, you would compute the new velocity and position for each boid at each time step. The new velocity is determined by combining the current velocity with the computed velocities from the three rules.
Additional rules such as collision detection, or extra functions computing boid direction can be added for more effective visuals.
Each rule is weighted to balance the influence on the boid's movement, ensuring that the behavior looks natural. Adjustments to these weights can vary the flocking dynamics, potentially resulting in different types of collective movement patterns. The overall efficiency and performance of the system can often be optimized by using spatial data structures like octrees or grids to quickly find nearby flockmates without having to check every boid pair. Optimisation using octrees were however slightly out of scope given the time I have with this project, so I wish to implement just the main rules for this project.

### Iterations and Development
---
I used Jon Macey's Particle system initially from the template 'BlankNGL', as this already had a great framework for handling large numbers of rendered objects such as particles, it also had a good framework for being able to interact with the scene using mouse controls using QMouseEvents. 

After intitial research and planning, I decided that I needed the following objects & functions:

**Flock class:**
---
- Flock - List of boids in scene
- boid size
- A function to initialise the boids
- update function, for putting all 3 functions together and adding them to the velocity of each boid in the flock
- Some kind of function to calculate a vector to make the boids move in a circular motion.
- Separation, alignment and cohesion functions - return a vector to add to the velocity for each boid in scene
- render function - to render the objs for each boids

**Boid class**
---
- position - vector
- velocity - vector
- accelaration - vector
- Max speed of boid
- Max force of the boid

After following this tutorial on how to write these three flocking functions: https://youtu.be/mhjuuHl6qHM?si=eL_OH50BlWzTPAW2 , refactoring the NGL particle system, and adjusting the needed parameters, I managed to get this:
[Iteration1](https://github.com/NCCA/cfgaa24programingassignment-Luc1nd4/blob/main/Videos/Iteration1.mp4)
The boids were following the movement I wanted, there were just a few issues I accounted for upon first getting the movement how I wanted:
- The boids would go off screen or out of view, so I created a bounding box which reversed the direction of the boids of they were out of scope.
- They looked a bit more like flies initially, so I attempted to create the function needed to make the boids move in a more circular motion, that calculates a target vector on a sphere given its current spherical coordinates.
- I was able to utilise the previous function 'randomVectorOnSphere' to randomise the initial placement of the boids, so the initial position of the boids is different every time, so each simulation is unique.

My next step was to implement and interactive UI that dynamically changes paramaters such as "max speed":

After much frustration upon attempting to implement a UI, I figured out creating a seperate window as a QApplication would be the approach I needed for this. The problem I was having before was, I kept getting an error "cannot use QWidgets without QApplication", as the QT app was a QGuiApplication, and upon changing it to a QApplication, the program would crash instantly upon running. So I settled for dynamically creating all the widgets within the main function. This was not a good approach, as the ideal way I would have done it is to create a .ui file for the UI but only found this out later in the project so did not have enough time to implement this.
Perhaps I could have used another NGL demo/template which uses a UI within the same window and just have my flocking algorithm within that.
Regardless, I added sliders that returned integers and divided them by 100 to fit the float decimal values as QSliders doesn't support float values.
This however meant I had to create two functions (one in NGLScene.cpp and in Flock.cpp) to pass the values from the main function ->NGLScene ->Flock in order to update them in the functions in Flock.cpp, which is slightly clunky, I would have preferred the updated slider values directly from the main function to the functions in Flock.cpp.

Here was what I got in the end:
[Iteration2](https://github.com/NCCA/cfgaa24programingassignment-Luc1nd4/blob/main/Videos/Iteration2.mp4)

My next step was to import a fish model and have their position update for every boid.
I put my model in cmake-build-debug, created the obj, and loaded it in initialiseGL(), and made sure to update the position of each obj for each boid, then draw the objs in the render() function in Flock.cpp.
I managed to render the objs, they were however rainbow coloured which suggested they had no shader:

[Iteration3](https://github.com/NCCA/cfgaa24programingassignment-Luc1nd4/blob/main/Videos/Iteration3.mp4)

Now I just needed to update the fish rotations to match the resulting rotation of the velocity vector, then texture and shade the fish.

After a while of trying different solutions, I added a for loop looping through each boid, normalising the velocity and calculated right and up vectors using cross products. I then created a matrix for the rotation of the fish, then create a matrix for the model, then combine these with the other transformations (_project, _view and _mouse). This is what I got:
[Iteration4](https://github.com/NCCA/cfgaa24programingassignment-Luc1nd4/blob/main/Videos/Iteration4.mp4)

There were quite a few problems with this approach, as when combining the mouse rotaton matrix with the model matrix, it doesn't rotate the scene, it rotates the fish, and when I try to pan using the middle mouse, the fish turn into a fish tornado. Which implied that I needed another approach to combining these matricies to the mouse rotation. Without any mouse rotations however, the boids rotate fine and believably. 

My next objective is to attach textures to these fish and try fix the mouse rotation issue, or perhaps try another approach to the fish rotation so the matrices combine properly.


### Critical Evaluation:

- Optimisation, I would have liked to optimise this program so boids dont have to calculate every other boid in the scene, as this program can handle about 500 boids until the framerate drops, but due to limited time I was unable to implement this for the submission.
- It would have also been great if I added the ability to change the number of boids in the scene within the GUI, perhaps adding another slider with the number of boids and having a button to reset the scene with the new number of boids.


### Research & Annotated bibliography
---

Intended outcome:
https://www.youtube.com/watch?v=15B8qN9dre4
- I wanted the fish to all circle eachother and be able to travel at different speeds.

Initial algorithms & tutorial:
https://youtu.be/mhjuuHl6qHM?si=eL_OH50BlWzTPAW2
- Helped understand the three algorithms needed for a believable flocking system: Separation, alignment, and cohesion.
Although in javascript, this was especially useful to follow upon initial implementation of these three algorithms. I implemented these functions in my Flock class, so that it could loop through all boids in the flock to follow the intended movement in the update() function.

https://www.youtube.com/watch?v=bqtqltqcQhw
- First video I watched when trying to implement this program, helped me also understand optimisation methods.

https://github.com/DrAshBooth/Boids/blob/master/boid.cpp
- Helped me apply the vector math and logic needed in these three algorithms in c++

Fish model used:
https://www.cgtrader.com/free-3d-models/animals/fish/common-carp-fish

OBJ demo:
https://github.com/NCCA/ObjDemo/blob/main/src/NGLScene.cpp

QTSliders:
https://doc.qt.io/qt-6/qslider.html

QBoxLayout:
https://doc.qt.io/qt-6/qboxlayout.html
### Build

---

#### requirement:
`c++ compiler supporting C++17`
`Qt5 or Qt6`
`NGL (NCCA Graphics Library)`
`vcpkg`

to build use:
```
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=[vcpkg toolchain location] ..
cmake --build .