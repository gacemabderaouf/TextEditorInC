# TextEditorInC
## Project purpose:

  Given text in French language, we want to implement some processing functions of
text, such as search for a word, search and replace a word with another given some options, ...etc.


## Realized Work
					 
  A C program to manipulate a text file. A menu navigation is used to do one of the following processing :
- Display the file page by page with the following information : Number of characters, number of words, number of lines and number of pages.
- Paragraphs formatting : align to left, right, left and right, and center. To get the formatting done, the user have to insert symbols before and after the paragraph to deal with. For example if you put /c/ before and after a paragraph, it will be centered.
- Search and replace function with its different formulas. When the searched word is not found, display the word or words that are closest to it . If it is an approximate search it must be indicated. In order to perform this function, it will be necessary to build an index file containing all the words in the file with their location. The search for a word will be done in the index. In case of replacement the index is updated.

## Logic explanation

1- When the user opens the file, we perform a first pass to do a syntax analysis (calculates the number of tags...);
2- An other pass is performed and it depends on the first one, the purpose is to index the page borders, calculate the number of words, the number of lines... and index also the words in several sorted index files (To make a dichotomous search) by indexing the lines too.
3- To replace a word we create a new file then (jump to 1).

## Program use
				
1- Enter the name of an existing file otherwise the program will end.
2- Use the "up and down" arrows to go to the previous/next page.
3- Select a number [0-9] a message will be displayed, enter the page number reach it.
4- Click on R/r to search for a word if it exists, it will be colored and to go to the next page click on Enter, if you want to cancel the search click Esc, if it doesn't exist a message will be displayed and you can search the closest words.
5- To replace a word it is necessary to seek it first, then to click on F/f, a message will be displayed, enter the new word, to replace the following occurence click Enter then another F/f.
