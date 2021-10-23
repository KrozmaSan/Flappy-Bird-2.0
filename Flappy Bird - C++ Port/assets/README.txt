Adding of custom skins is now supported ! To do so, follow these guidelines :

- For the actual skin, draw it in the player.png file. Just use your favorite image editing software, and draw it in this very
specific manner: you might notice they're all arranged in rectangles. That's not just an impression: every skin takes up a 
17 * 12 rectangle from the top left. Just follow these dimensions and you'll be fine. If you're lost, just copy one existing skin and
draw over it.
- Once you're done, count the coordinates of the skin on the grid, starting at the 0th row and the 0th column being on the top left,
and the 15th row and 15th column being on the bottom right. That's your skin's identifier.
- Next, navigate to the cosmeticStrings.data file right above this directory, open it using any text editor, and add a new entry for
your skin. You can write it anywhere, but try to order it with the others to make it easier to find later on. For the format, just follow
the others, with these four fields, all prefixed by R$1C$2, with $1 the row and $2 the column:
	- Name : the skin's name
	- MainString : the skin's primary introductory sentence
	- SubString : in case the main string gets too long, add things here. Can be left empty.
	- Condition : the number of points at which the skin is unlocked.

So a typical skin would look like this :
R0C0Name:My skin !;
R0C0MainString:I'm an example.;
R0C0SubString:And I'm another !;
R0C0Condition:10;

That's all ! You should be good for adding your own. If you're confused, ask the dev ! He'll be glad to help.