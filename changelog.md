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
info depth 1 time 1 score 100 nodes 16 nps 16000 pv f1
info depth 2 time 1 score 0 nodes 63 nps 63000 pv f1 b1
info depth 3 time 1 score 100 nodes 531 nps 531000 pv f1 b1 e1
info depth 4 time 1 score 0 nodes 1626 nps 1626000 pv f1 b1 g2 c1
info depth 5 time 1 score 150 nodes 8807 nps 8807000 pv f1 g6 f2 b1 g2
info depth 6 time 5 score -100 nodes 48543 nps 9708600 pv f1 b1 g2 c1 b6 c1e2
info depth 7 time 16 score 200 nodes 165904 nps 10369000 pv f1 f7 f2 b1 e1 f6 e1c2
info depth 8 time 48 score -100 nodes 391674 nps 8159875 pv f1 f7 b6 g7e5 f2 d6 f2f4 f7f5
info depth 9 time 154 score 300 nodes 1282743 nps 8329500 pv f1 f7 b6 b1 e1 e7 d2 b1c3 e1c2
info depth 10 time 653 score -200 nodes 4463461 nps 6835315 pv f2 f7 e1 b2 b6 e7 g2 c1 e1g3 c1e2
info depth 11 time 1360 score 350 nodes 9063896 nps 6664629 pv f2 b1 e2 c2 d3 b1d2 e3 c3 d1 c3e1 f1
info depth 12 time 3648 score -200 nodes 21853436 nps 5990525 pv f2 b1 e2 f6 d2 c1 e2c2 b2 d1 c3 f2d3 b3
info depth 13 time 7287 score 350 nodes 43168222 nps 5924004 pv f2 b2 e3 c2 d3 b2d2 e2 c3 d1 d3f1 c2e1 g2 f3
info depth 14 time 25601 score -200 nodes 134722981 nps 5262410 pv f2 b2 g3 c2 f4 g7e5 f2e4 b2d3 d4 c2e3 f3 d4f2 e4g2 e4
info depth 15 time 47023 score 400 nodes 264711184 nps 5629398 pv f2 f6 e3 e5 f4 f6e4 f3 e5g3 g2 g4 f5 e2 f1 g7g5 e5
```

```
1+0.01:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 2550 - 2296 - 154  [0.525] 5000
17.66 +/- 9.48

10+0.1:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 267 - 206 - 27  [0.561] 500
42.60 +/- 29.86
```

### 1.13

Late move pruning (LMP)

```
info depth 1 time 1 score 100 nodes 9 nps 9000 pv f1
info depth 2 time 1 score 0 nodes 49 nps 49000 pv f1 b1
info depth 3 time 1 score 100 nodes 255 nps 255000 pv f1 b1 e1
info depth 4 time 1 score 0 nodes 1035 nps 1035000 pv f1 b1 g2 c1
info depth 5 time 1 score 150 nodes 4968 nps 4968000 pv f1 g6 f2 b1 g2
info depth 6 time 4 score -100 nodes 31607 nps 7901750 pv f1 b1 g2 c1 g3 c1e2
info depth 7 time 18 score 200 nodes 123542 nps 6863444 pv f1 g6 g2 b1 a6 d2f1 b6
info depth 8 time 50 score -100 nodes 312878 nps 6257560 pv f1 g6 a6 f6 g2 f5 e1 f5f3
info depth 9 time 142 score 350 nodes 825764 nps 5815239 pv f2 b1 e3 c1 d2 c2 f2d1 b2 c3
info depth 10 time 661 score -200 nodes 3608508 nps 5459164 pv b6 f6 c5 b2 f1 c1 a6 b2d4 b6d5 e5
info depth 11 time 976 score 400 nodes 5258212 nps 5387512 pv b6 f6 c5 b2 d5 f6e4 d4 g7e5 c4 e5c6 b5
info depth 12 time 2740 score -250 nodes 13603047 nps 4964615 pv b6 b2 c5 b3 f2 c4 b6b4 c3 b5 d4 f2d3 d5
info depth 13 time 7129 score 400 nodes 34039378 nps 4774775 pv f2 b2 e2 c2 b6 d3 f2d2 b2d3 d1c3 e3 f2 d2f1 e3g2
info depth 14 time 36510 score -250 nodes 168354430 nps 4611186 pv b6 b2 b5 b3 f2 c4 b6b4 c3 c5 a4 c4a2 b5a3 b1 c2
info depth 15 time 53719 score 400 nodes 249043047 nps 4636032 pv b6 b2 c5 b3 c4 b2b4 b5 b3a5 a6 a4 b3 c6 b7 a1a3 c3
```

```
1+0.01:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 2793 - 2042 - 165  [0.575] 5000
52.58 +/- 9.56

10+0.1:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 263 - 218 - 19  [0.545] 500
31.35 +/- 30.02
```

### 1.14

Evaluation tempo

Previous attempts at 1+0.01:
```
// 100 tempo
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 2642 - 2195 - 163  [0.545] 5000
31.14 +/- 9.50

// 200 tempo
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 2938 - 1907 - 155  [0.603] 5000
72.68 +/- 9.67

// 300 tempo
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 3259 - 1588 - 153  [0.667] 5000
120.75 +/- 10.03
```

```
info depth 1 time 1 score -300 nodes 9 nps 9000 pv f1
info depth 2 time 1 score 400 nodes 34 nps 34000 pv f1 b1
info depth 3 time 1 score -300 nodes 225 nps 225000 pv f1 b1 e1
info depth 4 time 1 score 300 nodes 434 nps 434000 pv f1 b1 e1 b1d1
info depth 5 time 1 score -100 nodes 3166 nps 3166000 pv g1e3 b1 f3 b1d3 e2
info depth 6 time 1 score 100 nodes 10129 nps 10129000 pv f2 b1 e2 c1 e2c2 b2
info depth 7 time 2 score 0 nodes 19828 nps 9914000 pv f2 b1 e2 c1 d2 b1d1 e1
info depth 8 time 7 score 100 nodes 54589 nps 7798428 pv f2 b1 e2 f6 f3 c1 d2 b1d1
info depth 9 time 27 score 0 nodes 201427 nps 7460259 pv f2 b1 e2 f6 f3 c1 d2 b1d1 e1
info depth 10 time 71 score 100 nodes 491185 nps 6918098 pv f2 b1 e2 f6 f3 c1 d2 b1d1 f3e1 c1e3
info depth 11 time 246 score 0 nodes 1628233 nps 6618833 pv f2 b1 e2 f6 b6 a2 f3 f7 d2 c1 e2c2
info depth 12 time 498 score 100 nodes 3224022 nps 6473939 pv f2 b1 e2 c1 b6 f6 e3 a2 e3d1 b1d2 e1 e1
info depth 13 time 1417 score 0 nodes 8793954 nps 6206036 pv f2 b1 e1 c1 b7 a2 c6 b3 d2 b1d1 e2 c1e3 g1f3
info depth 14 time 2736 score 100 nodes 16598090 nps 6066553 pv f2 b1 e2 c2 b6 a2 d2 b1d1 f2d3 d1e3 c3 d1 g1e1 c1
info depth 15 time 6073 score 0 nodes 36691237 nps 6041698 pv f2 b2 e2 c2 d3 b2d2 f2d1 d3e1 c1 d3 c1c3 e3 d4 e2c4 e2
info depth 16 time 15756 score 100 nodes 91771591 nps 5824548 pv f2 b2 e2 c2 d1 b2d2 f2d3 a1c3 e2c1 e2 c2e3 c2 e2d4 c2e4 c2 e2
info depth 17 time 61357 score 0 nodes 358406960 nps 5841337 pv f2 b2 e3 c2 d4 d3 f2e4 c3 d2 d4f2 b1 e3c1 c3d1 e3 e2 g1e1 f3
info depth 18 time 154241 score 100 nodes 852586857 nps 5527627 pv f2 b2 e3 c2 d4 d2 c3 d3 f2e4 c4 b3 d2b4 a3 d2 g1e2 a2 a4 d4f3
```

```
1+0.01
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 3418 - 1433 - 149  [0.698] 5000
145.95 +/- 10.29

10+0.1
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 328 - 151 - 21  [0.677] 500
128.55 +/- 31.85
```

```
Score of AtaxxDotCpp-dev vs Tiktaxx-tryhard: 1623 - 3033 - 94  [0.352] 4750
-106.33 +/- 10.22
```

### 1.15

Min/max time management (TM)

```
1+0.01:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 3045 - 1832 - 123  [0.621] 5000
86.00 +/- 9.79

10+0.1:
Score of AtaxxDotCpp-dev vs AtaxxDotCpp-master: 294 - 194 - 12  [0.600] 500
70.44 +/- 30.74
```