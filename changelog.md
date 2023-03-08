# Ataxx.cpp changelog

Runner used: https://github.com/kz04px/cuteataxx

Book used: https://github.com/kz04px/cuteataxx/blob/master/res/openings/ply3_fair.txt

## Version 1

### 1.0

"Search" that reports the first move. Evaluation and time management not implemented.

```
bestmove f1
```

### 1.1

Material based evaluation, brute-force search, simple timer for stopping.

```
info depth 1 time 1 score 1 nodes 16 nps 16000 pv f1
info depth 2 time 1 score 0 nodes 288 nps 288000 pv f1 b1
info depth 3 time 1 score 1 nodes 7020 nps 7020000 pv f1 b1 e1
info depth 4 time 2 score 0 nodes 169640 nps 84820000 pv f1 b1 f2 c1
info depth 5 time 59 score 1 nodes 5084928 nps 86185220 pv f1 a2 e1 a3 d1
info depth 6 time 1849 score -1 nodes 151865736 nps 82133983 pv f1 b1 g2 c1 g3 c1e1
info depth 7 time 62967 score 3 nodes 5322126040 nps 84522464 pv f2 a2 e1 f6 d1 a2a4 d1b1
```

```
1+0.01:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 5000 - 0 - 0  [1.000] 5000
inf +/- -nan
```

### 1.2

Alpha-beta search

```
info depth 1 time 1 score 1 nodes 16 nps 16000 pv f1
info depth 2 time 1 score 0 nodes 138 nps 138000 pv f1 b1
info depth 3 time 1 score 1 nodes 1540 nps 1540000 pv f1 b1 e1
info depth 4 time 1 score 0 nodes 18613 nps 18613000 pv f1 b1 f2 c1
info depth 5 time 5 score 1 nodes 377653 nps 75530600 pv f1 a2 e1 a3 d1
info depth 6 time 33 score -1 nodes 2151426 nps 65194727 pv f1 b1 g2 c1 g3 c1e1
info depth 7 time 352 score 3 nodes 24776858 nps 70388801 pv f2 a2 e1 f6 d1 a2a4 d1b1
info depth 8 time 8053 score -2 nodes 532018913 nps 66064685 pv f1 b1 e1 a2 g2 c1 e1g3 c1e1
info depth 9 time 74280 score 4 nodes 5001493190 nps 67332972 pv f2 b1 e1 c1 d1 b1d2 e2 c2 d3
```

```
1+0.01:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 4444 - 551 - 5  [0.889] 5000
361.96 +/- 15.33

10+0.01:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 444 - 56 - 0  [0.888] 500
359.68 +/- 49.12
```

### 1.3

Order moves by capture count

```
info depth 1 time 1 score 1 nodes 16 nps 16000 pv f1
info depth 2 time 1 score 0 nodes 138 nps 138000 pv f1 b1
info depth 3 time 1 score 1 nodes 1540 nps 1540000 pv f1 b1 e1
info depth 4 time 1 score 0 nodes 11662 nps 11662000 pv f1 b1 f2 c1
info depth 5 time 7 score 1 nodes 158874 nps 22696285 pv f1 a2 e1 a3 d1
info depth 6 time 34 score -1 nodes 699772 nps 20581529 pv f1 b1 g2 c1 g3 c1e1
info depth 7 time 290 score 3 nodes 5971850 nps 20592586 pv f2 a2 e1 f6 d1 a2a4 d1b1
info depth 8 time 3104 score -2 nodes 58036338 nps 18697273 pv f1 b1 e1 b2 f2 b1d1 e2 b2d2
info depth 9 time 26437 score 4 nodes 500963864 nps 18949346 pv f2 b1 e1 c2 d2 b1d1 e2 c2e3 d1d3
```

```
1+0.01:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 2826 - 2164 - 10  [0.566] 5000
46.27 +/- 9.69

10+0.1:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 853 - 646 - 1  [0.569] 1500
48.25 +/- 17.74
```

### 1.4

Unattackable stone evaluation

```
info depth 1 time 1 score 100 nodes 16 nps 16000 pv f1
info depth 2 time 1 score 0 nodes 138 nps 138000 pv f1 b1
info depth 3 time 1 score 100 nodes 1540 nps 1540000 pv f1 b1 e1
info depth 4 time 1 score 0 nodes 11662 nps 11662000 pv f1 b1 f2 c1
info depth 5 time 2 score 150 nodes 60246 nps 30123000 pv f1 a2 f2 b1 g2
info depth 6 time 25 score -100 nodes 488025 nps 19521000 pv f1 b1 g2 c1 g3 c1e1
info depth 7 time 279 score 300 nodes 5363982 nps 19225741 pv f2 a2 e1 f6 d1 a2a4 d1b1
info depth 8 time 3094 score -200 nodes 51459550 nps 16632045 pv f1 b1 e1 b2 f2 b1d1 f1d2 c1
info depth 9 time 11453 score 350 nodes 190747232 nps 16654783 pv f2 b1 e1 c2 d2 b1d1 e2 c1 d3
```

```
1+0.01:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 3419 - 1511 - 70  [0.691] 5000
139.65 +/- 10.32

10+0.1:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 327 - 168 - 5  [0.659] 500
114.45 +/- 32.01
```

### 1.5

AB beta bound fix

```
info depth 1 time 1 score 100 nodes 16 nps 16000 pv f1
info depth 2 time 1 score 0 nodes 63 nps 63000 pv f1 b1
info depth 3 time 1 score 100 nodes 531 nps 531000 pv f1 b1 e1
info depth 4 time 1 score 0 nodes 1805 nps 1805000 pv f1 b1 f2 c1
info depth 5 time 1 score 150 nodes 18676 nps 18676000 pv f1 a2 f2 b1 g2
info depth 6 time 5 score -100 nodes 89851 nps 17970200 pv f1 b1 g2 c1 g3 c1e1
info depth 7 time 28 score 300 nodes 531722 nps 18990071 pv f2 a2 e1 f6 d1 a2a4 d1b1
info depth 8 time 192 score -200 nodes 2696913 nps 14046421 pv f1 b1 e1 b2 f2 b1d1 f1d2 c1
info depth 9 time 1005 score 350 nodes 16620143 nps 16537455 pv f2 b1 e1 c2 d2 b1d1 e2 c1 d3
info depth 10 time 4513 score -200 nodes 58039718 nps 12860562 pv f2 b1 g3 c1 f4 a2 f4f6 d2 f2d1 c2
info depth 11 time 28900 score 350 nodes 439071801 nps 15192795 pv f1 b1 e1 c2 g2 b3 d1 b3d2 e2 c1 d3
info depth 12 time 164536 score -250 nodes 1884233744 nps 11451802 pv f1 b1 e1 c2 a6 b3 g2 d1 f1d2 c1 e2 d3
```

```
1+0.01:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 5505 - 4446 - 49  [0.553] 10000
36.93 +/- 6.82

10+0.1:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 1097 - 897 - 6  [0.550] 2000
34.86 +/- 15.27
```

### 1.6

Transposition table for first move ordering

```
info depth 1 time 1 score 100 nodes 16 nps 16000 pv f1
info depth 2 time 1 score 0 nodes 63 nps 63000 pv f1 b1
info depth 3 time 1 score 100 nodes 531 nps 531000 pv f1 b1 e1
info depth 4 time 1 score 0 nodes 1805 nps 1805000 pv f1 b1 f2 c1
info depth 5 time 1 score 150 nodes 18557 nps 18557000 pv f1 a2 f2 b1 g2
info depth 6 time 8 score -100 nodes 88331 nps 11041375 pv f1 b1 g2 c1 g3 c1e1
info depth 7 time 38 score 300 nodes 499136 nps 13135157 pv f2 a2 e2 f6 d1 a2a4 d1b1
info depth 8 time 196 score -200 nodes 1751058 nps 8933969 pv f2 a2 e2 f6 d1 a1c1 d2 a2c2
info depth 9 time 776 score 350 nodes 9754196 nps 12569840 pv f2 b1 e1 c2 d2 b1d1 e2 c1 d3
info depth 10 time 5559 score -200 nodes 40955565 nps 7367433 pv f2 b1 g3 c1 f4 a2 f4f6 d1 f2d2 c2
info depth 11 time 37362 score 350 nodes 420440502 nps 11253158 pv f2 b1 e1 c2 a6 f6 d2 b1d1 e2 c1 d3
```

```
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 16643 - 13136 - 221  [0.558] 30000
40.80 +/- 3.94

10+0.1:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 604 - 391 - 5  [0.607] 1000
75.15 +/- 21.98
```