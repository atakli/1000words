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

QString dosyayiAc(QString fileName, QIODevice::OpenModeFlag flag=QIODevice::ReadOnly)
{
    QFile file(fileName);
    if (!file.open(flag | QIODevice::Text))
		return {};
    QString text = file.readAll();
    file.close();
    return text;
}
#if 0
QString base = "/home/b720/Desktop/1000words";
#else
QString base = "..";
#endif

QStringList unused_words = dosyayiAc(base + "/1000 Words/kullanılmamışlar.txt").split('\n');
QString signs = dosyayiAc(base + "/collection/signs.txt");
QString words = dosyayiAc(base + "/collection/words.txt");
QString letters = dosyayiAc(base + "/collection/letters.txt");
QString flashes = dosyayiAc(base + "/collection/flashes.txt");
QString rays = dosyayiAc(base + "/collection/rays.txt");
QString muhakemat = dosyayiAc(base + "/collection/muhakemat.txt");
auto kitaplar = {&words, &flashes, &letters, &rays, &signs, &muhakemat};

QFile file(base + "/results1.txt");
QTextStream stream(&file);

QStringList sonuclar;

int dongu = 0;
int bulgu = 0;

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
    if (find_all(text.mid(baslangic_noktasi, 2300), word).isEmpty())
        return false;
    else
        return true;
}
bool icinde_mi(int baslangic_noktasi, QStringList kombinasyon, const QString& text)
{
    if (isInThisInterval(baslangic_noktasi, kombinasyon[0], text))
    {

        if (kombinasyon.length() == 1)
            return true;
        return icinde_mi(baslangic_noktasi, kombinasyon.mid(1), text);
    }
    else
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
    QStringList words_new = choices(words, sizeOfGroup);

    for(int index=0; index<words_new.length(); ++index)
        if (words_new.at(index).contains('/'))
        {
            QStringList splittedStr = words_new.at(index).split(" / ");
            int randomIndex = rand() % splittedStr.length();
            words_new[index] = splittedStr.at(randomIndex);
        }
    return words_new;
}
//from itertools import combinations;
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
					bulgu += 1;
                    stream << "\nkombinasyon: ";
                    for (const QString &komb : kombinasyon)
                    {
                        stream << komb << " ";
                    }
                    stream << "\nmetin: " << text->mid(finding,2300);
//                    stream << "\nson part: " << text->mid(son_kelime_index,33);
                    stream << "\nkitap: " << QString::number(kitap) << " - thread: " << num << " - index: " << finding << "\n";
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
        dongu += 1;
		QStringList kombinasyon1 = choose(unused_words, groupNumber);
		QStringList unusedwords_new = excludeForAMoment(unused_words, kombinasyon1);
		QStringList kombinasyon2 = choose(unusedwords_new, groupNumber);
		unusedwords_new = excludeForAMoment(unusedwords_new, kombinasyon2);
		QStringList kombinasyon3 = choose(unusedwords_new, groupNumber);
        unusedwords_new = excludeForAMoment(unusedwords_new, kombinasyon3);
        QStringList kombinasyon4 = choose(unusedwords_new, groupNumber);
        unusedwords_new = excludeForAMoment(unusedwords_new, kombinasyon4);
        QStringList kombinasyon5 = choose(unusedwords_new, groupNumber);
        unusedwords_new = excludeForAMoment(unusedwords_new, kombinasyon5);
        QStringList kombinasyon6 = choose(unusedwords_new, groupNumber);

		QFuture<void> future1 = QtConcurrent::run(esas, kombinasyon1, 1);
		QFuture<void> future2 = QtConcurrent::run(esas, kombinasyon2, 2);
		QFuture<void> future3 = QtConcurrent::run(esas, kombinasyon3, 3);
        QFuture<void> future4 = QtConcurrent::run(esas, kombinasyon4, 4);
        QFuture<void> future5 = QtConcurrent::run(esas, kombinasyon5, 5);
        QFuture<void> future6 = QtConcurrent::run(esas, kombinasyon6, 6);

		future1.waitForFinished();
		future2.waitForFinished();
		future3.waitForFinished();
        future4.waitForFinished();
        future5.waitForFinished();
        future6.waitForFinished();

    }
    file.close();

    return 0;
}
