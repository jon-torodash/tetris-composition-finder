tetris-composition-finder
=========================

A program in C++ to analyze tiling games to identify all possible configurations. Currently focuses on classic Tetris.

Just run "make" in the main repo directory. The executable build/tetris will be created.

This program counts the number of ways the bottom 4 rows in a Tetris board can be filled
PERFECTLY (no pieces jutting out), ie producing a "Tetris" maneuver in the game.

ASCII is used for graphical purposes. Each Tetris piece has a unique symbol for
each tile it occupies that attempts to closely resemble what the piece looks like on the board.

So for example, the box piece is

<table><tr><td>
O&nbsp;O<br/>
O&nbsp;O
</td><tr></table>

and the flat straight piece is:  - - - -
									
Grouping each contiguous char type will give a sense of the shape it is describing.

This problem is broken down into two tasks:

1) Decompose the board columnwise. We count how many ways the 10 column Tetris board can be broken up such with at least one single column component. (Because a single straight vertical Tetris piece  is needed to fill "complete" a Tetris.) So for example, {1,4,5} and {2,1,3,2,1,1} would work, but {4,6} and {3,3,2,2} would not, because they do not use at least one "1" as part of a column-group in decomposing the board.

2) For each possible column-group size 2 through 9, we count how many ways to fill a board of that size using standard backtracking. For size 1, the trivial case, has just the vertical straight Tetramino piece to fit so we note it and move on. 

Size 2 has 3:
<table>
<tr><td>O&nbsp;O&nbsp;&nbsp;&nbsp;P&nbsp;P&nbsp;&nbsp;&nbsp;7&nbsp;7<br/>
O&nbsp;O&nbsp;&nbsp;&nbsp;P&nbsp;J&nbsp;&nbsp;&nbsp;L&nbsp;7<br/>
O&nbsp;O&nbsp;&nbsp;&nbsp;P&nbsp;J&nbsp;&nbsp;&nbsp;L&nbsp;7<br/>
O&nbsp;O&nbsp;&nbsp;&nbsp;J&nbsp;J&nbsp;&nbsp;&nbsp;L&nbsp;L</td><tr>
</table>

Note that we DO NOT count for size 2 the fill configuration that consists of two vertical straight pieces. This is because the furst task has already found every unique decomposition of a 10 column board in general terms. (Counting a size 2 board fill with 2 vertical pieces effectively double counts configurations such as {4,2,2,1,1} and {4,1,1,1,1,1,1}. We found each of those configurations separately in step 1.) As the "mini boards" grow larger toward size 9, there are more chances to prematurely fill out the board, triggering a backtrack. This markedly reduces the runtime of the task.

The final step is to combine tasks 1 and 2. For each of the decompositions found in the first task, we multiply the number of ways that the group sizes in each composition can be filled from step 2. The product is stored for each composition, and the sum of these products is the total number of ways that a Tetris can be formed, on the bottom of the board, without any tiles sticking out at the top. The second number computed considers the notion of "activating" a Tetris, which must be done with a single straight vertical tetramino. In a decomposition such as {1,4,5} the "1" must be the final piece placed, so the product contributes the same to this second total. For {4,1,1,1,1,1,1} however, any one of the six 1s could have been the final piece dropped in, so this decomposition's configurations are multiplied by 6 before being added to the second total.

This code should be very straightforward to adapt to analyzing more complex tiling games.
