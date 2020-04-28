# simulino

> Simuino is an Arduino UNO/MEGA Pin Simulator.

This is a fork of [bsaxen/simuino](https://github.com/bsaxen/simuino) and [bsaxen/servuino](https://github.com/bsaxen/servuino) with some minor imprevements and fixes (e.g. locale/charset issues, missing return values, ...).

Thanks to [@DoodleSchrank](https://github.com/DoodleSchrank) for helping me fix the locale/charset issue ❤

Note: Simuino currently only works on Linux (i.e. Debian or Ubuntu).

## Requirements

- g++ (`apt-get install g++`)
- git (`apt-get install git`)
- ncurses-dev or ncurses (`apt-get install ncurses-dev`)

## Installation

Install the requirements listed above.

Then, open a terminal and navigate (using `cd`) to a folder of your choice.

Clone this repository: `git clone https://github.com/irgendwr/simuino.git`

And enter the folder: `cd simuino`

Now compile the program: `g++ -o simuino simuino.c -lncurses`

And then run it: `./simuino`

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

## Links

Useful links to the original documentation:

- Get Started: http://web.simuino.com/get-started
- FAQ: http://web.simuino.com/faq
- Commands: http://web.simuino.com/commands
- Supported Functions: http://web.simuino.com/reference