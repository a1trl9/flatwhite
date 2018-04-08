# flatwhite
Python3-C binding Module for Document Retrieval..

## Installation
```
python setup.py install
```

## Usage
```
from flatwhite import ged, led, ngram_distance

ged(source_str, target_str, match=0, mismatch=1, insert=1, delete=1)
led(source_str, target_str, match=1, mismatch=-1, insert=-1, delete=-1)
ngram_distance(str1, str2, index=2)
```
