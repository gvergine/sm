# sm - state machines for the modern devop

![Build](https://github.com/gvergine/sm/workflows/Build/badge.svg)
[![Coverage Status](https://coveralls.io/repos/github/gvergine/sm/badge.svg?branch=master&kill_cache=1)](https://coveralls.io/github/gvergine/sm)

The sm project consist of
- libsm - a pure C state machine library
- smi - a state machine parser and runner for shell use-cases.

## smi

### Examples

Consider a `toggle` state machine that turns off and on a lamp at regular time intervals (timeout event) and only if it is enabled by a toggle button (toggle event):
<details>
<summary>click to expand</summary>

```
initial state disabled {
    on entry {
        action stop_timer
    }
    on toggle {
        goto enabled_light_on
    }
    on exit {
        action start_timer
    }
}
state enabled_light_on {
    on entry {
        action turn_light_on
    }
    on toggle {
        action turn_light_off
        goto disabled
    }
    on timeout {
        goto enabled_light_off
    }
}
state enabled_light_off {
    on entry {
        action turn_light_off
    }
    on toggle {
        goto disabled
    }
    on timeout {
        goto enabled_light_on
    }
}
```
</details>

#### Here is what you can do with `smi`:

Run remotely and access the state machine via tcp (requires `dpipe` or similar):
```
dpipe smi -d toggle.sm = nc -lk 3000
```

Keep running a program (in this example `sol`) with a 5 seconds timeout delay between runs:
```
dpipe smi -d keep_running.sm =  ./keep_running.sh "sol"
```

The state machine can be exposed via mqtt:
```
mosquitto_sub -t events | smi -d toggle.sm | mosquitto_pub -t commands -l
```

and mqtt clients can be used to send events

```
$ mosquitto_pub -t events -m toggle
$ mosquitto_pub -t events -m timeout
```

and receive commands

```
$ mosquitto_sub -t commands
stop_timer
start_timer
turn_light_on
turn_light_off
```

Enumerate events:
```
$ smi -d toggle.sm -E
timeout
toggle
```
Enumerate actions:
```
$ smi -d toggle.sm -A
stop_timer
turn_light_on
start_timer
turn_light_off
```
Enumerate states:
```
$ smi -d toggle.sm -S
disabled
enabled_light_on
enabled_light_off
```
Find errors:
```
$ smi -d toggle_with_errors.sm --validate-only
ERROR: No state is defined as initial.
ERROR: State `disabled` is not reachable.
ERROR: State `enabled_light_on` is not reachable.
ERROR: State `enabled_light_off` is not reachable.
$ echo $?
1
```

## Build from source

```
sudo apt install bison flex gengetopt
git clone https://github.com/gvergine/sm.git
cd sm
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make
```

## Debug build

The following instructions are useful only to who want to build a debug version
of this project (and hopefully contribute to the project):

1. Install build and test tools and clone the project:
```
sudo apt install bison flex gengetopt check subunit lcov valgrind 
git clone https://github.com/gvergine/sm.git
cd sm
```

2. Create a build directory and invoke cmake
```
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
env CTEST_OUTPUT_ON_FAILURE=1 ctest -R sm-unit-tests -T memcheck
env CTEST_OUTPUT_ON_FAILURE=1 ctest -R smi-tests # Don't use -T memcheck here!
make sm_coverage smi_coverage
```
