# coding: utf-8
with open("sil.txt") as dosya:
    text = dosya.read()
text1 = text.replace("-\n", "")
text1 = text1.replace("\n", " ")
text1 = text1.replace("  ", " ")
with open("sil1.txt","w") as dosya:
    dosya.write(text1)
    
