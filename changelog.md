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

Alpha-beta (AB) search

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

Transposition table (TT) for first move ordering

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
1+0.01:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 16643 - 13136 - 221  [0.558] 30000
40.80 +/- 3.94

10+0.1:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 604 - 391 - 5  [0.607] 1000
75.15 +/- 21.98
```

### 1.7

Principal variation search (PVS) and TT early returns

```
info depth 1 time 1 score 100 nodes 16 nps 16000 pv f1
info depth 2 time 1 score 0 nodes 63 nps 63000 pv f1 b1
info depth 3 time 1 score 100 nodes 531 nps 531000 pv f1 b1 e1
info depth 4 time 1 score 0 nodes 1624 nps 1624000 pv f1 b1 f2 c1
info depth 5 time 1 score 150 nodes 11300 nps 11300000 pv f1 a2 f2 b1 e2c1
info depth 6 time 4 score -100 nodes 47862 nps 11965500 pv f1 b1 a6 c1 g2 c1e1
info depth 7 time 23 score 300 nodes 295083 nps 12829695 pv f2 a2 e2 f6 d1 a2a4 d1b1
info depth 8 time 96 score -200 nodes 903154 nps 9407854 pv f2 a2 e2 f6 d1 a1c1 d2 a2c2
info depth 9 time 345 score 350 nodes 3888130 nps 11269942 pv f2 b1 e1 c2 d2 b1d1 e2 c1 d3
info depth 10 time 1832 score -200 nodes 14485613 nps 7906993 pv f2 b1 g3 c1 f4 b2 f4f6 d2 e2 c1e1
info depth 11 time 10776 score 350 nodes 104791337 nps 9724511 pv f2 b1 e1 c2 b6 f6 d2 b1d1 e2 c1 d3
info depth 12 time 98852 score -250 nodes 678162522 nps 6860382 pv f2 b2 e1 c2 a6 f6 a5 d1 f2d2 c1 e2 d3
```

```
1+0.01:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 2682 - 2291 - 27  [0.539] 5000
27.23 +/- 9.62

10+0.1:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 794 - 696 - 10  [0.533] 1500
22.73 +/- 17.55
```

### 1.8

Prevent storing moves in TT when out of time

```
1+0.01:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 2543 - 2435 - 22  [0.511] 5000
7.51 +/- 9.60
```

### 1.9

Reverse futility pruning (RFP)

```
info depth 1 time 1 score 100 nodes 16 nps 16000 pv f1
info depth 2 time 1 score 0 nodes 63 nps 63000 pv f1 b1
info depth 3 time 1 score 100 nodes 531 nps 531000 pv f1 b1 e1
info depth 4 time 1 score 0 nodes 1624 nps 1624000 pv f1 b1 f2 c1
info depth 5 time 1 score 150 nodes 11300 nps 11300000 pv f1 a2 f2 b1 e2c1
info depth 6 time 4 score -100 nodes 48253 nps 12063250 pv f1 b1 g2 c2 g3 c2e1
info depth 7 time 20 score 300 nodes 226185 nps 11309250 pv f2 a2 e2 f6 d2 a2a4 d2b1
info depth 8 time 89 score -200 nodes 772162 nps 8675977 pv f2 a2 e2 f6 d2 a1c1 d1 a2c2
info depth 9 time 246 score 350 nodes 2091402 nps 8501634 pv f2 b1 e1 c2 d2 b1d1 e2 c1 d3
info depth 10 time 1375 score -200 nodes 9628944 nps 7002868 pv f2 b1 g3 c1 f4 a2 f4f6 d1 f2d2 c2
info depth 11 time 6407 score 350 nodes 44637495 nps 6966988 pv f2 b1 e1 c2 a6 f6 d2 b1d1 e2 c1 d3
info depth 12 time 62129 score -250 nodes 363484334 nps 5850477 pv f2 b2 e1 c2 a6 f6 a5 d1 f2d2 c1 e2 d3
```

```
1+0.01:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 3097 - 1879 - 24  [0.622] 5000
86.37 +/- 9.89

10+0.1:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 326 - 172 - 2  [0.654] 500
110.60 +/- 32.01
```

### 1.10

Late move reduction (LMR)

```
info depth 1 time 1 score 100 nodes 16 nps 16000 pv f1
info depth 2 time 1 score 0 nodes 63 nps 63000 pv f1 b1
info depth 3 time 1 score 100 nodes 531 nps 531000 pv f1 b1 e1
info depth 4 time 1 score 0 nodes 1624 nps 1624000 pv f1 b1 f2 c1
info depth 5 time 1 score 150 nodes 8411 nps 8411000 pv f1 a2 f2 b1 e2c1
info depth 6 time 3 score -100 nodes 32818 nps 10939333 pv f1 b1 g2 c2 g3 c2e1
info depth 7 time 14 score 300 nodes 156390 nps 11170714 pv f2 a2 e2 f6 d2 a2a4 d2b1
info depth 8 time 62 score -200 nodes 529737 nps 8544145 pv f2 a2 e2 f6 d2 a1c1 d1 a2c2
info depth 9 time 172 score 300 nodes 1518319 nps 8827436 pv f2 f6 e3 a2 g2 a3 d3 f6d4 f2e4
info depth 10 time 421 score -200 nodes 3163252 nps 7513662 pv f2 f6 e3 a2 e1 b3 e4 b3d3 f2d4 e5
info depth 11 time 1294 score 350 nodes 9299773 nps 7186841 pv f2 f6 e3 f5 e4 f6f4 f3 e5 g4 e4g3 g2
info depth 12 time 3770 score -200 nodes 24435305 nps 6481513 pv f2 f6 e1 b2 d2 c3 e1c2 a1c1 c3b1 d1 c3 c1b3
info depth 13 time 30483 score 400 nodes 189516529 nps 6217121 pv b6 b1 f1 a2 e2 f6 d2 b1d3 f1e3 a2c4 c3 f6d4 d2e4
info depth 14 time 104810 score -250 nodes 598138977 nps 5706888 pv b6 f6 f2 e5 c5 d6 b6d5 e6 c6 d7 d5f7 c6e7 g6 f5
```

```
1+0.01:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 3676 - 2819 - 5  [0.566] 6500
46.08 +/- 8.51

10+0.1:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 548 - 450 - 2  [0.549] 1000
34.16 +/- 21.61
```

### 1.11

Win / draw / loss (WDL) detection

```
1+0.01:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 2878 - 2000 - 122  [0.588] 5000
61.65 +/- 9.65

10+0.1:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 298 - 184 - 18  [0.614] 500
80.63 +/- 30.73
```

### 1.12

History heuristic (HH) move ordering

```
1+0.01:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 2550 - 2296 - 154  [0.525] 5000
17.66 +/- 9.48

10+0.1:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 267 - 206 - 27  [0.561] 500
42.60 +/- 29.86
```