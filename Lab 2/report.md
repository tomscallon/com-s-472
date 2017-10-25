# Tom Scallon
## Com S 472, Lab 2 Report

### Effects of Increasing Search Depth

See below the results of pitting agents with various search depths against one another:

<pre>
1/1 = -1
1/2 = -1
1/3 = 1
1/4 = -1
1/5 = -1
1/6 = -1
2/1 = -1
2/2 = -1
2/3 = 1
2/4 = -1
2/5 = -1
2/6 = -1
3/1 = 1
3/2 = 1
3/3 = -1
3/4 = 1
3/5 = 1
3/6 = -1
4/1 = -1
4/2 = -1
4/3 = 1
4/4 = -1
4/5 = -1
4/6 = 1
5/1 = 1
5/2 = -1
5/3 = 1
5/4 = 1
5/5 = -1
5/6 = 1
6/1 = 1
6/2 = -1
6/3 = 1
6/4 = -1
6/5 = 1
6/6 = 1</pre>

(See `int main` in `main.cpp` for the code that generated this output.)

Here, results are in the format `w/b = r` where...
  - `w` indicates the search depth for white
  - `b` indicates the search depth for black
  - `r` indicates the result (1 = white wins, -1 = black wins, 0 = tie)

Unfortunately, since the agents' decisions are deterministic (they always scan the board in the same order), we cannot run multiple tests to observe a larger-scale pattern; however, a general (and unsurprising) rule is observed: For the most part, agents with greater search depth are more likely to win.

### Effects of Changing Evaluation Function

Two evaluation functions are included in the code. The first, called "A", simply counts the number of spaces occupied by a player's color. More spaces occupied results in a linearly higher evaluation.

The second, called "B", uses the same idea with some modification. It takes into account the observations that (1) possessing corner spots is important and (2) occupying spaces adjacent to an empty corner spot is dangerous (as it may allow the opponent to take the corner).

See below the results of pitting these two different evaluation functions against one another:

<pre>
1/1 = -1
1/2 = -1
1/3 = -1
1/4 = -1
1/5 = -1
1/6 = -1
2/1 = 1
2/2 = -1
2/3 = -1
2/4 = -1
2/5 = -1
2/6 = -1
3/1 = -1
3/2 = -1
3/3 = -1
3/4 = -1
3/5 = -1
3/6 = -1
4/1 = 1
4/2 = -1
4/3 = -1
4/4 = -1
4/5 = -1
4/6 = -1
5/1 = 1
5/2 = -1
5/3 = -1
5/4 = -1
5/5 = -1
5/6 = -1
6/1 = 1
6/2 = 1
6/3 = -1
6/4 = -1
6/5 = -1
6/6 = -1</pre>

(See `int main` in `main.cpp` for the code that generated this output.)

Here, results are in the format `w/b = r` where...
  - `w` indicates the search depth for white (using eval a)
  - `b` indicates the search depth for black (using eval b)
  - `r` indicates the result (1 = white wins, -1 = black wins, 0 = tie)

It is readily apparent evaluation function "B" is superior to "A" -- B beats A in almost every match. The only cases in which B doesn't prevail are when it has significantly lower search depth.
