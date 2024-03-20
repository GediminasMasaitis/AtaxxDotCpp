# Ataxx.cpp changelog

Runner used: https://github.com/kz04px/cuteataxx

Books used:
* https://github.com/kz04px/cuteataxx/blob/master/res/openings/ply3_fair.txt Jul 27, 2022
* https://github.com/zzzzz151/Ataxxer/blob/main/openings_8ply.txt Feb 26, 2024

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

```
1+0.01:
Score of AtaxxDotCpp-1.15.1 vs Tiktaxx-tryhard: 2153 - 2745 - 102  [0.441] 5000
-41.33 +/- 9.59
```

### 1.15.1

Added board display, node-limited search


### 1.16

Loss by no pieces detection

```
+---+---+---+---+---+---+---+
| x |   |   |   |   |   | o | 7
+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   | 6
+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   | 5
+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   | 4
+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   | 3
+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   | 2
+---+---+---+---+---+---+---+
| o |   |   |   |   |   | x | 1
+---+---+---+---+---+---+---+
  A   B   C   D   E   F   G

Turn: x
FEN: x5o/7/7/7/7/7/o5x x
Eval: 400
```

```
perft 7
time 1 depth 1 nodes 16 nps 16000
time 1 depth 2 nodes 256 nps 256000
time 1 depth 3 nodes 6460 nps 6460000
time 1 depth 4 nodes 155888 nps 155888000
time 43 depth 5 nodes 4752668 nps 110527162
time 1427 depth 6 nodes 141865520 nps 99415220
time 52246 depth 7 nodes 5023479496 nps 96150509
```

```
info depth 1 time 1 score -300 nodes 9 nps 9000 pv f1
info depth 2 time 1 score 400 nodes 34 nps 34000 pv f1 b1
info depth 3 time 1 score -300 nodes 225 nps 225000 pv f1 b1 e1
info depth 4 time 1 score 300 nodes 434 nps 434000 pv f1 b1 e1 b1d1
info depth 5 time 1 score -100 nodes 3166 nps 3166000 pv g1e3 b1 f3 b1d3 e2
info depth 6 time 1 score 100 nodes 10129 nps 10129000 pv f2 b1 e2 c1 e2c2 b2
info depth 7 time 3 score 0 nodes 19828 nps 6609333 pv f2 b1 e2 c1 d2 b1d1 e1
info depth 8 time 8 score 100 nodes 54589 nps 6823625 pv f2 b1 e2 f6 f3 c1 d2 b1d1
info depth 9 time 28 score 0 nodes 201427 nps 7193821 pv f2 b1 e2 f6 f3 c1 d2 b1d1 e1
info depth 10 time 75 score 100 nodes 491185 nps 6549133 pv f2 b1 e2 f6 f3 c1 d2 b1d1 f3e1 c1e3
info depth 11 time 252 score 0 nodes 1628242 nps 6461277 pv f2 b1 e2 f6 b6 a2 f3 f7 d2 c1 e2c2
info depth 12 time 512 score 100 nodes 3223727 nps 6296341 pv f2 b1 e2 c1 b6 f6 e3 a2 e3d1 b1d2 e1 e1
info depth 13 time 1450 score 0 nodes 9025918 nps 6224771 pv f2 b1 e1 c1 b7 a2 c6 b3 d2 b1d1 e2 c1e3 g1f3
info depth 14 time 2847 score 100 nodes 16905670 nps 5938064 pv f2 b1 e2 c2 b6 a2 d2 b1d1 f2d3 d1e3 c3 d1 g1e1 c1
info depth 15 time 9554 score 0 nodes 56404657 nps 5903774 pv f2 b2 e2 c2 d3 b2d2 e3 c3 d1 d3e1 e3f1 d3 e3 d1f3 d2b2
info depth 16 time 19943 score 100 nodes 113292568 nps 5680818 pv f2 b2 e2 c2 d1 b2d2 f2d3 a1c3 e2c1 e2 c2e3 c2 e2d4 c2e4 c2 e2
```

```
1+0.01:
Score of AtaxxDotCpp-1.16 vs AtaxxDotCpp-1.15.1: 4848 - 4935 - 217  [0.496] 10000
-3.02 +/- 6.73
```

### 1.16.1

Add extended display, add ability to make moves in-place (not used yet)

```
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   x   |       |       |       |       |       |   o   | 7
| -100  |       |       |       |       |       |  100  |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 6
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 5
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 4
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 3
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 2
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   o   |       |       |       |       |       |   x   | 1
|  100  |       |       |       |       |       | -100  |
+-------+-------+-------+-------+-------+-------+-------+
    A       B       C       D       E       F       G

Turn: x
FEN: x5o/7/7/7/7/7/o5x x
White: 0x0040000000000001
Black: 0x0001000000000040
Eval: 400
```

### 1.17

Order moves by total material gain

```
info depth 1 time 1 score -300 nodes 9 nps 9000 pv f1
info depth 2 time 1 score 400 nodes 34 nps 34000 pv f1 b1
info depth 3 time 1 score -300 nodes 225 nps 225000 pv f1 b1 e1
info depth 4 time 1 score 200 nodes 481 nps 481000 pv f1 a1c1 e2 c1e1
info depth 5 time 1 score -100 nodes 2414 nps 2414000 pv f2 b1 f2e4 c1 e4c2
info depth 6 time 1 score 200 nodes 10170 nps 10170000 pv b6 b1 b5 b1d1 f1 d1f2
info depth 7 time 3 score 0 nodes 23638 nps 7879333 pv b6 b1 b5 b1d1 b5d5 f6 d5f7
info depth 8 time 9 score 100 nodes 66058 nps 7339777 pv b6 b1 b5 b2 c4 b3 b5c3 c2
info depth 9 time 27 score 0 nodes 192762 nps 7139333 pv b6 b1 b5 b2 c4 b3 b5c3 a1c2 c4a2
info depth 10 time 91 score 100 nodes 587396 nps 6454901 pv b6 b1 b5 a2 a6 c2 f1 f6 e1 c2e2
info depth 11 time 389 score 0 nodes 2420594 nps 6222606 pv b6 b1 b5 a2 a6 c2 f1 d1 e1 c2e2 f2
info depth 12 time 1217 score 100 nodes 7163414 nps 5886124 pv b6 b2 f1 c1 g2 b1 b7 f6 c5 d1 e1 c1e2
info depth 13 time 2861 score 0 nodes 16346976 nps 5713728 pv b6 b2 f1 b3 e1 g6 c6 a2 e2 b2d1 f1d2 c2 e1c3
info depth 14 time 8493 score 100 nodes 46374127 nps 5460276 pv b6 b2 c6 c2 d6 a3 d6f7 d1 e6 d1f2 e5 d1 e5c3 b3
info depth 15 time 25177 score 0 nodes 135696524 nps 5389701 pv b6 b2 c5 c1 d6 d2 e6 e1 f6 f1 c6 c3 b4 b2c4 b5
```

```
1+0.01:
Score of AtaxxDotCpp-1.17 vs AtaxxDotCpp-1.16.1: 2704 - 2175 - 121  [0.553] 5000
36.90 +/- 9.55
```

```
60+0.6:
Score of AtaxxDotCpp-1.17 vs AtaxxDotCpp-1.16.1: 1746 - 1165 - 89  [0.597] 3000
68.15 +/- 12.46
```

### 2.0

Basic NN-based evaluation, 98->1 trained on 3M positions

```
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   x   |       |       |       |       |       |   o   | 7
| -216  |       |       |       |       |       |  219  |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 6
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 5
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 4
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 3
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 2
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   o   |       |       |       |       |       |   x   | 1
|  231  |       |       |       |       |       | -232  |
+-------+-------+-------+-------+-------+-------+-------+
    A       B       C       D       E       F       G

Turn: x
FEN: x5o/7/7/7/7/7/o5x x

White: 0x0040000000000001
Black: 0x0001000000000040
Wall:  0x0000000000000000
Empty: 0x003e7f7f7f7f7f3e
Play:  0x007f7f7f7f7f7f7f

Eval: -3
```

```
info depth 1 time 1 score 183 nodes 9 nps 9000 pv b7
info depth 2 time 1 score 2 nodes 53 nps 53000 pv b7 g6
info depth 3 time 1 score 241 nodes 212 nps 212000 pv b7 g6 c7
info depth 4 time 1 score -18 nodes 1942 nps 1942000 pv a6 a2 b7 a3
info depth 5 time 1 score 244 nodes 5782 nps 5782000 pv b7 b1 f1 g6 e1
info depth 6 time 6 score -54 nodes 24075 nps 4012500 pv f2 f7 a6 g6 e1 e7
info depth 7 time 23 score 394 nodes 87049 nps 3784739 pv g2 f6 g3 f6d5 g4 d5b7 g4g6
info depth 8 time 139 score -152 nodes 502835 nps 3617517 pv b6 b1 c5 c1 c4 b1b3 b4 a2
info depth 9 time 373 score 434 nodes 1293412 nps 3467592 pv f2 g6 b7 a2 c7 a3 d7 g6e6 d6
info depth 10 time 1301 score -150 nodes 4286902 nps 3295082 pv b6 b1 c7 f7 d6 e7 c7e6 f6 d7 e5
info depth 11 time 2841 score 447 nodes 9156460 nps 3222970 pv b6 g6 c5 f5 d6 g6g4 c7 g4g2 d6f6 f1 e7
info depth 12 time 11922 score -201 nodes 36313206 nps 3045898 pv b6 b2 c7 f7 d6 e7 c7e6 f6 d7 e5 e7g6 e7
info depth 13 time 41941 score 501 nodes 126131284 nps 3007350 pv b6 b2 c7 c1 f2 d2 e3 c1e2 g1e1 e3d1 f1 e3 e1c1
```

```
1+0.01:
Score of AtaxxDotCpp-2.0 vs AtaxxDotCpp-1.17: 695 - 4242 - 63  [0.145] 5000
-307.82 +/- 13.49

10+0.1:
Score of AtaxxDotCpp-2.0 vs AtaxxDotCpp-1.17: 190 - 802 - 8  [0.194] 1000
-247.41 +/- 27.13

1+0.01:
Score of AtaxxDotCpp-2.0-classical vs AtaxxDotCpp-1.17: 956 - 995 - 49  [0.490] 2000
-6.78 +/- 15.03
```

### 2.1

Relative (us/them instead of white/black) NN-based evaluation, 98->1 trained on 3M positions

```
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   x   |       |       |       |       |       |   o   | 7
| -140  |       |       |       |       |       |  133  |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 6
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 5
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 4
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 3
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 2
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   o   |       |       |       |       |       |   x   | 1
|  146  |       |       |       |       |       | -151  |
+-------+-------+-------+-------+-------+-------+-------+
    A       B       C       D       E       F       G

Turn: x
FEN: x5o/7/7/7/7/7/o5x x

White: 0x0040000000000001
Black: 0x0001000000000040
Wall:  0x0000000000000000
Empty: 0x003e7f7f7f7f7f3e
Play:  0x007f7f7f7f7f7f7f

Eval: -213
```

```
info depth 1 time 1 score -81 nodes 9 nps 9000 pv b7
info depth 2 time 1 score 230 nodes 34 nps 34000 pv b7 g6
info depth 3 time 1 score -71 nodes 225 nps 225000 pv b7 g6 c7
info depth 4 time 1 score 205 nodes 942 nps 942000 pv g2 f7 f1 e7
info depth 5 time 1 score -55 nodes 4702 nps 4702000 pv g2 a2 g3 b1 b7
info depth 6 time 3 score 155 nodes 15332 nps 5110666 pv g2 g6 f1 b1 g3e1 g5f3
info depth 7 time 11 score 55 nodes 45867 nps 4169727 pv g2 b1 g3 c1 g4 a2 g4g6
info depth 8 time 95 score 44 nodes 348128 nps 3664505 pv b7 f6 f1 e7 e1 d7 b7d6 e6
info depth 9 time 290 score 132 nodes 1018096 nps 3510675 pv g2 b2 a6 f7 a5 c1 a4 b2b4 b5
info depth 10 time 661 score 50 nodes 2310836 nps 3495969 pv g2 g6 g3 f7 g4 g5 g3f5 f6 f4 e5
info depth 11 time 2295 score 128 nodes 7961570 nps 3469093 pv b7 b1 f2 g6 c7 f7 g2 a2 d7 f7d6 c6
info depth 12 time 6703 score 51 nodes 22749342 nps 3393904 pv b7 a2 f1 b3 e2 a4 d2 b3d3 f1e3 a2c2 e3c3 a4a6
info depth 13 time 20972 score 136 nodes 70246613 nps 3349542 pv b7 f6 c6 e5 f2 d6 b7d5 e6 c5 d4 c6e4 g7f5 d4f4
```

```
1+0.01:
Score of AtaxxDotCpp-2.1 vs AtaxxDotCpp-2.0: 4565 - 394 - 41  [0.917] 5000
417.54 +/- 17.25

10+0.1:
Score of AtaxxDotCpp-2.1 vs AtaxxDotCpp-1.17: 871 - 598 - 31  [0.591] 1500
63.95 +/- 17.68

1+0.01:
Score of AtaxxDotCpp-2.1 vs AtaxxDotCpp-1.17: 2670 - 2245 - 85  [0.543] 5000
29.60 +/- 9.57
```

### 2.2

NN weights integer quantization

```
info depth 1 time 1 score -81 nodes 9 nps 9000 pv b7
info depth 2 time 1 score 230 nodes 34 nps 34000 pv b7 g6
info depth 3 time 1 score -71 nodes 225 nps 225000 pv b7 g6 c7
info depth 4 time 1 score 205 nodes 942 nps 942000 pv g2 f7 f1 e7
info depth 5 time 1 score -55 nodes 4702 nps 4702000 pv g2 a2 g3 b1 b7
info depth 6 time 2 score 155 nodes 14216 nps 7108000 pv g2 g6 f1 b1 g3e1 g5f3
info depth 7 time 9 score 55 nodes 44420 nps 4935555 pv g2 b1 g3 c1 g4 a2 g4g6
info depth 8 time 52 score 40 nodes 254367 nps 4891673 pv f2 g6 e3 g5 f4 f5 e3g4 f6
info depth 9 time 153 score 150 nodes 721611 nps 4716411 pv f1 b1 b7 g6 e1 g5 d1 b1d2 e2
info depth 10 time 327 score 49 nodes 1493104 nps 4566067 pv f1 b1 e1 g6 d1 c1 e1c2 b2 d2 c3
info depth 11 time 1038 score 129 nodes 4669904 nps 4498944 pv f2 g6 g3 b1 g3 a3 g2 a4 f4 a4a6 f4f6
info depth 12 time 3788 score 26 nodes 16751008 nps 4422124 pv f2 g6 e1 b2 g3 f5 b7 b2d2 f2d1 f4 d2f3 g4
info depth 13 time 24439 score 121 nodes 104440858 nps 4273532 pv a6 f6 b7 e5 c7 d6 c6 e5d7 b6 d6b5 d6 a6c5 a6
```

```
1+0.01:
Score of AtaxxDotCpp-2.2 vs AtaxxDotCpp-2.1: 2892 - 2000 - 108  [0.589] 5000
62.65 +/- 9.67

1+0.01
Score of AtaxxDotCpp-2.2 vs Tiktaxx-tryhard: 2627 - 2255 - 118  [0.537] 5000
25.90 +/- 9.53
```

### 2.3

Relative NN evaluaion architecture 98->16->1 trained on 10M positions

```
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   x   |       |       |       |       |       |   o   | 7
|  -84  |       |       |       |       |       |  -10  |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 6
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 5
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 4
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 3
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 2
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   o   |       |       |       |       |       |   x   | 1
|  -7   |       |       |       |       |       |  -80  |
+-------+-------+-------+-------+-------+-------+-------+
    A       B       C       D       E       F       G

Turn: x
FEN: x5o/7/7/7/7/7/o5x x

White: 0x0040000000000001
Black: 0x0001000000000040
Wall:  0x0000000000000000
Empty: 0x003e7f7f7f7f7f3e
Play:  0x007f7f7f7f7f7f7f

Eval: -95
```

```
info depth 1 time 1 score 21 nodes 9 nps 9000 pv b6
info depth 2 time 1 score 90 nodes 75 nps 75000 pv b7 b2
info depth 3 time 1 score -14 nodes 313 nps 313000 pv b7 b1 b6
info depth 4 time 1 score 85 nodes 1177 nps 1177000 pv b7 b2 f2 c3
info depth 5 time 1 score 3 nodes 4039 nps 4039000 pv b7 b2 c7 a2 d7
info depth 6 time 9 score 68 nodes 40115 nps 4457222 pv a6 g7e5 b7 e5g4 g1f3 a1c3
info depth 7 time 28 score 9 nodes 123865 nps 4423750 pv b7 b2 a6 a2 c7 a3 d7
info depth 8 time 103 score 57 nodes 421517 nps 4092398 pv f2 f7 e3 b2 e3c3 a1b3 g2 a2
info depth 9 time 503 score 39 nodes 1952540 nps 3881789 pv b7 b2 c6 a3 d7 a4 b5 b4 a7a5
info depth 10 time 1815 score 34 nodes 6664406 nps 3671849 pv b6 f6 f2 b2 c7 d5 c7e5 g7f5 d5e7 e5f7
info depth 11 time 4072 score 44 nodes 14542637 nps 3571374 pv b6 b1 c5 a2 d6 b1d1 c7 d1f1 e7 b4 f6
info depth 12 time 16341 score 28 nodes 55719029 nps 3409768 pv b6 f6 c5 e6 c5e5 g7f5 b6d6 e6 b5d7 d5 c6 e5c7
info depth 13 time 98777 score 19 nodes 324475474 nps 3284929 pv b6 b2 c5 c3 c7 d4 b6c4 b2b4 d4b5 b3 b6 c4a6 b7
```

```
1+0.01:
Score of AtaxxDotCpp-2.3 vs AtaxxDotCpp-2.2: 2448 - 2427 - 125  [0.502] 5000
1.46 +/- 9.50

10+0.1:
Score of AtaxxDotCpp-2.3 vs AtaxxDotCpp-2.2: 1193 - 735 - 72  [0.614] 2000
81.00 +/- 15.33
```

### 2.4

NN efficient updates (NNUE)

```
info depth 1 time 1 score 21 nodes 9 nps 9000 pv b6
info depth 2 time 1 score 90 nodes 75 nps 75000 pv b7 b2
info depth 3 time 1 score -14 nodes 313 nps 313000 pv b7 b1 b6
info depth 4 time 1 score 85 nodes 1177 nps 1177000 pv b7 b2 f2 c3
info depth 5 time 1 score 3 nodes 4039 nps 4039000 pv b7 b2 c7 a2 d7
info depth 6 time 5 score 68 nodes 40115 nps 8023000 pv a6 g7e5 b7 e5g4 g1f3 a1c3
info depth 7 time 18 score 9 nodes 123865 nps 6881388 pv b7 b2 a6 a2 c7 a3 d7
info depth 8 time 73 score 57 nodes 421517 nps 5774205 pv f2 f7 e3 b2 e3c3 a1b3 g2 a2
info depth 9 time 382 score 39 nodes 1952540 nps 5111361 pv b7 b2 c6 a3 d7 a4 b5 b4 a7a5
info depth 10 time 1391 score 34 nodes 6664406 nps 4791089 pv b6 f6 f2 b2 c7 d5 c7e5 g7f5 d5e7 e5f7
info depth 11 time 3249 score 44 nodes 14542637 nps 4476034 pv b6 b1 c5 a2 d6 b1d1 c7 d1f1 e7 b4 f6
info depth 12 time 13483 score 28 nodes 55719029 nps 4132539 pv b6 f6 c5 e6 c5e5 g7f5 b6d6 e6 b5d7 d5 c6 e5c7
info depth 13 time 83167 score 19 nodes 324475474 nps 3901493 pv b6 b2 c5 c3 c7 d4 b6c4 b2b4 d4b5 b3 b6 c4a6 b7
```

```
1+0.01:
Score of AtaxxDotCpp-2.4 vs AtaxxDotCpp-2.3: 2459 - 2453 - 88  [0.501] 5000
0.42 +/- 9.53
```

### 2.5

98->32->1 net trained on 10M positions

```
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   x   |       |       |       |       |       |   o   | 7
|  -94  |       |       |       |       |       |  36   |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 6
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 5
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 4
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 3
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 2
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   o   |       |       |       |       |       |   x   | 1
|  36   |       |       |       |       |       |  -44  |
+-------+-------+-------+-------+-------+-------+-------+
    A       B       C       D       E       F       G

Turn: x
FEN: x5o/7/7/7/7/7/o5x x

White: 0x0040000000000001
Black: 0x0001000000000040
Wall:  0x0000000000000000
Empty: 0x003e7f7f7f7f7f3e
Play:  0x007f7f7f7f7f7f7f

Eval: -71
```

```
info depth 1 time 1 score 21 nodes 9 nps 9000 pv f2
info depth 2 time 1 score 60 nodes 64 nps 64000 pv b6 b2
info depth 3 time 1 score 23 nodes 450 nps 450000 pv g1e1 b2 d1
info depth 4 time 1 score 45 nodes 2660 nps 2660000 pv b6 b2 b5 f6
info depth 5 time 1 score 58 nodes 7411 nps 7411000 pv f2 b2 e1 a3 d1
info depth 6 time 4 score 44 nodes 30378 nps 7594500 pv b6 a2 c5 f6 f2 b2
info depth 7 time 18 score 61 nodes 122003 nps 6777944 pv f2 f6 e1 e6 g3 b2 d1
info depth 8 time 70 score 29 nodes 431816 nps 6168800 pv b6 b2 a5 c2 c5 a3 b4 b3
info depth 9 time 268 score 43 nodes 1525964 nps 5693895 pv f2 f6 e1 e6 e3 d7 f4 d7b6 f4g6
info depth 10 time 995 score 39 nodes 5370909 nps 5397898 pv b6 f6 a5 a2 c5 b1 b7 b2 d5 b2d4
info depth 11 time 4525 score 58 nodes 22849021 nps 5049507 pv f2 b2 b6 a2 e2 a3 d2 b2d3 e2 c3 e1
info depth 12 time 22586 score 26 nodes 112540248 nps 4982743 pv b6 b2 f2 a3 e3 b4 c5 a3b5 e3c4 c3 b5a3 b3
info depth 13 time 57673 score 45 nodes 284449200 nps 4932103 pv f2 f6 e3 e5 d2 f4 f3 f6e4 d3 c4 e4f6 c4e4 e2
```

```
Score of AtaxxDotCpp-2.5 vs AtaxxDotCpp-2.4: 3720 - 1192 - 88  [0.753] 5000
193.45 +/- 11.02
```

### 2.6

98->64->1 net trained on 10M positions

```
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   x   |       |       |       |       |       |   o   | 7
| -129  |       |       |       |       |       |  81   |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 6
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 5
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 4
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 3
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 2
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   o   |       |       |       |       |       |   x   | 1
|   3   |       |       |       |       |       | -138  |
+-------+-------+-------+-------+-------+-------+-------+
    A       B       C       D       E       F       G

Turn: x
FEN: x5o/7/7/7/7/7/o5x x

White: 0x0040000000000001
Black: 0x0001000000000040
Wall:  0x0000000000000000
Empty: 0x003e7f7f7f7f7f3e
Play:  0x007f7f7f7f7f7f7f

Eval: -116
```

```
info depth 1 time 1 score -17 nodes 9 nps 9000 pv g1e3
info depth 2 time 1 score 102 nodes 57 nps 57000 pv f2 b2
info depth 3 time 1 score 3 nodes 557 nps 557000 pv g1e3 g6 d2
info depth 4 time 1 score 105 nodes 2507 nps 2507000 pv b6 a1a3 f2 a4
info depth 5 time 1 score 3 nodes 8034 nps 8034000 pv g1e3 b2 f4 a3 f2
info depth 6 time 4 score 71 nodes 27681 nps 6920250 pv b6 f6 b5 e7 b6d6 f6d7
info depth 7 time 20 score 25 nodes 127061 nps 6353050 pv f2 b1 e1 f6 d2 b1d1 g1e2
info depth 8 time 57 score 16 nodes 325750 nps 5714912 pv f2 b2 b6 f6 c5 e5 c5b3 a2
info depth 9 time 131 score 46 nodes 709054 nps 5412625 pv f2 b2 g3 a3 f4 a4 a7b5 b4 f4f6
info depth 10 time 488 score 3 nodes 2364413 nps 4845108 pv f2 b2 e3 f6 b6 f5 e3g5 g6 a5 f4
info depth 11 time 1920 score 44 nodes 8965176 nps 4669362 pv f2 f6 e3 b1 f4 e5 e3f5 g7e6 e4 f6g4 e4g5
info depth 12 time 5567 score 6 nodes 24290129 nps 4363234 pv b6 f6 c5 b2 c4 c3 c5b3 a2 c3b1 a3 f2 c3
info depth 13 time 28077 score 43 nodes 116893028 nps 4163301 pv b6 f6 f2 e5 c6 d4 e3 e4 d5 f6d6 d4b2 e5f3 b6d4
info depth 14 time 91358 score -12 nodes 374272042 nps 4096762 pv b6 b2 c5 f6 c4 b2d3 d4 f6d5 c3 e4 c4e5 c4 e3 d5f4
```

```
Score of AtaxxDotCpp-2.6 vs AtaxxDotCpp-2.5: 3157 - 1741 - 102  [0.642] 5000
101.16 +/- 9.92
```

### 2.7

Tighter RFP

```
info depth 1 time 1 score -17 nodes 9 nps 9000 pv g1e3
info depth 2 time 1 score 102 nodes 52 nps 52000 pv f2 b2
info depth 3 time 1 score 3 nodes 483 nps 483000 pv g1e3 g6 d2
info depth 4 time 1 score 95 nodes 1533 nps 1533000 pv f2 f6 b6 e5
info depth 5 time 1 score 3 nodes 5646 nps 5646000 pv g1e3 b2 f4 a3 f2
info depth 6 time 3 score 70 nodes 20550 nps 6850000 pv b6 f6 f2 f6g4 f3 f6
info depth 7 time 16 score 47 nodes 110192 nps 6887000 pv g1e3 f6 f2 b1 f4 f6d4 e4
info depth 8 time 42 score 15 nodes 269797 nps 6423738 pv f2 b2 b6 f6 b5 a3 b5c3 a1c2
info depth 9 time 95 score 54 nodes 578816 nps 6092800 pv f2 f6 f3 e5 f4 f5 e4 e6 d5
info depth 10 time 273 score 9 nodes 1556880 nps 5702857 pv f2 f6 b6 e5 b5 f4 f2d4 f6d5 b6d6 e4
info depth 11 time 770 score 44 nodes 4193999 nps 5446751 pv f2 f6 e3 b1 f4 e5 e3f5 g7e6 e4 f6g4 e4g5
info depth 12 time 5025 score 5 nodes 25308314 nps 5036480 pv b6 b2 c5 c3 f2 d4 b6c4 b2d3 d5 e3 e4 c3e5
info depth 13 time 13654 score 51 nodes 66563749 nps 4875036 pv b6 b2 f2 f6 b5 c2 e3 d2 f2d3 c3 b5b3 a2 c2a3
info depth 14 time 72109 score -17 nodes 335673552 nps 4655085 pv b6 b2 c5 f6 c4 c3 c5b3 a1c2 d3 b4 d4 c5 d3b5 d3
```

```
Score of AtaxxDotCpp-2.7 vs AtaxxDotCpp-2.6: 2827 - 2070 - 103  [0.576] 5000
53.01 +/- 9.63
```

### 2.8

Retrain same 98->64->1 net on 22M of fresh data for 40 epochs, quantize by 512

```
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   x   |       |       |       |       |       |   o   | 7
| -118  |       |       |       |       |       |  88   |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 6
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 5
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 4
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 3
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 2
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   o   |       |       |       |       |       |   x   | 1
|  65   |       |       |       |       |       |  -18  |
+-------+-------+-------+-------+-------+-------+-------+
    A       B       C       D       E       F       G

Turn: x
FEN: x5o/7/7/7/7/7/o5x x

White: 0x0040000000000001
Black: 0x0001000000000040
Wall:  0x0000000000000000
Empty: 0x003e7f7f7f7f7f3e
Play:  0x007f7f7f7f7f7f7f

Eval: 6
```

```
info depth 1 time 1 score 133 nodes 9 nps 9000 pv f2
info depth 2 time 1 score 35 nodes 57 nps 57000 pv f2 f7
info depth 3 time 1 score 100 nodes 531 nps 531000 pv g1e3 a1b3 f4
info depth 4 time 1 score 71 nodes 2549 nps 2549000 pv f2 f6 e2 f7
info depth 5 time 1 score 76 nodes 11342 nps 11342000 pv f1 f6 f1g3 b2 g3e5
info depth 6 time 4 score 16 nodes 35979 nps 8994750 pv b6 f6 f1 b1a3 b6b4 a1b3
info depth 7 time 15 score 94 nodes 110699 nps 7379933 pv b6 b2 c5 b2d1 g1e1 f6 c5e6
info depth 8 time 66 score 16 nodes 442785 nps 6708863 pv a7c5 b2 d6 f6 d6f7 b2b4 f7d6 b4d5
info depth 9 time 172 score 101 nodes 1083334 nps 6298453 pv f2 b2 e3 c2 d3 b2d2 f2d1 b1 d3
info depth 10 time 342 score 29 nodes 2039928 nps 5964701 pv f2 f6 e3 e5 f4 f6e4 f2d3 d5 f4 g7f5
info depth 11 time 995 score 84 nodes 5806572 nps 5835750 pv f2 b2 e3 c1 d2 b1 f2f4 b2d1 e3c2 b2 d2b3
info depth 12 time 2743 score 6 nodes 15362075 nps 5600464 pv f2 b2 b6 f6 c6 g5 d7 f5 c6a4 b3 e6 g5e5
info depth 13 time 9459 score 90 nodes 51459327 nps 5440250 pv b6 b2 c5 c1 d6 d2 d4 d2f2 d6f6 c1c3 b6c4 b2b4 d4b3
info depth 14 time 36114 score 15 nodes 187153934 nps 5182309 pv b6 f6 c6 e5 d6 f6d5 c5 e5c7 d5f4 f6d4 c5 e4c6 e4 c4
info depth 15 time 116249 score 91 nodes 579024308 nps 4980897 pv b6 b2 c5 b3 d6 c4 b6b4 c3 b5 b3d5 b5d4 b3 d3 b5 c3a2
```

```
1+0.01:
Score of AtaxxDotCpp-2.8 vs AtaxxDotCpp-2.7: 3629 - 1264 - 107  [0.737] 5000
178.56 +/- 10.77

10+0.1:
Score of AtaxxDotCpp-2.8 vs AtaxxDotCpp-2.7: 713 - 255 - 32  [0.729] 1000
171.90 +/- 23.75
```

### 2.9

Internal iterative reductions

```
info depth 1 time 1 score 133 nodes 9 nps 9000 pv f2
info depth 2 time 1 score 35 nodes 57 nps 57000 pv f2 f7
info depth 3 time 1 score 100 nodes 531 nps 531000 pv g1e3 a1b3 f4
info depth 4 time 1 score 71 nodes 2549 nps 2549000 pv f2 f6 e2 f7
info depth 5 time 1 score 76 nodes 11342 nps 11342000 pv f1 f6 f1g3 b2 g3e5
info depth 6 time 4 score 16 nodes 35408 nps 8852000 pv b6 f6 f1 g5 f2d1 a1c1
info depth 7 time 14 score 94 nodes 101037 nps 7216928 pv b6 b2 c5 b2d1 g1e1 f6 c5e6
info depth 8 time 67 score 16 nodes 434783 nps 6489298 pv a7c5 b2 d6 f6 d6f7 b2b4 f7d6 b4d5
info depth 9 time 160 score 84 nodes 975101 nps 6094381 pv g1e3 b2 f4 c2 d3 b2d2 e4 d2d4 e5
info depth 10 time 422 score 31 nodes 2459190 nps 5827464 pv f2 b2 e3 f6 d2 c3 e3c2 a1b3 d3 b2c4
info depth 11 time 761 score 92 nodes 4337512 nps 5699752 pv b6 f6 c5 e6 f2 b2 d6 f6d5 c6 e5 d4
info depth 12 time 1393 score 60 nodes 7709640 nps 5534558 pv b6 a2 c5 f6 d6 e5 c5e6 g7e7 e5f7 d7 c6 e6c7
info depth 13 time 2583 score 73 nodes 13996287 nps 5418616 pv b6 b2 c5 b3 b4 b2c4 b5 b3a5 a6 a4 c4b2 b4a2 b6b4
info depth 14 time 4161 score 56 nodes 22078172 nps 5305977 pv b6 b2 c5 f7 d4 c3 c4 b2d3 b6d5 d3f2 e3 f7d6 c4c6
info depth 15 time 8070 score 50 nodes 41988979 nps 5203095 pv b6 b2 c5 b3 b4 b2c4 b5 c3 a4 c3a3 a7a5 c4c6 b4b2 d5c3
info depth 16 time 10443 score 50 nodes 54265585 nps 5196359 pv b6 b2 c5 e5 d6 f6d5 b6d4 d6e4 c4 e5c6 c4d6 g7e5 c5e6 c4 e5
info depth 17 time 35317 score 42 nodes 174712919 nps 4946992 pv b6 b2 f2 b3 c7 b4 e2 a5 e2c3 b6c4 c7b5 c3a4 a2 a3 b1 c3
info depth 18 time 48383 score 59 nodes 238058417 nps 4920290 pv b6 b2 c5 f6 d4 e5 c5c3 e5c5 c3e5 c5c3 f7 d4f2 d4 f2e4 c3d5 b7 c6
info depth 19 time 104301 score 42 nodes 504762966 nps 4839483 pv b6 f6 f2 g5 e1 g4 e2 f3 e1e3 g4g2 f4 g2e4 g6 f5 f7 g5e6 d3
```

```
1+0.01:
Score of AtaxxDotCpp-2.9 vs AtaxxDotCpp-2.8: 2627 - 2241 - 132  [0.539] 5000
26.88 +/- 9.52

10+0.1:
Score of AtaxxDotCpp-2.9 vs AtaxxDotCpp-2.8: 1293 - 1139 - 68  [0.531] 2500
21.43 +/- 13.45
```

## From here, tests are using openings_8ply.txt book

### 2.10

Fix PVS flag

```
info depth 1 time 1 score 133 nodes 9 nps 9000 pv f2
info depth 2 time 1 score 35 nodes 57 nps 57000 pv f2 f7
info depth 3 time 1 score 100 nodes 531 nps 531000 pv g1e3 a1b3 f4
info depth 4 time 1 score 71 nodes 2540 nps 2540000 pv f2 f6 e2 f7
info depth 5 time 1 score 76 nodes 11299 nps 11299000 pv f1 f6 f1g3 b2 g3e5
info depth 6 time 4 score 16 nodes 35288 nps 8822000 pv b6 f6 f1 g5 f2d1 a1c1
info depth 7 time 14 score 94 nodes 100543 nps 7181642 pv b6 b2 c5 b2d1 g1e1 f6 c5e6
info depth 8 time 68 score 16 nodes 440289 nps 6474838 pv a7c5 b2 d6 f6 d6f7 b2b4 f7d6 b4d5
info depth 9 time 159 score 75 nodes 974489 nps 6128861 pv g1e3 b2 f4 b2a4 d4 a4b6 f4f6 a1c1 c5
info depth 10 time 469 score 31 nodes 2739635 nps 5841439 pv f2 b2 e3 f6 d2 c3 e3c2 a1b3 d3 b2c4
info depth 11 time 725 score 73 nodes 4171571 nps 5753891 pv b6 f6 c5 b2 f2 f5 d4 f5e3 c5e5 g7e6 d4f5
info depth 12 time 1196 score 60 nodes 6708457 nps 5609077 pv b6 f6 c5 e5 a6 e5g4 d6 g4f2 d6f7 g7e6 d4f5
info depth 13 time 1832 score 61 nodes 10154834 nps 5543031 pv b6 f6 c5 f5 d4 f6d6 d5 e5 c5e6 c5 e4 c4
info depth 14 time 3467 score 60 nodes 18664128 nps 5383365 pv b6 b2 c5 c2 d6 d1 g1e2 b2d2 c5d3 d1e3 c3 e3c5 a7c6 a1c1
info depth 15 time 6991 score 51 nodes 36694638 nps 5248839 pv b6 b2 c6 f6 f2 f5 d5 g4 d5f4 g5 c6e5 g5e4 f2d4 e6 d5
info depth 16 time 17128 score 49 nodes 87543872 nps 5111155 pv b6 f6 c5 e5 b4 d6 b6d5 e6 c6 e5c4 e5 g7f5 b5 d7 d5e7 e5f7
info depth 17 time 23080 score 50 nodes 117678614 nps 5098726 pv b6 b2 c5 f6 d4 e5 c5c3 e5e3 c3e5 e3c3 g6 d4f5 f7 e5g5 f2 e6
info depth 18 time 29721 score 61 nodes 151292340 nps 5090418 pv b6 b2 c5 c3 d6 b4 d6c4 b3 b5 b3a5 b3 b5d4 b3d3 a1b3 c2 a2
info depth 19 time 50238 score 60 nodes 253179189 nps 5039595 pv b6 b2 c6 c3 d7 d4 c6e5 c3d5 d7f6 d5f5 g6 e5f7 c5 f6e4 c5e5 f7d5 e6
```

```
Old ply3_fair book, 1+0.1:
Score of AtaxxDotCpp-2.10 vs AtaxxDotCpp-2.9: 3818 - 3485 - 197  [0.522] 7500
15.44 +/- 7.76

1+0.01:
Score of AtaxxDotCpp-2.10 vs AtaxxDotCpp-2.9: 2559 - 2308 - 133  [0.525] 5000
17.46 +/- 9.50

10+0.1:
Score of AtaxxDotCpp-2.10 vs AtaxxDotCpp-2.9: 986 - 941 - 73  [0.511] 2000
7.82 +/- 14.94
```

### 2.11

Fully clear state on uainewgame

```
Score of AtaxxDotCpp-2.11 vs AtaxxDotCpp-2.10: 2427 - 2415 - 158  [0.501] 5000
0.83 +/- 9.47
```

### 2.12

History table aging

```
1+0.01:
Score of AtaxxDotCpp-2.12 vs AtaxxDotCpp-2.11: 2640 - 2249 - 111  [0.539] 5000
27.23 +/- 9.54

10+0.1:
Score of AtaxxDotCpp-2.12 vs AtaxxDotCpp-2.11: 505 - 455 - 40  [0.525] 1000
17.39 +/- 21.12
```

### 2.13

Retrain same 98->64->1 net on same data for 100 epochs, quantize by 128

```
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   x   |       |       |       |       |       |   o   | 7
|  -86  |       |       |       |       |       |  103  |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 6
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 5
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 4
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 3
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 2
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   o   |       |       |       |       |       |   x   | 1
|  121  |       |       |       |       |       |  -82  |
+-------+-------+-------+-------+-------+-------+-------+
    A       B       C       D       E       F       G

Turn: x
FEN: x5o/7/7/7/7/7/o5x x

White: 0x0040000000000001
Black: 0x0001000000000040
Wall:  0x0000000000000000
Empty: 0x003e7f7f7f7f7f3e
Play:  0x007f7f7f7f7f7f7f

Eval: 21
```

```
info depth 1 time 1 score 168 nodes 9 nps 9000 pv b6
info depth 2 time 1 score 42 nodes 67 nps 67000 pv g1e2 g6
info depth 3 time 1 score 93 nodes 432 nps 432000 pv f2 f6 b6
info depth 4 time 1 score 55 nodes 1683 nps 1683000 pv b6 b2 c6 c3
info depth 5 time 1 score 66 nodes 5967 nps 5967000 pv b6 b2 f2 f6 g3
info depth 6 time 4 score 36 nodes 29445 nps 7361250 pv f2 b2 g1e1 g6 e1c1 a1c2
info depth 7 time 17 score 105 nodes 105997 nps 6235117 pv f2 b2 g3 b2b4 f2g4 b4b6 g4g6
info depth 8 time 44 score 39 nodes 261752 nps 5948909 pv f2 b2 e3 f6 d2 c1 e3c2 a1c3
info depth 9 time 158 score 79 nodes 877687 nps 5554981 pv b6 b2 c5 c1 b4 c3 c5b3 c2 b4a2
info depth 10 time 467 score 45 nodes 2525893 nps 5408764 pv a7c5 f6 b4 e5 d4 f6d5 c4 e5e3 d3 a1c3
info depth 11 time 794 score 64 nodes 4201178 nps 5291156 pv b6 b2 c5 c3 d6 b4 b6c4 b3 d6b5 a4 c4a3
info depth 12 time 1358 score 47 nodes 6992227 nps 5148915 pv b6 b2 c5 c3 d6 b4 b6c4 b2d4 b4d5 c3e5 c6 d3
info depth 13 time 1900 score 47 nodes 9705924 nps 5108381 pv b6 b2 c5 c3 d6 b4 d6f7 b3b5 a7c6 c3d5 b5d6 d4
info depth 14 time 3005 score 47 nodes 15180002 nps 5051581 pv b6 b2 c5 c3 d6 g6f4 c5e5 f6 d6f5 g7g5 e5g4 g6 g4e6
info depth 15 time 8438 score 34 nodes 40598487 nps 4811387 pv f2 f6 b6 e5 c6 d6 b6d5 e6 c5 d4 c6c4 e5e3 e5 e4 b7
info depth 16 time 13598 score 46 nodes 64308248 nps 4729243 pv b6 f6 c5 e5 d5 f6d4 b6c4 e4 d6 e4g2 d6e4 g1e3 c5d3 d4 g5e3 c5
info depth 17 time 43765 score 40 nodes 199606991 nps 4560881 pv b6 f6 c6 b2 c5 e7 d6 e6 d7 f6d5 d7f7 d5f6 b6d7 f6g4 f6 c5c7 b7
info depth 18 time 49054 score 46 nodes 224128818 nps 4569022 pv b6 f6 c5 e5 d5 f6d4 b6c4 e4 d6 e4g2 d6e4 g1e3 c5d3 d2f1 d4f3 g1e2 e4f2
info depth 19 time 68367 score 40 nodes 312380876 nps 4569176 pv b6 b2 c5 c3 d6 c4 b6d4 b2d3 d5 g7e5 c5e4 c3e3 c4b2 e6 b2c4 e6c5 d3f4 b6
```

```
1+0.01:
Score of AtaxxDotCpp-2.13 vs AtaxxDotCpp-2.12: 2686 - 2170 - 144  [0.552] 5000
35.98 +/- 9.53
```

### 2.14

98->64x2->1 net trained on 22M positions

```
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   x   |       |       |       |       |       |   o   | 7
| -103  |       |       |       |       |       |  108  |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 6
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 5
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 4
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 3
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 2
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   o   |       |       |       |       |       |   x   | 1
|  99   |       |       |       |       |       | -116  |
+-------+-------+-------+-------+-------+-------+-------+
    A       B       C       D       E       F       G

Turn: x
FEN: x5o/7/7/7/7/7/o5x x

White: 0x0040000000000001
Black: 0x0001000000000040
Wall:  0x0000000000000000
Empty: 0x003e7f7f7f7f7f3e
Play:  0x007f7f7f7f7f7f7f

Eval: -91
```

```
info depth 1 time 1 score 59 nodes 9 nps 9000 pv f2
info depth 2 time 1 score 70 nodes 56 nps 56000 pv f2 b2
info depth 3 time 1 score 27 nodes 424 nps 424000 pv a7c6 a2 d6
info depth 4 time 1 score 70 nodes 1682 nps 1682000 pv b6 f6 b5 a2
info depth 5 time 1 score 41 nodes 6243 nps 6243000 pv b6 a2 b6d5 g7e5 a7c6
info depth 6 time 3 score 49 nodes 20886 nps 6962000 pv b6 f6 a5 a2 b7 a2
info depth 7 time 7 score 99 nodes 51703 nps 7386142 pv b6 f6 c5 b2 c4 b3 c5c3
info depth 8 time 24 score 25 nodes 145852 nps 6077166 pv b6 f6 c7 b2 d7 g5 e6 g7e7
info depth 9 time 72 score 70 nodes 423642 nps 5883916 pv b6 b2 c5 b3 d6 b3d1 c5e4 d1f1 e4g2
info depth 10 time 229 score 56 nodes 1255801 nps 5483847 pv f2 b2 e3 f6 f4 e5 e3f5 e6g4 g6 f5
info depth 11 time 457 score 49 nodes 2484660 nps 5436892 pv f2 b2 e3 c1 f2d2 c2 e3c3 c1d3 b3 d3b4 c2c4
info depth 12 time 1035 score 49 nodes 5411053 nps 5228070 pv f2 b2 e3 c1 f2d2 c2 e3c3 c1d3 b3 d3b4 c2c4
info depth 13 time 1562 score 49 nodes 8140282 nps 5211448 pv f2 b2 e2 b2 e1 a2 f3 a3 e2e4 e5 f2f4 f7
info depth 14 time 5088 score 36 nodes 25502802 nps 5012343 pv f2 b2 e3 c1 d2 c2 e3d1 b1 d1c3 a2 c3d1 b3 c1c3
info depth 15 time 10586 score 50 nodes 51765329 nps 4889980 pv b6 f6 c5 e5 d6 f6d5 c6 e6 d4 e6e4 c5e6 c5 e6c4 d3 c5e3
info depth 16 time 13014 score 50 nodes 63479008 nps 4877747 pv b6 f6 c5 e5 d6 f6d5 b6d6 e4 c4 e6 d3 e5c3 e3 c3e5 c3
info depth 17 time 29898 score 54 nodes 141054570 nps 4717859 pv b6 f6 c5 e5 d6 f6d5 c6 e6 d7 e5e7 c7 e6g4 d7f5 d5b7 c5a6
info depth 18 time 57204 score 44 nodes 264563842 nps 4624918 pv b6 f6 c5 e5 d5 f6d6 c6 g7e6 b5 f4 c6e4 e6d4 c4 e4 c6d4 e7c5 c6
info depth 19 time 65362 score 50 nodes 304919587 nps 4665089 pv b6 f6 c5 e5 d5 f6d6 c6 e6 d4 e6e4 c5e6 c5 e6c4 d3 c5e3 e4 f6d5 c4
```

```
1+0.01:
Score of AtaxxDotCpp-2.14 vs AtaxxDotCpp-2.13: 3057 - 1790 - 153  [0.627] 5000
90.00 +/- 9.78

10+0.01:
Score of AtaxxDotCpp-2.14 vs AtaxxDotCpp-2.13: 571 - 386 - 43  [0.592] 1000
65.02 +/- 21.42
```

### 2.15

Speed up NNUE slightly

```
Score of AtaxxDotCpp-2.15 vs AtaxxDotCpp-2.14: 2458 - 2412 - 130  [0.505] 5000
3.20 +/- 9.49
```

### 2.16

NNUE accumulator stack

```
info depth 1 time 1 score 59 nodes 9 nps 9000 pv f2
info depth 2 time 1 score 70 nodes 56 nps 56000 pv f2 b2
info depth 3 time 1 score 27 nodes 424 nps 424000 pv a7c6 a2 d6
info depth 4 time 1 score 70 nodes 1682 nps 1682000 pv b6 f6 b5 a2
info depth 5 time 1 score 41 nodes 6243 nps 6243000 pv b6 a2 b6d5 g7e5 a7c6
info depth 6 time 3 score 49 nodes 20886 nps 6962000 pv b6 f6 a5 a2 b7 a2
info depth 7 time 7 score 99 nodes 51703 nps 7386142 pv b6 f6 c5 b2 c4 b3 c5c3
info depth 8 time 24 score 25 nodes 145852 nps 6077166 pv b6 f6 c7 b2 d7 g5 e6 g7e7
info depth 9 time 73 score 70 nodes 423642 nps 5803315 pv b6 b2 c5 b3 d6 b3d1 c5e4 d1f1 e4g2
info depth 10 time 231 score 56 nodes 1255801 nps 5436367 pv f2 b2 e3 f6 f4 e5 e3f5 e6g4 g6 f5
info depth 11 time 465 score 49 nodes 2484660 nps 5343354 pv f2 b2 e3 c1 f2d2 c2 e3c3 c1d3 b3 d3b4 c2c4
info depth 12 time 1068 score 49 nodes 5411053 nps 5066529 pv f2 b2 e3 c1 f2d2 c2 e3c3 c1d3 b3 d3b4 c2c4
info depth 13 time 1590 score 49 nodes 8140282 nps 5119674 pv f2 b2 e2 b2 e1 a2 f3 a3 e2e4 e5 f2f4 f7
info depth 14 time 5062 score 36 nodes 25502802 nps 5038088 pv f2 b2 e3 c1 d2 c2 e3d1 b1 d1c3 a2 c3d1 b3 c1c3
info depth 15 time 10664 score 50 nodes 51765329 nps 4854213 pv b6 f6 c5 e5 d6 f6d5 c6 e6 d4 e6e4 c5e6 c5 e6c4 d3 c5e3
info depth 16 time 13023 score 50 nodes 63479008 nps 4874376 pv b6 f6 c5 e5 d6 f6d5 b6d6 e4 c4 e6 d3 e5c3 e3 c3e5 c3
info depth 17 time 29904 score 54 nodes 141054570 nps 4716913 pv b6 f6 c5 e5 d6 f6d5 c6 e6 d7 e5e7 c7 e6g4 d7f5 d5b7 c5a6
info depth 18 time 56863 score 44 nodes 264563842 nps 4652653 pv b6 f6 c5 e5 d5 f6d6 c6 g7e6 b5 f4 c6e4 e6d4 c4 e4 c6d4 e7c5 c6
info depth 19 time 65128 score 50 nodes 304919587 nps 4681850 pv b6 f6 c5 e5 d5 f6d6 c6 e6 d4 e6e4 c5e6 c5 e6c4 d3 c5e3 e4 f6d5 c4
```

```
1+0.01:
Score of AtaxxDotCpp-2.16 vs AtaxxDotCpp-2.15: 2558 - 2287 - 155  [0.527] 5000
18.85 +/- 9.48

1+0.01:
Score of AtaxxDotCpp-2.16-classical vs AtaxxDotCpp-1.17: 2682 - 2172 - 146  [0.551] 5000
35.56 +/- 9.53

10+0.1:
Score of AtaxxDotCpp-2.16 vs Tiktaxx-tryhard: 941 - 42 - 17  [0.950] 1000
509.68 +/- 47.89
```

### 2.17

98->64x2->1 net trained on the same 22M positions with position rotation

```
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   x   |       |       |       |       |       |   o   | 7
| -148  |       |       |       |       |       |  97   |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 6
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 5
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 4
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 3
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 2
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   o   |       |       |       |       |       |   x   | 1
|  107  |       |       |       |       |       | -147  |
+-------+-------+-------+-------+-------+-------+-------+
    A       B       C       D       E       F       G

Turn: x
FEN: x5o/7/7/7/7/7/o5x x

White: 0x0040000000000001
Black: 0x0001000000000040
Wall:  0x0000000000000000
Empty: 0x003e7f7f7f7f7f3e
Play:  0x007f7f7f7f7f7f7f

Eval: -191
```

```
info depth 1 time 1 score 38 nodes 9 nps 9000 pv f2
info depth 2 time 1 score 128 nodes 52 nps 52000 pv b6 f6
info depth 3 time 1 score 33 nodes 303 nps 303000 pv f1 f6 e2
info depth 4 time 1 score 149 nodes 1955 nps 1955000 pv a6 a2 a6c6 a2a4
info depth 5 time 1 score 61 nodes 7245 nps 7245000 pv b6 f6 b6c4 a2c4 a7c5
info depth 6 time 4 score 73 nodes 28079 nps 7019750 pv f2 f6 e2 e5 f2f4 f6e4
info depth 7 time 17 score 101 nodes 122567 nps 7209823 pv b6 f6 c5 f6g4 b6d4 g4f2 e3
info depth 8 time 58 score 64 nodes 380269 nps 6556362 pv a7c5 b2 d6 c1 c6 c1e3 d4 b2c4
info depth 9 time 144 score 112 nodes 881231 nps 6119659 pv b6 f6 c5 b2 c4 f6g4 g1f3 b2d3 c5c3
info depth 10 time 264 score 62 nodes 1529897 nps 5795064 pv b6 f6 c5 b2 c4 b3 c5c3 a1c2 b4 d3
info depth 11 time 490 score 79 nodes 2802878 nps 5720159 pv b6 f6 c5 e5 d4 f6d5 b6c4 e4 d3 e5e3 c4e5
info depth 12 time 1421 score 46 nodes 7798042 nps 5487714 pv b6 b2 c5 f6 c4 c1 c5e5 g7e6 b6d7 d6 d5 d7c5
info depth 13 time 2228 score 46 nodes 12145920 nps 5451490 pv b6 f6 c5 b2 c4 c1 c5c3 a1b3 b6b4 b2c4 c5 b3b5
info depth 14 time 2966 score 78 nodes 16114632 nps 5433119 pv b6 f6 c5 b2 c4 b2d3 d4 f6d5 c3 c5a6 d4c6 b7 d5c7
info depth 15 time 5141 score 59 nodes 27608200 nps 5370200 pv b6 f6 c5 e5 b4 d6 b6d5 e6 c5e7 c5 e6c4 g7e6 b4b2 d4
info depth 16 time 9819 score 73 nodes 51267664 nps 5221271 pv f2 b2 e3 c3 d4 b2d3 f2e4 c4 e3d5 c3e5 c4e3 f4 d4f6 d4 g7f5
info depth 17 time 35595 score 55 nodes 179869189 nps 5053215 pv b6 b2 c5 b3 c4 b2b4 b5 c3 a4 c4a5 a6 c4 c6 b3d1 c5b3 d1f2 b7
info depth 18 time 48497 score 55 nodes 243255431 nps 5015886 pv b6 b2 c5 e5 d5 f6d6 c6 e6 b6d7 d5c7 e7 c7d5 f6 c6 b7 d6f7 e6
info depth 19 time 55303 score 70 nodes 277450633 nps 5016918 pv b6 f6 c5 e5 d4 f6d5 b6c4 e4 d3 e5c3 e5 e3 c5b3 f2 d5f3 e3c4 e3 c3e2
info depth 20 time 95524 score 57 nodes 472838359 nps 4949943 pv f2 f6 e3 b2 e1 e6 e3f5 g7e5 e1c3 a1b3 f2d4 d5 c3e4 d6 d4b3 d5c3 d5 f6f4
```

```
1+0.01:
Score of AtaxxDotCpp-2.17 vs AtaxxDotCpp-2.16: 2763 - 2080 - 157  [0.568] 5000
47.76 +/- 9.55
```

### 2.18

98->64x2->1 net trained on the same 22M positions with both position rotation and diagonal mirroring

```
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   x   |       |       |       |       |       |   o   | 7
|  -83  |       |       |       |       |       |  121  |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 6
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 5
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 4
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 3
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 2
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   o   |       |       |       |       |       |   x   | 1
|  91   |       |       |       |       |       |  -77  |
+-------+-------+-------+-------+-------+-------+-------+
    A       B       C       D       E       F       G

Turn: x
FEN: x5o/7/7/7/7/7/o5x x

White: 0x0040000000000001
Black: 0x0001000000000040
Wall:  0x0000000000000000
Empty: 0x003e7f7f7f7f7f3e
Play:  0x007f7f7f7f7f7f7f

Eval: -85
```

```
info depth 1 time 1 score 6 nodes 9 nps 9000 pv b6
info depth 2 time 1 score 100 nodes 34 nps 34000 pv b6 f7
info depth 3 time 1 score 34 nodes 254 nps 254000 pv b6 f6 b5
info depth 4 time 1 score 102 nodes 922 nps 922000 pv b6 f6 c5 e7
info depth 5 time 1 score 29 nodes 6475 nps 6475000 pv a6 f6 b5 f6g4 c4
info depth 6 time 3 score 43 nodes 25843 nps 8614333 pv b6 f6 a5 e5 a6 d4
info depth 7 time 12 score 77 nodes 83600 nps 6966666 pv b6 f6 c5 f6g4 g1f3 b2 b5
info depth 8 time 39 score 37 nodes 230224 nps 5903179 pv b6 f6 c5 a2 d5 g5 d5f5 g6
info depth 9 time 99 score 78 nodes 574419 nps 5802212 pv b6 f6 c5 g5 d5 e5 c5e6 f5 d5f4
info depth 10 time 338 score 33 nodes 1845728 nps 5460733 pv f2 f6 e1 b2 d2 c3 e1c2 a1b3 d3 b2d1
info depth 11 time 642 score 72 nodes 3509481 nps 5466481 pv b6 f6 c5 e6 d4 d5 b6c4 e5 d6 d4c6 f7
info depth 12 time 1760 score 50 nodes 9275359 nps 5270090 pv f2 b2 e3 c3 g3 d4 f2d3 b2d2 d4c2 e2 d4 e2c4
info depth 13 time 2406 score 50 nodes 12670817 nps 5266341 pv f2 b2 e3 e5 d2 d4 f2e4 f6d5 d3 d5b6 e4f6 b6c4
info depth 14 time 10696 score 33 nodes 54459484 nps 5091574 pv f2 f6 e3 e7 f4 g7g5 e3f5 e7e5 g5e4 e6 f4d5 f5f3 d5e3 f5
info depth 15 time 18117 score 63 nodes 91505427 nps 5050804 pv b6 b2 c5 f6 d5 f6e4 d4 b2c4 e5 c5b7 f6 b7c5 e5c6 c4e5 d6
info depth 16 time 32830 score 64 nodes 161718757 nps 4925944 pv f2 f6 e3 e5 d4 f6e4 f2d3 d5 e3c4 e4c5 e4 c4e3 c4 e4c3 c5b3 a4c3
info depth 17 time 36517 score 64 nodes 179681273 nps 4920482 pv f2 f6 e3 e5 d4 e5 e3f5 g7e6 f4d6 f5d5 e7f5 d5e3 e5d3 e5 e3d5 a4c3
info depth 18 time 48275 score 53 nodes 235484176 nps 4877973 pv b6 b2 c5 f6 c4 a2 b3 a1a3 c4c2 a2c3 b4 b2c4 b5 a4 a5 b3d4 b3 d3
info depth 19 time 65940 score 55 nodes 320459690 nps 4859867 pv b6 b2 c5 a3 b4 b3 b6a4 a2 c3 a2c4 a2 c3b1 b5 c3 a3c2 a3 d2b4 b2a4
info depth 20 time 101295 score 65 nodes 485632317 nps 4794237 pv f2 b2 e3 f6 d2 c3 e3c2 a1b3 d3 c4 e3 e5c4 d4 f6e4 c4e5 d3d5 d3 f4
```

```
1+0.01:
Score of AtaxxDotCpp-2.18 vs AtaxxDotCpp-2.17: 2509 - 2355 - 136  [0.515] 5000
10.70 +/- 9.49
```

### 2.19

98->128x2->1 net trained on the same 22M positions

```
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   x   |       |       |       |       |       |   o   | 7
| -145  |       |       |       |       |       |  422  |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 6
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 5
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 4
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 3
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|       |       |       |       |       |       |       | 2
|       |       |       |       |       |       |       |
+-------+-------+-------+-------+-------+-------+-------+
|       |       |       |       |       |       |       |
|   o   |       |       |       |       |       |   x   | 1
|  432  |       |       |       |       |       | -153  |
+-------+-------+-------+-------+-------+-------+-------+
    A       B       C       D       E       F       G

Turn: x
FEN: x5o/7/7/7/7/7/o5x x

White: 0x0040000000000001
Black: 0x0001000000000040
Wall:  0x0000000000000000
Empty: 0x003e7f7f7f7f7f3e
Play:  0x007f7f7f7f7f7f7f

Eval: -1004
```

```
info depth 1 time 1 score -93 nodes 9 nps 9000 pv g2
info depth 2 time 1 score 314 nodes 34 nps 34000 pv g2 b2
info depth 3 time 1 score 108 nodes 116 nps 116000 pv f2 b1 b6
info depth 4 time 1 score 110 nodes 534 nps 534000 pv f2 b2 e3 g6
info depth 5 time 1 score 37 nodes 2801 nps 2801000 pv f2 f6 g3 b2 e1
info depth 6 time 2 score 60 nodes 14182 nps 7091000 pv f1 b2 g2 f6 e2 b3
info depth 7 time 10 score 68 nodes 59308 nps 5930800 pv f2 f6 e3 e6 b6 b2 e3e5
info depth 8 time 29 score 35 nodes 148057 nps 5105413 pv f2 f6 e3 f5 e3d1 f4 d1b2 f4g2
info depth 9 time 75 score 70 nodes 375757 nps 5010093 pv f2 f6 e3 b2 b6 e5 f4 f6e4 f3
info depth 10 time 172 score 48 nodes 795145 nps 4622936 pv f2 f6 e3 f5 d2 f4 f2e4 e5 d2b2 f4f2
info depth 11 time 439 score 48 nodes 2002144 nps 4560692 pv f2 f6 e3 f5 d2 g5 e3e5 g5e4 e6 g5
info depth 12 time 1046 score 44 nodes 4730047 nps 4522033 pv f2 f6 e3 g5 f2f4 f5 e3e5 g5e6 e4 g5 e4g4 f6e4
info depth 13 time 1848 score 62 nodes 8370199 nps 4529328 pv f2 f6 e3 g5 f4 f5 e3e5 g5e4 g6 e5f7 e3 g6e5 d4
info depth 14 time 2960 score 62 nodes 13355648 nps 4512043 pv f2 f6 e3 g5 d2 d7 d2b2 d7b6 e4 f6f4 f2e4 e5 e3f5
info depth 15 time 11251 score 37 nodes 49140450 nps 4367651 pv f2 f6 e3 b2 f3 e5 d2 a3 e3f5 g7e6 c1 a1c2 f2d1 b1
info depth 16 time 13506 score 57 nodes 58815919 nps 4354799 pv f2 f6 e3 e5 e4 f5 f4 e6 e4g6 f7 c4d6 c2 e4d2 b1c3 d5e3
info depth 17 time 21857 score 42 nodes 93889383 nps 4295620 pv f2 f6 e3 b2 d3 c3 e3c2 a1b3 d2 c1 d3d1 b2d3 d1b2 d1 e2 c2c4
info depth 18 time 29061 score 47 nodes 124870788 nps 4296851 pv f2 f6 f3 g5 f4 f6g4 g3 f5 g2 e6 e4 e5 g3e3 d4 f4f6 c6 a7c7
info depth 19 time 39338 score 53 nodes 168604837 nps 4286055 pv f2 f6 e3 e6 d4 e5 e3f5 e4 d5 f5e3 d5d3 g7f5 d4b2 d4 g1f3 f4 a7c7
info depth 20 time 73371 score 59 nodes 306840678 nps 4182043 pv f2 f6 e3 e5 e4 f6d4 f2f4 d4f3 f5 g4 f2 g4g2 e3f1 f3d5 f2d4 f4f2 f6
```

```
1+0.01:
Score of AtaxxDotCpp-2.19 vs AtaxxDotCpp-2.18: 4069 - 3191 - 240  [0.559] 7500
40.86 +/- 7.78
```