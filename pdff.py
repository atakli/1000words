risale = "/home/b720/Desktop/1000words/collection/Signs+of+Miraculousness+-+Vahide.pdf"
from sys import path
path.append("/home/b720/Downloads/PyPDF4")
from pypdf.pdf import PdfFileReader
reader = PdfFileReader(open(risale, "rb"))
print(reader.numPages)

def find_all(a_str, sub):
    start = 0
    while True:
        start = a_str.find(sub, start)
        if start == -1: return
        yield start
        start += len(sub) # use start += 1 to find overlapping matches

from pdfminer.high_level import extract_text
text = extract_text("/home/b720/Desktop/1000words/collection/Signs+of+Miraculousness+-+Vahide.pdf")

with open("/home/b720/Desktop/1000words/1000 Words/1000words_kullanılanlar.txt") as dosya:
	used_words = dosya.readlines()
used_words_new = []
for word in used_words:
	if ' ' in word:
		word = word.split(' ')[0]
	used_words_new.append(word)
used_words = used_words_new
del used_words_new
with open("/home/b720/Desktop/1000words/1000 Words/1000words_tekrarsız.txt") as dosya:
	all_words = dosya.readlines()
	
all_words_new = []
for word in all_words:
	if '(' in word or '[' in word:
		 word = word.split(' ')[0]
	all_words_new.append(word)
all_words = all_words_new
del all_words_new					# emre gibi yazanlar elendi
"""
all_words_new = []
for word in all_words:
	if '/' in word:
		all_words_new += word.split(' / ')
	all_words_new.append(word)
all_words = all_words_new
del all_words_new					# / olanlar ayrıldı
"""
unused_words = []
for word in all_words:
	if '/' not in word:
		if word not in used_words:
			unused_words.append(word)
	else:
		for w in word.split(' / '):
			if w in used_words:
				break
			else:
				unused_words.append(word)

with open('/home/b720/Desktop/1000words/1000 Words/kullanılmamış_kelimeler.txt','w') as file:
	file.writelines(unused_words)
		
for word in all_words:
	if word not in used_words:
		findings = list(find_all(text, word))
		if findings:
			for finding in findings:
				for word_2 in all_words:
			
		( [ 
		/

