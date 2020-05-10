# simuino

[![Docker Pulls](https://img.shields.io/docker/pulls/irgendwr/simuino.svg)](https://hub.docker.com/r/irgendwr/simuino)

> Simuino is an Arduino UNO/MEGA Pin Simulator.

This is a fork of [bsaxen/simuino](https://github.com/bsaxen/simuino) and [bsaxen/servuino](https://github.com/bsaxen/servuino) with some minor imprevements and fixes (e.g. locale/charset issues, missing return values, ...).

Thanks to [@DoodleSchrank](https://github.com/DoodleSchrank) for helping me fix the locale issue ❤

Note: Simuino currently only works on Linux (i.e. Debian or Ubuntu) or using the [Docker Image](https://hub.docker.com/r/irgendwr/simuino).

## Installation

### Linux

First, install the following requirements:

- g++ (`apt-get install build-essential`)
- git (`apt-get install git`)
- ncurses-dev or ncurses (`apt-get install ncurses-dev`)

Then, open a terminal and navigate (using `cd`) to a folder of your choice.

Clone this repository: `git clone https://github.com/irgendwr/simuino.git`

And enter the folder: `cd simuino`

Now compile the program: `g++ -O3 -lncurses -o simuino simuino.c`

And then run it: `./simuino`

### Windows

Windows is currently not supported. Either use a Linux VM, [WSL](https://docs.microsoft.com/en-us/windows/wsl/install-win10) or [docker](https://www.docker.com/).

### Docker

The docker image can be run as shown here:

```bash
docker pull irgendwr/simuino
docker run -it --rm -v /sketchbook:/simuino/sketchbook --name simuino irgendwr/simuino
```

More details are available on the [simuino docker hub page](https://hub.docker.com/r/irgendwr/simuino).

## Usage

Run the file with `./simuino` (assuming you are in the folder in which you downloaded it)

> A typical use case is:
>
> Type `help` to get a list of available commands.
>
> 1. Type `list` to see available sketches
>
> 2. Choose sketch according to number given in list, type the number of your sketch and confirm with return (enter key).
>
> 3. Type `load` to load (copy/compile/run) the sketch
>
> 4. Type `run` to enter Run Mode. The prompter changes to R.
>
> In Run Mode, you can step through the simulation in different ways.
>
>     f - step forward
>     b - step backward
>     r - loop forward
>     p - loop backward
>     a - goto step 1
>     z - goto last step
>     t - analogRead/digitalRead stepping
>     R - animation of loop
>     s - debug window
> 
> Press `h` to see the list of available commands.
>
> You can quickly switch between window layouts by pressing `w` . There are 6 different to choose between. The digit in the prompter shows the number of the window layout (0,1,2,3,4,5).
>
> Note: Sketches should be placed in folder `<your path>/simuino/` sketchbook. Sketches should be added with comments according to Simuino configuration, in order to control simulation length, scenario and name of sketch. The extension of the sketch file is `.ino`.

Quote from the [simuino guide](http://web.simuino.com/get-started).

Note: This will mount the sketchbook folder on your local system at `/sketchbook/`.

## Links

- [Commands](docs/commands.md)
- [Supported Functions](docs/reference.md)

Useful links to the original documentation:

- Get Started: http://web.simuino.com/get-started
- FAQ: http://web.simuino.com/faq

## License

From the original repository:

> Copyright (C) 2012 Benny Saxen
>
> This program is free software: you can redistribute it and/or modify
> it under the terms of the GNU General Public License as published by
> the Free Software Foundation, either version 3 of the License, or
> (at your option) any later version.
>
> This program is distributed in the hope that it will be useful,
> but WITHOUT ANY WARRANTY; without even the implied warranty of
> MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
> GNU General Public License for more details.
>
> You should have received a copy of the GNU General Public License
> along with this program. If not, see http://www.gnu.org/licenses/

> You are welcome to redistribute it, as long as it is named Simuino.