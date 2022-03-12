with open('/home/b720/Desktop/1000words/1000 Words/kullanılmamış_kelimeler.txt') as dosya:
	unused_words = dosya.readlines()
	
from pdfminer.high_level import extract_text
text = extract_text("/home/b720/Desktop/1000words/collection/Signs+of+Miraculousness+-+Vahide.pdf")
	
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
#unused_words1 = unused_words1
#kombinasyonlar = [] # bu satır dahi lüzumsuz değil
def komb_yap(silinecek_item, toBeCombined=[]):
	global unused_words
	for sil in silinecek_item:
		try:
			unused_words.remove(sil)
		except ValueError:
			for item in unused_words:
				if sil in item.split(' / '):
					unused_words.remove(item)
	kombinasyonlar = list(combinations(toBeCombined, 7))
	return kombinasyonlar

def choose(words, m):
	words_new = random.choices(words,k=m)
	for index, word in enumerate(words_new):
		if '/' in word:
			words_new[index] = random.choice(word.split(' / '))
	return words_new
	

sonuç = []
döngü = 0
bulgu = 0
break_it = 0
dosya = open("/home/b720/Desktop/1000words/results.txt",'w+a')
while 1: 
	try:
		döngü += 1													
		unused_words1 = choose(unused_words, 7)			
		#unused_words1 = ["diverse", "constant", "faith", "remedies"] 
		print("unused_words1: ", unused_words1)
		kombinasyonlar = komb_yap([], unused_words1)
		for komb in kombinasyonlar:
			print("komb: ", komb)
			print(döngü,". döngü.."," ---> ", bulgu, sep='')
			word = komb[0]
			findings = list(find_all(text, word))
			if findings:
				for finding in findings:
					if icinde_mi(finding, komb[1:]):
						bulgu += 1
						print("kombinasyon: ",komb, file=dosya, flush=True)
						print("index: ",finding, file=dosya, flush=True)
						print('\n', file=dosya, flush=True)
						komb_yap(komb)
						sonuç.append((komb, finding, word))
						break_it = 1
						break
			if break_it == 1:
				break_it = 0
				break
	except KeyboardInterrupt:
		print(döngü,". döngü..", sep='')
		print("sonuç: ", sonuç)
		break
dosya.close()
