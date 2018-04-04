# flatwhite
Python3-C binding Module for Edit Distance

## Installation
```
python setup.py install
```

## Usage
```
from flatwhite import ged, led

ged(source_str, target_str, match=0, mismatch=1, gap=1)
led(source_str, target_str, match=1, mismatch=-1, gap=-1)
```
