git config --global credential.helper store
# her seferinde şifre sormayı bırakması için
# hatta zbu ayar için timeout da koyabilirsin:
git config --global credential.helper 'cache --timeout=600'
# çok hoş: (file contentini clipboard'a kopyala. başka güzel şeyleri de varmış xclip'in. man'ı oku diyorlar)
xclip -sel c < githubToken.txt

#import collections # used_words'daki duplicate'leri bulmak için. all_words_new'de 4 tane çıktı namussuz
#print([item for item, count in collections.Counter(used_words).items() if count > 1])

risale = "/home/b720/Desktop/1000words/collection/Signs+of+Miraculousness+-+Vahide.pdf"
from PyPDF4.pdf import PdfFileReader
reader = PdfFileReader(open(risale, "rb"))
print(reader.numPages)

In [19]: print(len(used_words))
    ...: print(len(unused_words))
    ...: print(len(all_words))
    ...: print(len(used_words) + len(unused_words))
    ...: print(len(all_words) - len(used_words) - len(unused_words))


"""
slash = []				# kalan eksikleri burası sayesinde buldum. meğer "bu satırdakilerden sadece birini kullan" dediklerimizden birden fazlasını kullanmışız
for i in all_words:
	if '/' in i:
	    slash.append(i.split(' / '))
for i in slash:
    p = 0
    for j in i:
        if j in used_words:
            p += 1
    if p > 1:
        print(i)
"""

def find_all(a_str, sub):
    start = 0
    while True:
        start = a_str.find(sub, start)
        if start == -1: return
        yield start
        start += len(sub) # use start += 1 to find overlapping matches
  
with open("/home/b720/Desktop/1000words/1000 Words/1000words_kullanılanlar.txt") as dosya:
	used_words = dosya.readlines()
used_words_new = []
for word in used_words:
	if ' - ' in word:
		word = word.split(' - ')[0]
	used_words_new.append(word.strip())
used_words = used_words_new
del used_words_new

with open("/home/b720/Desktop/1000words/1000 Words/1000words_tekrarsız.txt") as dosya:
	all_words = dosya.readlines()	
all_words_new = []
for word in all_words:
	if '(' in word or '[' in word:
		 word = word.split(' ')[0]
	all_words_new.append(word.strip())
all_words = all_words_new
del all_words_new					# emre gibi yazanlar elendi

all_words_new = []
for word in all_words:
	if '/' in word:
		all_words_new += word.split(' / ')
	else:
		all_words_new.append(word)			# / olanlar ayrıldı

unused_words = []
for word in all_words:
	if '/' not in word:
		if word not in used_words:
			unused_words.append(word.strip())
	else:
		sayaç = 0
		splitted_word = word.split(' / ')
		for w in splitted_word:
			if w in used_words:
				break
			else:
				sayaç += 1
				continue
		if sayaç == len(splitted_word):
			unused_words.append(word.strip())
unused_words_new = []
for word in unused_words:
	unused_words_new.append(word + '\n')
with open('/home/b720/Desktop/1000words/1000 Words/kullanılmamış_kelimeler.txt','w') as dosya:
	dosya.writelines(unused_words_new)
	
unused_words_new = []
for word in unused_words:
	if '/' in word:
		unused_words_new += word.split(' / ')
	else:
		unused_words_new.append(word)			# / olanlar ayrıldı
"""		
noluyor = []
for i in used_words:
	if i not in all_words_new:
		noluyor.append(i)
print(*noluyor,sep='\n')
"""
"""
noluyor = []
for i in unused_words:
	if i not in all_words:
		noluyor.append(i)
print(*noluyor,sep='\n')
"""










