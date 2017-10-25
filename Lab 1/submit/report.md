# Com S 472: Lab 1
## Lab Report / Tom Scallon

### Search Strategy Performance
| Intranet | Breadth | Depth   | Best   | Beam   |
|:--------:| ------- | ------- | ------ | ------ |
| 1        | 91 / 4  | 58 / 15 | 15 / 7 | 16 / 7 |
| 5        | 88 / 8  | 42 / 10 | 35 / 8 | 35 / 8 |
| 7        | 56 / 6  | 12 / 9  | 30 / 8 | 25 / 8 |

Performance measure: nodes expanded / path length

### Search Strategy Solution Paths

#### Breadth

##### Intranet 1

<pre>
Path from start to goal (length 4):
  page1.html (start)
  page18.html
  page29.html
  page99.html
  page50.html (goal)</pre>

##### Intranet 5

<pre>
Path from start to goal (length 8):
  page1.html (start)
  page40.html
  page99.html
  page89.html
  page87.html
  page96.html
  page95.html
  page72.html
  page62.html (goal)</pre>

##### Intranet 7

<pre>
Path from start to goal (length 6):
  page1.html (start)
  page48.html
  page71.html
  page57.html
  page62.html
  page61.html
  page86.html (goal)</pre>

#### Depth

##### Intranet 1

<pre>
Path from start to goal (length 15):
  page1.html (start)
  page23.html
  page60.html
  page39.html
  page78.html
  page25.html
  page42.html
  page84.html
  page30.html
  page68.html
  page93.html
  page87.html
  page79.html
  page2.html
  page83.html
  page50.html (goal)</pre>

##### Intranet 5

<pre>
Path from start to goal (length 10):
  page1.html (start)
  page40.html
  page99.html
  page5.html
  page97.html
  page68.html
  page48.html
  page7.html
  page95.html
  page72.html
  page62.html (goal)</pre>

##### Intranet 7

<pre>
Path from start to goal (length 9):
  page1.html (start)
  page48.html
  page71.html
  page57.html
  page90.html
  page39.html
  page60.html
  page11.html
  page78.html
  page86.html (goal)</pre>

#### Best

##### Intranet 1

<pre>
Path from start to goal (length 7):
  page1.html (start)
  page14.html
  page69.html
  page87.html
  page78.html
  page88.html
  page99.html
  page50.html (goal)</pre>

##### Intranet 5

<pre>
Path from start to goal (length 8):
  page1.html (start)
  page40.html
  page99.html
  page89.html
  page87.html
  page96.html
  page95.html
  page72.html
  page62.html (goal)</pre>

##### Intranet 7

<pre>
Path from start to goal (length 8):
  page1.html (start)
  page48.html
  page8.html
  page19.html
  page89.html
  page23.html
  page73.html
  page61.html
  page86.html (goal)</pre>

#### Beam

##### Intranet 1

<pre>
Path from start to goal (length 7):
  page1.html (start)
  page14.html
  page69.html
  page87.html
  page78.html
  page88.html
  page99.html
  page50.html (goal)</pre>

##### Intranet 5

<pre>
Path from start to goal (length 8):
  page1.html (start)
  page40.html
  page99.html
  page89.html
  page87.html
  page96.html
  page95.html
  page72.html
  page62.html (goal)</pre>

##### Intranet 7

<pre>
Path from start to goal (length 8):
  page1.html (start)
  page48.html
  page8.html
  page19.html
  page89.html
  page23.html
  page73.html
  page61.html
  page86.html (goal)</pre>

### Question Responses

#### 2a.

Included in the code `WebSearch.java` you'll find three heuristics. The heuristic whose numbers are reported above in the "Best" sections is `WebSearch::h3` -- but `h3` builds upon `WebSearch::h1` and `WebSearch::h2`, so it is necessary to understand the progression to understand `h3`.

`WebSearch::h1` is an incredibly simple heuristic. It evaluates the distance of a link from the goal page as the fraction of words in the link's hypertext that are *not* query words. For example, if the query words are `A B C`, and a link's hypertext is `A D E B E A E E`, `h1` would evaulate that link's score as `0.625` (because 5/8 of the words in the link's hypertext were *not* query words).

`WebSearch::h2` is an extension of `h1`. It follows the same methodology, except it inflicts penalties upon query words as follows:

 - If a query word is not preceded by another query word, inflict an `x%` penalty on that word.
 - If a query word does not follow the last found query word in the original query string, inflict a `y%` penalty on that word. The first query word in a link's hypertext must be the first query word in the query string to avoid this penalty.

For the results above, I used `x% = y% = 40%`. To get a better understanding of how this works, consider the following strings:

Query String: `A B C`<br>
Hypertext: `E B A F B C`

The words of `Hypertext` are scored as follows:
 - `E` - 1
   - +1 (not a query word)
 - `B` - 0.8
   - +0 (query word)
   - +0.4 (not preceded by query word)
   - +0.4 (not in correct order -- expected `A`)
 - `A` - 0.4
   - +0 (query word)
   - +0 (preceded by query word)
   - +0.4 (not in correct order -- expected `C`)
 - `F` - 1
   - +1 (not a query word)
 - `B` - 0.4
   - +0 (query word)
   - +0.4 (not preceded by query word)
   - +0 (in correct order)
 - `C` - 0
   - +0 (query word)
   - +0 (preceded by query word)
   - +0 (in correct order)

In total, the above string scores `3.6 / 6 = .6`. Notice `h1` would have scored the same string `2 / 6 = 0.333...`.

Finally, we can discuss `WebSearch::h3` -- the actual heuristic used in this lab. `h3` combines two separate heuristics -- `h2` (from above) and a new heuristic, `WebSearch::pageH`. `pageH` is analagous to `h1`, except it evaluates the content of a page instead of the content of a link's hypertext; that is, `pageH` accepts the contents of a page and returns the proportion of words on that page that are *not* query words.

To obtain `h3` for a link, we evaluate `h2` on that link, and then evaluate `pageH` on the page on which that link was found. Finally, we combine these two values using a weighted average. The value `P ∈ [0,1]` determines the weight of `pageH` -- that is:

<pre>  h3 = P * pageH + (1 - P) * h3</pre>

In the code, I use `P = 0.5`. (Note, in the code, I refer to `P` as `PAGE_PROPORTION`.)

We define path cost as the length of the shortest path to the goal. Observe `h3` is not admissible: For any link `a`, `h3(a) ∈ [0,1]` -- so, in essence, `h3` never suggests a cost over 1 (a single hop). For any non-goal state, the heuristic supplies an admissible response -- the minimum true cost from any non goal state is at least 1, so `h3` will never overestimate. However, consider a goal state: it is very possible a page leading to a goal page will contain non-query words, or that a link leading to a goal page will have non-query words in its hypertext. These links will receive a `h3`-score > 0 when instead they should be 0. Thus, `h3` is not an admissible heuristic.

#### 2c.

Refer to the beginning of this document for a table containing nodes expanded / path lengths.

For intranet 1, my heuristic vastly outperformed breadth-first and depth-first searches. It also vastly outperforms breadth-first search on intranet 5; however, it only marginally outperforms depth-first on this intranet. Finally, on intranet 7 the heuristic again outperforms breadth-first. Depth-first, strangely, finds a solution incredibly quickly (about 250% faster than best-first). Altogether, the heuristic performed well on the sample intranets.
