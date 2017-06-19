# Video Cut& paste 报告

实现的是SIGGRAPH05的paper

## Usage

### prerequisite

> Assume that you have a unix-like or linux operating system.

You will need to have these software installed in your system path(in order for the Cmake to find the package)

1. **QT5**(for GUI)
2. **ffmpeg**
3. **opencv**
4. **G++** (or other C++ compiler)

### how to marked

run these scripts in your terminal.

```
cmake .
make all
```

or you can specify what interface you want to marked

```
make gui
```

for making a QT5 GUI client

or just

```
make cmd_test
```

to run the code without a GUI.

The executable program is located in Directory `exe/gui` and `exe/cmd_test`.

you can run `./exe/gui` to launch the program.
