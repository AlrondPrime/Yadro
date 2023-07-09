## Requirements
You need to manually create file `Properties.cmake` and set Cmake variables like below

```
set(BOOST_ROOT <path_to_your_Boost_library>)
set(BOOST_LIBRARYDIR <path_to_your_Boost_library>)
```

## Usage
```
Yadro.exe --input-file=../tapeIn.txt --output-file=../tapeOut.txt
```
or 
```
Yadro.exe ../tapeIn.txt ../tapeOut.txt
```
to disable tests use flag
```
--gtest_filter=-*
```

## What this program is
Program emulates magnetic tape workflow and performs Merge Sort from input tape to outup tape using 3 temporary tapes.
