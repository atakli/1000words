#include <QFile>
#include <signal.h>
//#include <unistd.h>
//#include <cstdlib>
#include <iostream>
#include <QRandomGenerator>
#include <QTextStream>
//#include <QPdf>

QString dosyayiAc(QString fileName, QIODevice::OpenModeFlag flag=QIODevice::ReadOnly)
{
    QFile file(fileName);
    if (!file.open(flag | QIODevice::Text))
        return {};
//    QTextStream in(&file);
//    QString text = in.readAll();
    QString text = file.readAll();
    file.close();
    return text;
}

QStringList unused_words = dosyayiAc("/home/b720/Desktop/1000words/1000 Words/kullanılmamış_kelimeler.txt").split('\n');
QString text = dosyayiAc("/home/b720/Desktop/1000words/collection/signs.txt");

QStringList sonuclar;
int dongu = 0;
int bulgu = 0;
QFile file("/home/b720/Desktop/1000words/results1.txt");

void signal_callback_handler(int signum)
{
    QString print = QString::number(dongu) + ". döngü.. ---> " + QString::number(bulgu);
    std::cout << print.toStdString() << std::endl;
    std::cout << "Sonuc: ";
    for (auto sonuc : sonuclar)
        std::cout << sonuc.toStdString() << ' ';
    // Terminate program
    //   exit(signum);
}
QVector<int> find_all(QString a_str, QString sub)
{
    int start = 0;
    QVector<int> allOccurrences;
    while(1)
    {
        start = a_str.indexOf(sub, start);
        if (start == -1)
            return allOccurrences;
        allOccurrences.append(start);
        start += sub.length(); // use start += 1 to find overlapping matches
    }
}
bool isInThisInterval(int baslangic_noktasi, QString word, QString text)
{
    if (find_all(text.mid(baslangic_noktasi, 2300), word).isEmpty())
        return false;
    else
        return true;
}
bool icinde_mi(int baslangic_noktasi, QStringList kombinasyon, QString text)
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
    std::cout << "burda1\n";
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
void trimTheList(QStringList silinecek_item/*, QStringList toBeCombined={}*/)
{
    for (auto sil : silinecek_item)
    {
        bool silindiMi = unused_words.removeOne(sil);
        if (!silindiMi)
        {
            for (auto item : unused_words)
                if (item.split(" / ").contains(sil))
                    unused_words.removeOne(item);
        }
    }
    // kombinasyonlar = list(combinations(toBeCombined, 7))
    // return kombinasyonlar
}
void write(QStringList kombinasyon, int index)
{
    file.write("kombinasyon: ");
    for (QString komb : kombinasyon)
    {
        file.write(komb.toStdString().c_str());
        file.write(" ");
    }
    file.write("\n");
    file.write("index: ");
    file.write(std::to_string(index).c_str());
    file.write("\n");
    file.write("\n");
}
int main(int argc, char *argv[])
{
    signal(SIGINT, signal_callback_handler);

    if (!file.open(QIODevice::Append | QIODevice::Text))
        return 2;
    int groupNumber;
    if (QString(argv[1]) == "")
        groupNumber = 7;
    else
        groupNumber = QString(argv[1]).toInt();

    while (1)
    {
        std::cout << dongu << ". döngü.." << " ---> " << bulgu << '\n';
        dongu += 1;
        QStringList kombinasyon = choose(unused_words, groupNumber);

        QString word = kombinasyon.at(0);
        QVector<int> findings = find_all(text, word);
        if (!findings.isEmpty())
        {
            for (int finding : findings)
            {
                if (icinde_mi(finding, kombinasyon.mid(1), text))
                {
                    bulgu += 1;
                    write(kombinasyon, finding);
                    trimTheList(kombinasyon);
                    sonuclar.append(kombinasyon);
                    sonuclar.append(QString::number(finding));
                    break;
                }
            }
        }
    }
    file.close();

    return 0;
}
