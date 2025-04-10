# Specialisation Project in Physical Computing

This is a project for a Specialisation project course at MSc in Computer Science at Roskilde Universtiy.

This project is sensor controlled synth.

## First setup and flash:

- [Complete first 3 steps](https://daisy.audio/tutorials/cpp-dev-env/)

- Clone the repository with the submodules, run the shell command:

```
$ git clone --recurse-submodules https://github.com/RUC-MSc-CS-2sem-PC-2025/Specialisation-Project.git
```

- Build the libDaisy:

```
$ cd Specialisation-Project/libraries/libDaisy
$ make clean; make;
```

- Build the DaisySP:

```
$ cd ../DaisySP
$ make clean; make;
```

- Finally build the project and flash the Daisy Seed:
*Remember to put daisy to bootloader moode (holding boot and pressing reset)

```
$ cd ../SensorSynth
$ make clean; make; make program-dfu;
```

## Editing the codebase:
After completing the previous steps now the environment should be ready

** NOTE: ALL THE CODE IS INSIDE "SensorSynth" Folder **

Create a new branch:

```
$ git switch -c <branch-name>
```

Edit the code and then re-build and flash the daisy.

Re-build:
```
$ cd ../SensorSynth
$ make clean; make;
```

Flash:
```
$ make program-dfu;
```

