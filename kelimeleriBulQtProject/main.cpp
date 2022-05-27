#include <QFile>
#include <signal.h>
//#include <unistd.h>
//#include <cstdlib>
#include <iostream>
#include <QtConcurrent/QtConcurrent>
#include <QRandomGenerator>
#include <QElapsedTimer>
#include <QTextStream>
//#include <QDebug>
//#include <QPdf>
// 37 ve 38'de ekstra kelimeler bulunabilir pek bakmadım
QString dosyayiAc(QString fileName, QIODevice::OpenModeFlag flag=QIODevice::ReadOnly)
{
    QFile file(fileName);
    if (!file.open(flag | QIODevice::Text))
		return {};
    QString text = file.readAll();
    file.close();
    return text;
}
#ifdef linux
QString base = "/home/b720/Desktop/1000words";
#else
QString base = "..";																				// TODO: neden olmadı
#endif

QStringList unused_words = dosyayiAc(base + "/1000 Words/kullanılmamışlar.txt").split('\n');
// TODO: self-exlanatory gibi olanları sacrifice ettim şimdilik. yani o selfexplanatory haline geldi
QString signs = dosyayiAc(base + "/collection/signs.txt").replace("-\n"," ").replace("\n"," ").replace("  "," ");
QString words = dosyayiAc(base + "/collection/words.txt").replace("-\n"," ").replace("\n"," ").replace("  "," ");
QString letters = dosyayiAc(base + "/collection/letters.txt").replace("-\n"," ").replace("\n"," ").replace("  "," ");
QString flashes = dosyayiAc(base + "/collection/flashes.txt").replace("-\n"," ").replace("\n"," ").replace("  "," ");
QString rays = dosyayiAc(base + "/collection/rays.txt").replace("-\n"," ").replace("\n"," ").replace("  "," ");
QString muhakemat = dosyayiAc(base + "/collection/muhakemat.txt").replace("-\n"," ").replace("\n"," ").replace("  "," ");
auto kitaplar = {&words, &flashes, &letters, &rays, &signs, &muhakemat};

const int readingTextLength = 2300;

QFile file(base + "/results1.txt");
QTextStream stream(&file);

QStringList sonuclar;

//int dongu = 0;
//int bulgu = 0;

/*void signal_callback_handler(int signum)
{
    QString print = QString::number(dongu) + ". döngü.. ---> " + QString::number(bulgu);
    std::cout << print.toStdString() << std::endl;
//    std::cout << "Sonuc: ";
//    for (auto sonuc : sonuclar)
//        std::cout << sonuc.toStdString() << ' ';
    // Terminate program
    //   exit(signum);
}*/
QVector<int> find_all(const QString &a_str, QString sub)
{
    int start = 0;
    QVector<int> allOccurrences;
    while(1)
    {
		start = a_str.indexOf(" " + sub, start);
//        std::cout << "start: " << start << '\n';
//        std::cout << "sub.length(): " << sub.length() << '\n';
		if (start == -1)
			start = a_str.indexOf("\n" + sub, start);			// TODO: burda sıkıntı olabilir. -1 meselesi
		if (start == -1)
            return allOccurrences;
        allOccurrences.append(start);
        start += sub.length(); // use start += 1 to find overlapping matches
    }
}
bool isInThisInterval(int baslangic_noktasi, QString word, const QString& text)
{
    return !find_all(text.mid(baslangic_noktasi, readingTextLength), word).isEmpty();
}
bool icinde_mi(int baslangic_noktasi, QStringList kombinasyon, const QString& text)
{
    if (isInThisInterval(baslangic_noktasi, kombinasyon[0], text))
    {
        if (kombinasyon.length() == 1)
            return true;
        return icinde_mi(baslangic_noktasi, kombinasyon.mid(1), text);
    }
    return false;
}
QStringList choices(QStringList words, uint8_t sizeOfGroup)
{
    QStringList words_new = {};
    QVector<int> used_indexes;

    for(size_t i=0; i<sizeOfGroup; ++i)
    {
        int randomIndex = rand() % words.length();
        if (used_indexes.contains(randomIndex))
        {
            --i;
            continue;
        }
        else
            used_indexes << randomIndex;
        words_new << words.at(randomIndex);
    }
    return words_new;
}
QStringList choose(QStringList words, uint8_t sizeOfGroup)
{
//	QStringList unusedwords_new = excludeForAMoment(unused_words, kombinasyon1);

    QStringList words_new = choices(words, sizeOfGroup);

    for(int index=0; index < words_new.length(); ++index)
        if (words_new.at(index).contains('/'))
        {
            QStringList splittedStr = words_new.at(index).split(" / ");
            int randomIndex = rand() % splittedStr.length();
            words_new[index] = splittedStr.at(randomIndex);
        }
    return words_new;
}
void trimTheList(QStringList silinecek_item)
{
    for (const QString& sil : silinecek_item)
    {
        bool silindiMi = unused_words.removeOne(sil);
        if (!silindiMi)
        {
            for (const QString& item : unused_words)
                if (item.split(" / ").contains(sil))
                    unused_words.removeOne(item);
        }
    }
}

QStringList excludeForAMoment(const QStringList& words, QStringList silinecek_item)
{
	QStringList words_new = words;
    for (const QString& sil : silinecek_item)
	{
		bool silindiMi = words_new.removeOne(sil);
		if (!silindiMi)
		{
            for (const QString& item : words_new)
				if (item.split(" / ").contains(sil))
					words_new.removeOne(item);
		}
	}
	return words_new;
}
void esas(QStringList kombinasyon, uint8_t num)
{
	uint8_t kitap = 0;
	bool break_it = false;
	for (auto text : kitaplar)
	{
		kitap += 1;
		QString word = kombinasyon.at(0);
        QVector<int> findings = find_all(*text, word);
//        int son_kelime_index = *std::max_element(findings.begin(), findings.end());
		if (!findings.isEmpty())
		{
			for (int finding : findings)
			{
				if (icinde_mi(finding, kombinasyon.mid(1), *text))
				{
//					bulgu += 1;
                    stream << "\nkombinasyon: ";
                    for (const QString &komb : kombinasyon)
                    {
                        stream << komb << " ";
                    }
					stream << "\nkitap: " << QString::number(kitap) << " - thread: " << num << " - index: " << finding << "\n";
                    stream << "\nMetin:\n" << text->mid(finding, readingTextLength).replace("\n", " ") << "\n";
//                    stream << "\nson part: " << text->mid(son_kelime_index,33);
					stream << "--------------------------------------------------------------------------------------------\n\n";
					stream.flush();
					trimTheList(kombinasyon);
					break_it = true;
//					sonuclar.append(kombinasyon);
//					sonuclar.append(QString::number(finding));
					break;
				}
			}
		}
		if (break_it)
        {
			break;
		}
	}
}

int main(int argc, char *argv[])
{
//	signal(SIGINT, signal_callback_handler);

    unused_words.pop_back();										// TODO: ??????????????? Bu ne saçmalık

	if (!file.open(QIODevice::Append | QIODevice::Text))
		return 2;

//    QElapsedTimer timer1;
//    QElapsedTimer timer2;

    int groupNumber;
    if (QString(argv[1]) == "")
        groupNumber = 7;
    else
        groupNumber = QString(argv[1]).toInt();
//    timer1.start();

    while (1)
    {
//        timer2.start();
//        dongu += 1;

        QStringList kombinasyonlar[10];

        kombinasyonlar[0] = choose(unused_words, groupNumber);
        QStringList unusedwords_new = excludeForAMoment(unused_words, kombinasyonlar[0]);
        kombinasyonlar[1] = choose(unusedwords_new, groupNumber);

        for(int i=2; i < 10; ++i)
        {
            unusedwords_new = excludeForAMoment(unusedwords_new, kombinasyonlar[i-1]);
            kombinasyonlar[i] = choose(unusedwords_new, groupNumber);
        }

//        unusedwords_new = excludeForAMoment(unusedwords_new, kombinasyon[1]);
//        kombinasyon[2] = choose(unusedwords_new, groupNumber);
//        unusedwords_new = excludeForAMoment(unusedwords_new, kombinasyon[2]);
//        kombinasyon[3] = choose(unusedwords_new, groupNumber);
//        unusedwords_new = excludeForAMoment(unusedwords_new, kombinasyon[3]);
//        kombinasyon[4] = choose(unusedwords_new, groupNumber);
//        unusedwords_new = excludeForAMoment(unusedwords_new, kombinasyon[4]);
//        kombinasyon[5] = choose(unusedwords_new, groupNumber);
//        unusedwords_new = excludeForAMoment(unusedwords_new, kombinasyon[5]);
//        kombinasyon[6] = choose(unusedwords_new, groupNumber);
//        unusedwords_new = excludeForAMoment(unusedwords_new, kombinasyon[6]);
//        kombinasyon[7] = choose(unusedwords_new, groupNumber);
//        unusedwords_new = excludeForAMoment(unusedwords_new, kombinasyon[7]);
//        kombinasyon[8] = choose(unusedwords_new, groupNumber);
//        unusedwords_new = excludeForAMoment(unusedwords_new, kombinasyon[8]);
//        kombinasyon[9] = choose(unusedwords_new, groupNumber);

        QFuture<void> futures[10];
        for(int i=0; i < 10; ++i)
        {
            futures[i] = QtConcurrent::run(esas, kombinasyonlar[i], i + 1);
        }

//		QFuture<void> future1 = QtConcurrent::run(esas, kombinasyon1, 1);
//		QFuture<void> future2 = QtConcurrent::run(esas, kombinasyon2, 2);
//		QFuture<void> future3 = QtConcurrent::run(esas, kombinasyon3, 3);
//        QFuture<void> future4 = QtConcurrent::run(esas, kombinasyon4, 4);
//        QFuture<void> future5 = QtConcurrent::run(esas, kombinasyon5, 5);
//		QFuture<void> future6 = QtConcurrent::run(esas, kombinasyon6, 6);
//		QFuture<void> future7 = QtConcurrent::run(esas, kombinasyon7, 7);
//		QFuture<void> future8 = QtConcurrent::run(esas, kombinasyon8, 8);
//		QFuture<void> future9 = QtConcurrent::run(esas, kombinasyon9, 9);
//		QFuture<void> future10 = QtConcurrent::run(esas, kombinasyon10, 10);

        for(int i=0; i < 10; ++i)
        {
            futures[i].waitForFinished();
        }

//		future1.waitForFinished();
//		future2.waitForFinished();
//		future3.waitForFinished();
//        future4.waitForFinished();
//        future5.waitForFinished();
//		future6.waitForFinished();
//		future7.waitForFinished();
//		future8.waitForFinished();
//		future9.waitForFinished();
//		future10.waitForFinished();

    }
    file.close();

    return 0;
}
/*Dignity 129, divulge 16 yerde geçiyo ama listede dignity yok!
	 Acquisition 17
consent 35*/
// 26. lemadan aldığım madem ölüm öldürülmüyor'lu yere itiraz gelirse: yeni okumaya başlayan bi adamın en fazla 4. 5. okuyacağı kitapta yazıyor bu
// muhakemattaki israiliyatın dipottaki iahını almadım ama orda bi iki kelime vardı illustrate mesela
