# BBDDelay

![CI](https://github.com/Chowdhury-DSP/BBDDelay/workflows/CI/badge.svg)
[![License](https://img.shields.io/badge/License-BSD-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

This repository contains some in-progress experiments with delay
lines made using bucket-brigade device emulations. Everything here
is still very rough and raw, so use at your own risk!

## Building

To build from scratch, you must have CMake installed.

```bash
# Clone the repository
$ git clone https://github.com/Chowdhury-DSP/BBDDelay.git
$ cd BBDDelay

# initialize and set up submodules
$ git submodule update --init --recursive

# build with CMake
$ cmake -Bbuild
$ cmake --build build --config Release
```

## References

[Colin Raffel and Julius Smith, DAFx 2010](https://colinraffel.com/publications/dafx2010practical.pdf)

[Martin Holters and Julian Parker, DAFx 2018](http://dafx2018.web.ua.pt/papers/DAFx2018_paper_12.pdf)

https://github.com/jpcima/ensemble-chorus

https://github.com/jpcima/string-machine

## License

BBDDelay is open source, and is licensed under the BSD 3-clause license.
Enjoy!

