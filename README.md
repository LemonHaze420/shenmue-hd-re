# Shenmue HD RE

## Overview

WIP


## Prerequisites

* Visual Studio 2022
* [Shenmue I & II](https://store.steampowered.com/app/758330/)
* [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader/releases)

## Building

1. Ensure `SHEN_ROOT` is set to the root path of your Shenmue I & II installation.
2. Open the solution with Visual Studio 2022.
3. Modify `config.h` to reflect the kind of build you want to make.
4. Build the solution in `Debug` or `Release`.


## Usage

Once successfully built, `shenmue.asi` will be copied to both the `sm1` and `sm2` directories within `SHEN_ROOT` and will be injected automatically when starting either Shenmue I or Shenmue II.


# Contributors

* LemonHaze