This is a self-written indexing && searching
Author z5050181 Tianxing Hu.
The structure would be like:
	- a3search.cpp
	- makefile
	- stmr.cpp
	- stmr.h
	- stopwords.cpp	
	- Synonyms.dictionary
1.Indexing
	1st.  build a fileindex file 
	      use map<fileid,filename> as struct
	2nd.  build a word index file
	      1.dealing with Caps,non-words&stem,moreover delete stop words.
		which stem word by using stmr.
	      2.use map<word,map<fileID,frequncy> as word index.

2.Searching
	1st. read index file by line 
	     check line head whether has search term.
	     If has then :
		using map<fileId,Map<word,frequncy>> store record.
	2rd. check weather each file have whole search team

3.Ranking 
	sort first according to occurrences, second lexicagraphically from search map
	out finally result.

4.Concept searching 
 	idea :
	find search key word‘s Synonyms.
	Then use Synonyms to do search.
	Last out final result.
	
    
