# Fish Flocking Simulation using Reynolds Flocking Algorithm

My initial idea was to create

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