>⠀  
> **WARNING**: Most of the code is written in a bad manner because of the compiler limitations
>⠀⠀  
⠀

# DOSGL
## Overview
The whole idea of the project is to recreate **3d render pipeline** on MS-DOS. Right now, it's able to render models only in wireframe mode.

![This is an image](https://i.ibb.co/9bJGmNv/Screenshot-2021-12-26-200530.png)

## How to make it work
1. If you don't have a machine that can run MS-DOS, you should install DOSBox or any other emulator.
2. Using DOSBox options file, you should modify the following line
```
cycles=auto
```
to
```
cycles=max
```
3. [Mount](https://www.dosbox.com/wiki/MOUNT) the folder you cloned the repository to. In my case it's:
```
mount c F:\DosBox
C:
```
4. Add compiler folder to *PATH* variable, using following command:
```
PATH=Z:\;C:\TC\BIN
```
5. *"cd"* to the *TEST* folder and run *TEST.EXE*

> **WARNING**: Every time you run your emulator you have to repeat steps 3, 4 and 5. To automate this, open DOSBox option file and write those commands in *autoexec* section

## How to use it
Copy *dosgl.h*, *dglm.h* and *shader.h* from INCLUDE and *DOSGL.LIB* from BIN folder into your project folder.

---
## *dosgl.h*
*dosgl.h* is an interface for communicating with the **pipeline**. All of the functions are trying to copy functionality of *modern* **OpenGL**.

If you never used OpenGL before, learn basic concepts on [LearnOpenGL](https://learnopengl.com/Getting-started/Hello-Triangle)

## *dglm.h*
This header contains basic operations with matrices and vectors. It's like glm, but much smaller.

## *shader.h*
Throught using this library you will need to create your shader. It's done by creating a class that derives from abstract class **shader**. More info is available in header itself.

---
## TEST
To get a basic idea on how to use DOSGL, investigate TEST folder that contains simple cpp file and bat file that compiles it.

## Compilation
Run COMP.BAT to compile DOSGL.
The result is in BIN folder

![something](https://s10.gifyu.com/images/2021-12-16-13-49-33.gif)
