# Commands

## Admin Mode (default)

| Command        | Description                                                     | Example   |
| -------------- | --------------------------------------------------------------- | --------- |
| `exit`         | Exits simuino.                                                  | `exit`    |
| `help`         | Shows command list in message window.                           | `help`    |
| `list`         | Lists available sketches. Selected sketch is marked with a `>`. | `list`    |
| `load`         | Loads and compiles the currently selected sketch.               | `load`    |
| `run`          | Enters Run Mode.                                                | `run`     |
| `win <mode>`   | Sets window layout. Mode: 0 - 4                                 | `win 3`   |
| `info <topic>` | Parameters:<br>`scen` shows scenario data<br>`loop` shows loop information<br>`error` shows servuino errors<br>`g++` shows compilation errors | `info scen`    |

## Run Mode

### General

| Key | Description                                  |
| --- | -------------------------------------------- |
| h   | Shows this information.                      |
| q   | Quits Run Mode. Switches back to Admin Mode. |
| w   | Toggles window layout mode.                  |

### Step/Go-to

| Key | Description                         |
| --- | ----------------------------------- |
| f   | Step forward                        |
| b   | Step backward                       |
| r   | Loop forward                        |
| o   | Loop reset                          |
| p   | Loop backward                       |
| a   | Go to first step                    |
| z   | Go to last step                     |
| t   | Step to next analogRead/digitalRead |

### Animation (real-time delay)

| Key | Description           |
| --- | --------------------- |
| R   | Loop forward          |
| P   | Loop backward         |
| G   | Step through scenario |

### Scenario Breakpoints

| Key | Description                                 |
| --- | ------------------------------------------- |
| y   | Shows view of Scenario Breakpoints          |
| x   | Shows expanded view of Scenario Breakpoints |
| v   | Sets value analogRead/digitalRead.<br>Current step must be one step before Read.<br>Use `t` to step between Read events. |

### Information

| Key | Description                   |
| --- | ----------------------------- |
| c   | Shows current configuration   |
| d   | Shows scenario delay overview |
| I   | Shows loop information        |
| s   | Shows sketch source code      |
