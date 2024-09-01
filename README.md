# Computer Graphics
This is a repo, which aims to centralize all projects realized in the Computer Graphics course at Computer Science degree.

---

## Project 1
Inside the "Projeto1" folder, the project was focused on reading real world elements(points, lines and polygons) coordinates from a file or a user input, transform then into viewport coordinates and display it in the screen created via [PySide framework](https://wiki.qt.io/Qt_for_Python).

###  Prerequisites
Any enviroment capable of interpreting ".ipynb" files, python 3.10 or greater and [PySyde6](https://doc.qt.io/qtforpython-6/quickstart.html).

### Executing
To run just execute the ".ipynb" file after installing the dependencies.

---

## Project 2
Inside the "Projeto2" folder, the idea was to continue the work from the first project, this time, adding moving, rotating, clipping, reseting and scaling features in the elements displayed in the viewport.

### Prerequisites
Any enviroment capable of interpreting ".ipynb" files, python 3.10 or greater and [PySyde6](https://doc.qt.io/qtforpython-6/quickstart.html).

### Executing
To run just execute the ".ipynb" file after installing the dependencies.

---

## Project 3
Inside the "Projeto3" folder, the main idea of the project was to implement a copy from [Pong](https://en.wikipedia.org/wiki/Pong), using [OpenGL](https://en.wikipedia.org/wiki/OpenGL).

### Prerequisites
Mesa OpenGL utility library version 9.0.2 or higher
OpenGl Utility Toolkit 2.8.1 or higher
Developer docs for Mesa 23.2.1 or higher

### Executing
To execute manually, just compile the `main.cpp` using the `-lglut -lGLU -lGL` flags, and run the output file.  To run it automatically, give the executable permission to `glrun` script and pass the `main.cpp` to it as argument, like: `./glrun main.cpp`.
