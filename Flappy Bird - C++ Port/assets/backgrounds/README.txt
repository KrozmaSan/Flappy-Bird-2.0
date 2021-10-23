Custom backgrounds ARE supported ! They just require a bit of thought put into it.

First, keep in mind that arbitrary sizes can work : however, if the image is too small, it'll probably look bad, 
and if it's too big, the scrolling will take a while. Also keep a reasonable size : SDL2 can't handle pictures larger than 8192 * 8192. 
But for all intents and purposes, you should be clear.

I personally recommend pictures that are close to the screen's resolution : so ideally 1920 * 1080.
Pro tip : if your image doesn't loop, i.e. the rightmost side doesn't connect with the leftmost one, mirror it and copy it to the right; now it loops !

Now for actually adding one :

	- First, take your picture and put it in the current folder : assets/backgrounds/.
	- Rename it like the others : bg$.png, where $ is ONE LARGER THAN THE PREVIOUS PICTURE. Emphasis on this one : add pictures IN ASCENDING NUMBERS. It will NOT WORK otherwise.
	- Then, open the backgroundStrings.data file (can be opened by any text editor), and add a new entry for your picture. Mirror the existing ones, but if you're still confused,
write it like this :
	bg$Name:theName;
	bg$Cond:theAmountOfPointsToUnlock;