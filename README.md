# Fish Flocking Simulation using Reynolds Flocking Algorithm

My initial idea was to create a fish flocking sim to immitate a fish shoal as close to the real world as I could, I also wish to implement a UI that allows adjustment of different paramaters that updates the movement of the fish in real time.
This would involve:
-A flock class
-A boid class
-Importing and objs, and having them follow the fish movement and rotations
-A UI which allows for dynamic interaction of fish movement
-The fish to move in a circular motion.

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
[Iteration1.mp4]https://github.com/NCCA/cfgaa24programingassignment-Luc1nd4/blob/main/Videos/Iteration1.mp4



### Research & Annotated bibliography
---

Intended outcome:
https://www.youtube.com/watch?v=15B8qN9dre4
- I wanted the fish to all circle eachother and be able to travel at different speeds.

Initial algorithms & tutorial:
https://youtu.be/mhjuuHl6qHM?si=eL_OH50BlWzTPAW2
Helped understand the three algorithms needed for a believable flocking system: Separation, alignment, and cohesion.
Although in javascript, this was especially useful to follow upon initial implementation of these three algorithms. I implemented these functions in my Flock class, so that it could loop through all boids in the flock to follow the intended movement in the update() function.
https://www.youtube.com/watch?v=bqtqltqcQhw
-First video I watched when trying to implement this program.

https://github.com/DrAshBooth/Boids/blob/master/boid.cpp
- Helped me apply the vector math and logic needed in these three algorithms in c++

Fish model used:
https://www.cgtrader.com/free-3d-models/animals/fish/common-carp-fish

OBJ demo:
https://github.com/NCCA/ObjDemo/blob/main/src/NGLScene.cpp
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