PA1 Auto-grader, Release 1

mexp: incorrect output
   arguments ['./mexp', '/common/home/dg1188/Downloads/pa1-full-grader/pa1/data/mexp/test.07.txt']

   reference file: '/common/home/dg1188/Downloads/pa1-full-grader/pa1/data/mexp/ref.07.txt'
   line 1
     expected: '1 0 0 0 0'
     received: '67 -1 12 14 1088'
   line 2
     expected: '0 1 0 0 0'
     received: '92 7 -8888 23 1'
   line 3
     expected: '0 0 1 0 0'
     received: '12 12 12 12 12'
   line 4
     expected: '0 0 0 1 0'
     received: '0 1 45 1999 23'
   line 5
     expected: '0 0 0 0 1'
     received: '18 9 45 2 0'

Tests performed: 70 of 70
Tests failed:    1

Regular credit
-----
           Points Failed Score
  rot13       5.0      -   5.0
  sorta       7.0      -   7.0
  sudoku:1    9.0      -   9.0
  sudoku:2    9.0      -   9.0
  list       20.0      -  20.0
  mexp       20.0      1  18.0
  bst:1      15.0      -  15.0
  bst:2      15.0      -  15.0
           ------        -----
            100.0         98.0

**________________________________________________________________________________**

PA2 Auto-grader, Release 1

estimate: incorrect output
   arguments ['./estimate', 'train', 'data']

   training file: '/common/home/dg1188/Downloads/pa2-full-grader/pa2/data/train.15.txt'
   data file:     '/common/home/dg1188/Downloads/pa2-full-grader/pa2/data/data.15.txt'
   reference file: '/common/home/dg1188/Downloads/pa2-full-grader/pa2/data/ref.15.txt'
   line 32
     expected: '18700'
     received: '18699'
   line 89
     expected: '9856'
     received: '9857'

Tests performed: 20 of 20
Tests failed:    1

Regular credit
-----
           Points Failed Score
  estimate  100.0      1  95.0

**________________________________________________________________________________**

PA4 Auto-grader, Release 1

truthtable:1: incorrect output
   arguments ['./truthtable', '/common/home/dg1188/Downloads/pa4-full-grader/pa4/data/test.1.11.txt']

   reference file: '/common/home/dg1188/Downloads/pa4-full-grader/pa4/data/ref.1.11.txt'
   line 1
     expected: '0 0 | 0 1'
     received: '0 0 | 0 -1'
   line 2
     expected: '0 1 | 1 0'
     received: '0 1 | 1 -2'
   line 3
     expected: '1 0 | 1 0'
     received: '1 0 | 1 -2'
   line 4
     expected: '1 1 | 0 1'
     received: '1 1 | 0 -1'

Tests performed: 36 of 36
Tests failed:    1

Regular credit
-----
               Points Failed Score
  truthtable:1  100.0      1  95.0

Extra credit
-----
               Points Failed Score
  truthtable:2   16.0      -  16.0

