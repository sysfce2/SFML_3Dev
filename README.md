# 3Dev
Simple 3D game engine, that uses SFML, OpenGL, Assimp. 
## 3Dev Editor
3Dev Editor is in development, but now, if you on Windows, you can launch exe file in 3Dev Editor folder and make your scene.
## Tutorial
Check Wiki, if you want to learn how to make games with 3Dev.
## Features
Loading many model formats (.obj, .fbx, .dae, etc.)

Loading animations

Loading textures

GUI (Buttons, Text box)

Class for lighting

Class for camera

Class for simple shapes

3Dev Editor (soon)
## TODO
- [x] Make it cross-platform
- [x] Add lights (3Dev Editor)
- [ ] Add shapes (3Dev Editor)
- [ ] Add export function (3Dev Editor)
- [x] Add physics
- [x] Improve GUI
- [x] Add text input (Gui) 
- [ ] Adding and subtracting models and animations (maybe)
- [ ] Animation states
- [ ] Materials
## Building test game (Linux)
In 3Dev folder you have model, skybox, code and building files. First you need to download assimp sources in 3Dev folder, execute
```
git clone https://github.com/assimp/assimp
```
in terminal, you don't need to build it. Then download and build https://github.com/1Kuso4ek1/ibs, write
```
sudo cp ibs /usr/bin
```
to use ibs everywhere you want, then execute
```
ibs build.ibs libraries.ibs
```
in terminal.
## Building 3Dev Editor (Linux)
It's really easy. If you already build ibs, just write
```
ibs editor.ibs libraries.ibs
```
wait, and enjoy! :)
