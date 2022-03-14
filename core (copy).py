#from pdfminer.high_level import extract_text
#text = extract_text("/home/b720/Desktop/1000words/collection/Signs+of+Miraculousness+-+Vahide.pdf")

with open('/home/b720/Desktop/1000words/1000 Words/kullanılmamış_kelimeler.txt') as dosya:
	unused_words1 = dosya.readlines()
unused_words = []
for i in unused_words1:
	unused_words.append(i.strip())

groupNumber = 6

with open('/home/b720/Desktop/1000words/collection/signs.txt') as dosya:
	signs = dosya.read()
with open('/home/b720/Desktop/1000words/collection/words.txt') as dosya:
	words = dosya.read()
with open('/home/b720/Desktop/1000words/collection/letters.txt') as dosya:
	letters = dosya.read()
with open('/home/b720/Desktop/1000words/collection/flashes.txt') as dosya:
	flashes = dosya.read()
with open('/home/b720/Desktop/1000words/collection/rays.txt') as dosya:
	rays = dosya.read()
#text = signs

def find_all(a_str, sub):
	start = 0
	while True:
		start = a_str.find(' '+sub, start)
		if start == -1:
			start = a_str.find('\n'+sub, start)
		if start == -1: return
		yield start
		start += len(sub) # use start += 1 to find overlapping matches

def isInThisInterval(baslangic_noktasi, word):
	global text
	if list(find_all(text[baslangic_noktasi:baslangic_noktasi+2300], word)):
	    return True
	else:
	    return False

def icinde_mi(baslangic_noktasi, kombinasyon):
	if isInThisInterval(baslangic_noktasi, kombinasyon[0]):
	    if len(kombinasyon) == 1:
	        return True
	    return icinde_mi(baslangic_noktasi, kombinasyon[1:])
	else:
	    return False

import random
from itertools import combinations
#kombinasyonlar = [] # bu satır dahi lüzumsuz değil
def komb_yap(silinecek_item):
	global unused_words
	for sil in silinecek_item:
	    try:
	        unused_words.remove(sil)
	    except ValueError:
	        for item in unused_words:
	            if sil in item.split(' / '):
	                unused_words.remove(item)
	#kombinasyonlar = list(combinations(toBeCombined, groupNumber))
	#return kombinasyonlar

def choose(words, m):
	words_new = random.sample(words,k=m)
	for index, word in enumerate(words_new):
	    if '/' in word:
	        words_new[index] = random.choices(word.split(' / '))[0]
	return words_new

#import time
#sonuç = []
break_it = 0
döngü = 0
bulgu = 0
dosya = open("/home/b720/Desktop/1000words/results.txt",'a') # w+a doğru olmayabilir
#en_start = time.time()

def main():


while 1:
	#start = time.time()
	try:
		döngü += 1
		kombinasyon = choose(unused_words, groupNumber)
t1 = threading.Thread(target=print_square, args=(10,))
t2 = threading.Thread(target=print_cube, args=(10,))

		kitap = 0
		for text in [words, letters, flashes, rays, signs]:
			kitap += 1
			word = kombinasyon[0]
			findings = list(find_all(text, word))
			if findings:
				for finding in findings:
					if icinde_mi(finding, kombinasyon[1:]):
						bulgu += 1
						print(kitap, ": ",text[finding:finding+100], file=dosya, flush=True)
						print("kombinasyon: ",kombinasyon, file=dosya, flush=True)
						print("son kelime: ", text[finding+2200:finding+2300], file=dosya, flush=True)
						print("index: ",finding, file=dosya, flush=True)
						print("---------------------------------------------------------------------------------------------------------", file=dosya, flush=True)
						print('\n', file=dosya, flush=True)
						komb_yap(kombinasyon)
			            #sonuç.append((kombinasyon, finding, word))
						break_it = 1
						break
			if break_it == 1:
				break_it = 0
				break
	except KeyboardInterrupt:
		print(döngü,". döngü..", sep='')
	    #print("sonuç: ", sonuç)
	    #break
	#print("süre: ",time.time() - start)
	#print("ortalama süre: ", (time.time() - en_start) / döngü)
dosya.close()
